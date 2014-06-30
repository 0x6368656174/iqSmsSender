#include "iqsmppgenericnackpdu.h"

IQSMPPGenericNackPDU::IQSMPPGenericNackPDU(const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber)
{
}

IQSMPP::Commands IQSMPPGenericNackPDU::command() const
{
    return IQSMPP::GenericNack;
}

QByteArray IQSMPPGenericNackPDU::encodeBody() const
{
    return QByteArray();
}

IQSMPP::CommandStatus IQSMPPGenericNackPDU::decode(const QByteArray &pdu)
{
    quint32 length;
    IQSMPP::Commands headerCommand;
    IQSMPP::CommandStatus commandStatus;
    quint32 sequenceNumber;

    if (!decodeHeader(pdu, &length, &headerCommand, &commandStatus, &sequenceNumber))
        return IQSMPP::ESME_RSYSERR;

    if (headerCommand != command())
        return IQSMPP::ESME_RSYSERR;

    if (static_cast<int>(length) != pdu.length())
        return IQSMPP::ESME_RINVMSGLEN;

    setCommandStatus(commandStatus);
    setSequenceNumber(sequenceNumber);

    return IQSMPP::ESME_ROK;
}
