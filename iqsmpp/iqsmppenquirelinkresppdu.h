#ifndef IQSMPPENQUIRELINKRESPPDU_H
#define IQSMPPENQUIRELINKRESPPDU_H

#include "iqsmppabstractpdu.h"

class IQSMPPEnquireLinkRespPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPEnquireLinkRespPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;
};

#endif // IQSMPPENQUIRELINKRESPPDU_H
