#ifndef IQSMPPENQUIRELINKPDU_H
#define IQSMPPENQUIRELINKPDU_H

#include "iqsmppabstractpdu.h"

class IQSMPPEnquireLinkPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPEnquireLinkPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;

};

#endif // IQSMPPENQUIRELINKPDU_H
