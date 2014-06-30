#include "iqsmppabstractpdu.h"
#include "iqsmppbindpdu.h"
#include "iqsmppunbindpdu.h"
#include "iqsmppsubmitsmpdu.h"
#include <QDataStream>
#include <QDebug>

QAtomicInt IQSMPPAbstractPDU::_autoSequenceNumber = 1;

IQSMPPAbstractPDU::IQSMPPAbstractPDU(const quint32 sequenceNumber):
    _commandStatus(IQSMPP::ESME_ROK),
    _optionalParametersWasSets(false)
{
    if (sequenceNumber > AutoIncrement)
    {
        setSequenceNumber(sequenceNumber);
    }
    else
    {
        setSequenceNumber(_autoSequenceNumber.fetchAndAddRelaxed(1));
    }
}

IQSMPPAbstractPDU::~IQSMPPAbstractPDU()
{
}

void IQSMPPAbstractPDU::setSequenceNumber(const quint32 number)
{
    _sequenceNumber = number;
}

void IQSMPPAbstractPDU::setCommandStatus(const IQSMPP::CommandStatus status)
{
    _commandStatus = status;
}

QString IQSMPPAbstractPDU::readCStringFromStream(QDataStream &stream) const
{
    quint8 bit;

    QByteArray resultBA;
    stream >> bit;
    resultBA.append(bit);
    while (bit != 0x00)
    {
        stream >> bit;
        resultBA.append(static_cast<char>(bit));
    }

    QString result = QString::fromLatin1(resultBA);
    return result;
}

QByteArray IQSMPPAbstractPDU::encode() const
{
    QByteArray result;
    QDataStream stream (&result, QIODevice::WriteOnly);
    //Создадим заголовок
    stream << (quint32)0; //Сохраним место под длину
    stream << (quint32)command();
    stream << (quint32)_commandStatus;
    stream << _sequenceNumber;
    //Добавим тело
    QByteArray bodyBA = encodeBody();
    stream.writeRawData(bodyBA.constData(), bodyBA.length());

    //Добавим опциональные параметры
    QHashIterator<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter > > optionalParametrsI(_optionalParameters);
    while (optionalParametrsI.hasNext())
    {
        optionalParametrsI.next();
        if (!optionalParametrsI.value())
            continue;
        QByteArray optionalParametrBA = optionalParametrsI.value()->encode();
        if (optionalParametrBA.size() > 0)
            stream.writeRawData(optionalParametrBA.constData(), optionalParametrBA.size());
    }

    //Обновим длину
    stream.device()->seek(0);
    stream << (quint32)result.size();

    return result;
}

bool IQSMPPAbstractPDU::decodeHeader(const QByteArray &pdu,
                                     quint32 *length,
                                     IQSMPP::Commands *command,
                                     IQSMPP::CommandStatus *commandStatus,
                                     quint32 *sequenceNumber)
{
    if (pdu.length() < 16)
        return false;
    QByteArray pduCopy (pdu);
    QDataStream stream (&pduCopy, QIODevice::ReadOnly);
    stream >> *length;
    quint32 commandId;
    stream >> commandId;
    quint32 commandStatusId;
    stream >> commandStatusId;
    stream >> *sequenceNumber;
    *command = static_cast<IQSMPP::Commands>(commandId);
    *commandStatus = static_cast<IQSMPP::CommandStatus>(commandStatusId);
    return true;
}

void IQSMPPAbstractPDU::encodeOptionalParameters(QByteArray &optionalParametersBA)
{
    QByteArray ba(optionalParametersBA);
    QDataStream stream(&ba, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        quint16 tagId;
        quint16 length;

        stream >> tagId;
        stream >> length;

        char * data = new char[length];
        stream.readRawData(data, length);
        QByteArray optionsBA(data, length);
        delete[] data;
        data = NULL;

        IQSMPP::OptionalParameters tag = static_cast<IQSMPP::OptionalParameters>(tagId);
        if (!_optionalParametersWasSets)
        {
            _optionalParameters = optionalParameters();
            _optionalParametersWasSets = true;
        }
        if (!_optionalParameters.contains(tag))
            continue;
        if (!_optionalParameters[tag])
            continue;

        _optionalParameters[tag]->decode(optionsBA);
    }
}

void IQSMPPAbstractPDU::encodeOptionalParameters(QDataStream &stream)
{
    int optParameterLength = stream.device()->size() - stream.device()->pos();
    if (optParameterLength == 0)
        return;
    char *buff = new char[optParameterLength];
    stream.readRawData(buff, optParameterLength);
    QByteArray optParameterBA (buff, optParameterLength);
    delete[] buff;
    encodeOptionalParameters(optParameterBA);
}
