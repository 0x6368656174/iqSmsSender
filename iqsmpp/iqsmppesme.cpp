//#define DEBUG

#define ENQUIRE_CHECK_DEFAULT_INTERVAL 2500

#include "iqsmppesme.h"
#include "iqsmppabstractpdu.h"
#include "iqsmppbindpdu.h"
#include "iqsmppunbindpdu.h"
#include "iqsmppsubmitsmpdu.h"
#include "iqsmppgenericnackpdu.h"
#include "iqsmppenquirelinkpdu.h"
#include "iqsmppenquirelinkresppdu.h"
#include "iqsmppbindresppdu.h"
#include "iqsmppunbindresppdu.h"
#include <QDebug>

IQSMPPESME::IQSMPPESME(QObject *parent) :
    QObject(parent),
    _hostName("127.0.0.1"),
    _hostPort(2775),
    _socket(new QTcpSocket(this)),
    _state(Disconnected),
    _smscSystemId(""),
    _bindWaitTime(15000),
    _lastEnquireLinkSequenceNumerSend(0),
    _lastEnquireLinkSequenceNumerReseived(0),
    _enquireLinkSenderTimer(new QTimer(this))

{
    connect(_enquireLinkSenderTimer, SIGNAL(timeout()), this, SLOT(checkEnquire()));

    _enquireLinkSenderTimer->setInterval(ENQUIRE_CHECK_DEFAULT_INTERVAL);
}

void IQSMPPESME::setHostName(const QString &hostName)
{
    _hostName = hostName;
}

void IQSMPPESME::setHostPort(const quint16 hostPort)
{
    _hostPort = hostPort;
}

void IQSMPPESME::setBindWaitTime(const int time)
{
    _bindWaitTime = time;
}

IQSMPPBindOptions * IQSMPPESME::bindOptions()
{
    return &_bindOptions;
}

bool IQSMPPESME::bindToSMSC(const QString &hostName, const quint16 port)
{
    setHostName(hostName);
    setHostPort(port);
    return bindToSMSC();
}

void IQSMPPESME::setEnquireLinkCheckInterval(const int interval)
{
    _enquireLinkSenderTimer->setInterval(interval);
}

int IQSMPPESME::enquireLinkCheckInterval() const
{
    return _enquireLinkSenderTimer->interval();
}

bool IQSMPPESME::bindToSMSC()
{   
    _smscSystemId = "";
    //Отконектим автоматическую обработку
    disconnect(_socket, SIGNAL(readyRead()), this, SLOT(readPDUFromSocket()));

    _socket->connectToHost(_hostName, _hostPort);
    if (!_socket->waitForConnected(_bindWaitTime))
        return false;

    //Выполним бинд
    IQSMPPBindPDU bindPdu (_bindOptions);
    QByteArray bindBA = bindPdu.encode();
#ifdef DEBUG
    qDebug() << tr("Transmitted %0").arg(IQSMPP::bytes2String(bindBA));
#endif
    _socket->write(bindBA);
    if (!_socket->waitForBytesWritten(_bindWaitTime))
        return false;

    //Получим биндресп
    if (!_socket->waitForReadyRead())
        return false;
    //Прочитаем ответ
    QByteArray bindRespBA = _socket->readAll();
#ifdef DEBUG
    qDebug()<< tr("Reserved %0").arg(IQSMPP::bytes2String(bindRespBA));
#endif
    //Законектим все отсальные ответы на слот автоматической обработки
    connect(_socket, SIGNAL(readyRead()), this, SLOT(readPDUFromSocket()));

    IQSMPPBindRespPDU bindRespPdu (0);
    //Попробуем распарсить пакет
    if (bindRespPdu.decode(bindRespBA) != IQSMPP::ESME_ROK)
    {
        return false;
    }
    //Сообщим о том, что получили pdu
    emit pduReserved(bindRespPdu);
    //Проверим, чтоб статус был ок
    if (bindRespPdu.commandStatus() != IQSMPP::ESME_ROK)
        return false;

    //Установим название SMSC
    _smscSystemId = bindRespPdu.options()->systemId();

    //Запустим проверку связи
    _lastEnquireLinkSequenceNumerReseived = 0;
    _lastEnquireLinkSequenceNumerSend = 0;
    _enquireLinkSenderTimer->start();

    _state = Connected;
    emit connected();
    return true;
}

bool IQSMPPESME::unbindFromSMSC()
{
    bool result = true;
    if (_socket->isOpen())
    {
        //Выполним анбинд
        IQSMPPUnbindPDU unbindPDU;
        QByteArray unbindBA = unbindPDU.encode();
        _socket->write(unbindBA);
        _socket->waitForBytesWritten(1000);

        _socket->disconnectFromHost();
        result = _socket->waitForDisconnected();
    }

    _smscSystemId = "";

    //Остановим проверку связи
    _enquireLinkSenderTimer->stop();

    _state = Disconnected;
    emit disconnected();
    return result;
}

bool IQSMPPESME::sendPDU(const IQSMPPAbstractPDU &pdu)
{
    QByteArray pduPackage = pdu.encode();
#ifdef DEBUG
    qDebug() << tr("Transmitted %0").arg(IQSMPP::bytes2String(pduPackage));
#endif
    if (pduPackage.length() == 0)
        return false;

    _socket->write(pduPackage);
    return _socket->waitForBytesWritten();
}

void IQSMPPESME::readPDUFromSocket()
{
    QByteArray pduPackage = _socket->readAll();

#ifdef DEBUG
    qDebug()<< tr("Reserved %0").arg(IQSMPP::bytes2String(pduPackage));
#endif

    quint32 pduLength;
    IQSMPP::Commands pduCommand;
    IQSMPP::CommandStatus pduCommandStatus;
    quint32 pduSequenceNumber;

    if (!IQSMPPAbstractPDU::decodeHeader(pduPackage, &pduLength, &pduCommand, &pduCommandStatus, &pduSequenceNumber))
        return;

    //Проверим, что длина pdu соответствует заявленной
    if (static_cast<int>(pduLength) != pduPackage.size())
    {
        sendGenerickNack(pduSequenceNumber, IQSMPP::ESME_RINVMSGLEN);
        return;
    }

    //Проверим, чтоб была указана правильная команда
    //Команда должна находиться в разрешенном диапозоне
    quint32 pduCommandInt = static_cast<quint32>(pduCommand);
    if (!( pduCommandInt <= 0x000001FF )
            && !(pduCommandInt >=0x000001FF && pduCommandInt <= 0x800001FF))
    {
        sendGenerickNack(pduSequenceNumber, IQSMPP::ESME_RINVCMDID);
        return;
    }

    //В зависимости от типа команды
    switch (pduCommand)
    {
    case IQSMPP::BindReceiver:
    case IQSMPP::BindTransceiver:
    case IQSMPP::BindTransmitter:
    {
        IQSMPPBindPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK)
            emit pduReserved(result);
        return;
        break;
    }
    case IQSMPP::Unbind:
    {
        IQSMPPUnbindPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK)
            emit pduReserved(result);
        return;
        break;
    }
    case IQSMPP::SubmitSM:
    {
        IQSMPPSubmitSMPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK)
            emit pduReserved(result);
        return;
        break;
    }
    case IQSMPP::GenericNack:
    {
        IQSMPPGenericNackPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK)
            emit pduReserved(result);
        return;
        break;
    }
    case IQSMPP::UnbindResp:
    {
        IQSMPPUnbindRespPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK)
            emit pduReserved(result);
        return;
        break;
    }


        //Ответ на бинд, обрабатывается вручную в запросе бинд
    case IQSMPP::BindReceiverResp:
    case IQSMPP::BindTransceiverResp:
    case IQSMPP::BindTransmitterResp:
    {
        return;
        break;
    }

        //Запрошена проверка связи
    case IQSMPP::EnquireLink:
    {
        IQSMPPEnquireLinkPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK)
            sendEnquireLinkResp(result.sequenceNumber());
        return;
        break;
    }
        //Ответ на наш запрос проверки связи
    case IQSMPP::EnquireLinkResp:
    {
        IQSMPPEnquireLinkRespPDU result;
        if (result.decode(pduPackage) == IQSMPP::ESME_ROK && result.commandStatus() == IQSMPP::ESME_ROK)
            _lastEnquireLinkSequenceNumerReseived = result.sequenceNumber();
        return;
        break;
    }
    }
}

void IQSMPPESME::sendGenerickNack(const quint32 sequenceNumber, const IQSMPP::CommandStatus error)
{
    IQSMPPGenericNackPDU pdu(sequenceNumber);
    pdu.setCommandStatus(error);
    sendPDU(pdu);
}

void IQSMPPESME::sendEnquireLinkResp(const quint32 sequenceNumber)
{
    IQSMPPEnquireLinkRespPDU pdu (sequenceNumber);
    sendPDU(pdu);
}

void IQSMPPESME::checkEnquire()
{
    //Если ид последнено отправленного запроса не совпадает ид последнего полученного ответа, то рвем связь
    if (_lastEnquireLinkSequenceNumerReseived != _lastEnquireLinkSequenceNumerSend)
    {
        unbindFromSMSC();
        return;
    }

    //Отправим новый запрос на проверку связи
    IQSMPPEnquireLinkPDU pdu;
    _lastEnquireLinkSequenceNumerSend = pdu.sequenceNumber();
    sendPDU(pdu);
}
