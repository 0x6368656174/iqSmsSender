#define RECONECT_INTEVAL 15000

#include "iqsmssender.h"
#include "iqsettingmanager.h"
#include "iqhistoryloger.h"
#include "iqsmpp/iqsmppabstractpdu.h"
#include "iqsmpp/iqsmppoptionalparameter.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>
#include <cmath>

IQSMSSender::IQSMSSender(QObject *parent) :
    QObject(parent),
    _esme(new IQSMPPESME(this)),
    _sarMsgRefNum(0),
    _senderTimer(new QTimer(this)),
    _reconectTimer(new QTimer(this))
{
    connect(_senderTimer, SIGNAL(timeout()), this, SLOT(sendNextSumbitSM()));
    connect(_reconectTimer, SIGNAL(timeout()), this, SLOT(reconect()));
}

bool IQSMSSender::start()
{
    _reconectTimer->stop();
    bool result = reconect();
    if (!result)
    {
        QMessageBox::warning(0, tr("Connection to SMSC failed"), tr("A connection attempt failed. Check the connection settings or contact your technical support SMSC."));
    }
    else
    {
        _reconectTimer->start(RECONECT_INTEVAL);
    }
    return result;
}

bool IQSMSSender::reconect()
{
    if (_esme->state() == IQSMPPESME::Connected)
        return true;

    _senderTimer->setInterval(1000 / IQSettingManager::smppSMSPerSecond());

    //Биндимся к SMSC
    _esme->setBindWaitTime(5000);
    _esme->setHostName(IQSettingManager::smppHostName());
    _esme->setHostPort(IQSettingManager::smppHostPort());
    _esme->setEnquireLinkCheckInterval(IQSettingManager::smppEnqueireLinkSendInterval()*1000);
    _esme->bindOptions()->setSystemId(IQSettingManager::smppSystemId());
    _esme->bindOptions()->setPassword(IQSettingManager::smppPassword());

    if (!_esme->bindToSMSC())
    {
        IQHistoryLoger::write(tr("Connection to SMSC failed"), true);
        return false;
    }

    _senderTimer->start();
    IQHistoryLoger::write(tr("Connection to SMSC %0").arg(_esme->smscSystemId()));

    return true;
}

void IQSMSSender::stop()

{
    _reconectTimer->stop();
    _senderTimer->stop();
    _esme->unbindFromSMSC();
    IQHistoryLoger::write(tr("Connection to SMSC closed."));
}

void IQSMSSender::sendReservationNotify(const qint64 phoneNumber,
                                        const qint64 reservationNumber,
                                        const QDateTime &reservationDateTime,
                                        const QDateTime &performanceDateTime,
                                        const QString &reservationWorld)
{
    QString senderText = IQSettingManager::senderText();

    QString reservationNumberStr = QString("%0").arg(QString::number(reservationNumber), 7, '0');
    senderText = senderText.replace("%RESERVATION_NUMBER%", reservationNumberStr);
    senderText = senderText.replace("%RESERVATION_WORLD%", reservationWorld);
    senderText = senderText.replace("%RESERVATION_DATE%", reservationDateTime.toString("dd.MM.yyyy hh:mm"));
    senderText = senderText.replace("%PERFORMANCE_DATE%", performanceDateTime.toString("dd.MM.yyyy hh:mm"));

    sendMessage(phoneNumber, senderText);
}

void IQSMSSender::sendMessage(const qint64 phone, const QString &message)
{
    //Кодируем русский текст
    QByteArray messageBA;
    bool messageContainRussianSumbols = hasRussianSumbols(message);
    if (messageContainRussianSumbols)
    {
        QTextCodec *utf16Codec = QTextCodec::codecForName("UTF-16BE");
        messageBA = utf16Codec->fromUnicode(message);
        messageBA.remove(0, 2); //Удаляем первые 2 октета - это BOM
    }
    else
    {
        messageBA = message.toLatin1();
    }

    //Разбиваем сообщение на более мелкие куски
    bool result = true;
    int position = 0;
    int segmentNumber = 1;
    int totalSegment = ceil((qreal)messageBA.size() / 134);
    _sarMsgRefNum++;

    while (position < messageBA.size())
    {
        IQSMPPSubmitSMPDU submitSM;
        submitSM.options()->setSourceAddresNPI(IQSettingManager::smppSourceNPI());
        submitSM.options()->setSourceAddresTON(IQSettingManager::smppSourceTON());
        submitSM.options()->setSourceAddres(IQSettingManager::smppSourceAddres());
        submitSM.options()->setDestinationAddresNPI(IQSettingManager::smppDestinationNPI());
        submitSM.options()->setDestinationAddresTON(IQSettingManager::smppDestinationTON());
        submitSM.options()->setDestinationAddres(QString::number(phone));

        QByteArray package;
        if (messageBA.size()> 140)
        {
            submitSM.options()->setESMClass(0x40); //Включаем поддержку UDH
            //Добавим udh
            package.append((char)0x05);
            package.append((char)0x00);
            package.append((char)0x03);
            package.append((char)_sarMsgRefNum);
            package.append((char)totalSegment);
            package.append((char)segmentNumber);
        }

        if (messageContainRussianSumbols)
            submitSM.options()->setDataCodingType(IQSMPP::UCS2);

        package.append(messageBA.mid(position, 134));
        submitSM.options()->setShortMessage(package);

        _submitPDUStack.enqueue(submitSM);

        position += messageBA.size()> 140?134:140;
        segmentNumber++;
    }

    if (result)
    {
        IQHistoryLoger::write(tr("Sendet message to %0 \"%1\"").arg(phone).arg(message));
    }
    else
    {
        IQHistoryLoger::write(tr("Error send message to %0 \"%1\"").arg(phone).arg(message));
    }

}

bool IQSMSSender::hasRussianSumbols(const QString &string) const
{
    foreach (QChar ch, string)
    {
        if (ch.unicode() >= 0x0400 && ch.unicode() <= 0x04FF)
            return true;
    }
    return false;
}

void IQSMSSender::sendNextSumbitSM()
{
    if (_esme->state() != IQSMPPESME::Connected)
        return;
    if (_submitPDUStack.empty())
        return;
    IQSMPPSubmitSMPDU submitPDU = _submitPDUStack.dequeue();

    _esme->sendPDU(submitPDU);
}

