#include "iqsmppenquirelinkpdu.h"

IQSMPPEnquireLinkPDU::IQSMPPEnquireLinkPDU(const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber)
{
}

IQSMPP::Commands IQSMPPEnquireLinkPDU::command() const
{
    return IQSMPP::EnquireLink;
}

QByteArray IQSMPPEnquireLinkPDU::encodeBody() const
{
    return QByteArray();
}

IQSMPP::CommandStatus IQSMPPEnquireLinkPDU::decode(const QByteArray &pdu)
{
    quint32 length;
    IQSMPP::Commands command;
    IQSMPP::CommandStatus commandStatus;
    quint32 sequenceNumber;

    if (!decodeHeader(pdu, &length, &command, &commandStatus, &sequenceNumber))
        return IQSMPP::ESME_RSYSERR;

    if (command != IQSMPP::EnquireLink)
        return IQSMPP::ESME_RSYSERR;

    if (static_cast<int>(length) != pdu.length())
        return IQSMPP::ESME_RINVMSGLEN;

    setCommandStatus(commandStatus);
    setSequenceNumber(sequenceNumber);

    return IQSMPP::ESME_ROK;
}
