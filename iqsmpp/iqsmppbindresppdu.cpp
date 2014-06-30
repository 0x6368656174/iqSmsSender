#include "iqsmppbindresppdu.h"
#include <QDataStream>

IQSMPPBindRespPDU::IQSMPPBindRespPDU(const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber),
    _bindRespOptions(IQSMPPBindRespOptions())
{
}

IQSMPPBindRespPDU::IQSMPPBindRespPDU(const IQSMPPBindRespOptions &options, const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber),
    _bindRespOptions(options)
{
}

IQSMPPBindRespOptions* IQSMPPBindRespPDU::options()
{
    return &_bindRespOptions;
}

IQSMPP::Commands IQSMPPBindRespPDU::command() const
{
    switch (_bindRespOptions.bindType())
    {
    case IQSMPP::Transmitter:
        return IQSMPP::BindTransmitterResp;
    case IQSMPP::Receiver:
        return IQSMPP::BindReceiverResp;
    case IQSMPP::Transceiver:
        return IQSMPP::BindTransceiverResp;
    }
    return IQSMPP::BindTransmitter;
}

QByteArray IQSMPPBindRespPDU::encodeBody() const
{
    QByteArray result;
    QDataStream stream (&result, QIODevice::WriteOnly);

    const char * systemId = _bindRespOptions.systemId().toLatin1().constData();
    int systemIdL = _bindRespOptions.systemId().length();
    stream.writeRawData(systemId, systemIdL);
    stream << (quint8)0x00;

    return result;
}

IQSMPP::CommandStatus IQSMPPBindRespPDU::decode(const QByteArray &pdu)
{
    quint32 length;
    IQSMPP::Commands command;
    IQSMPP::CommandStatus commandStatus;
    quint32 sequenceNumber;

    if (!decodeHeader(pdu, &length, &command, &commandStatus, &sequenceNumber))
        return IQSMPP::ESME_RSYSERR;

    if (command != IQSMPP::BindReceiverResp && command != IQSMPP::BindTransceiverResp && command != IQSMPP::BindTransmitterResp)
        return IQSMPP::ESME_RSYSERR;

    if (static_cast<int>(length) != pdu.length())
        return IQSMPP::ESME_RINVMSGLEN;

    setCommandStatus(commandStatus);
    setSequenceNumber(sequenceNumber);

    switch (command)
    {
    case IQSMPP::BindReceiverResp:
        _bindRespOptions.setBindType(IQSMPP::Receiver);
        break;
    case IQSMPP::BindTransceiverResp:
        _bindRespOptions.setBindType(IQSMPP::Transceiver);
        break;
    case IQSMPP::BindTransmitterResp:
        _bindRespOptions.setBindType(IQSMPP::Transmitter);
        break;
    default:
        break;
    }

    QByteArray pduCopy (pdu);
    QDataStream stream (&pduCopy, QIODevice::ReadOnly);
    stream.skipRawData(16);

    QString systemId = readCStringFromStream(stream);
    _bindRespOptions.setSystemId(systemId);

    return IQSMPP::ESME_ROK;
}
