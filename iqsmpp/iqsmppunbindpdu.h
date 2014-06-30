#ifndef IQSMPPUNBINDPDU_H
#define IQSMPPUNBINDPDU_H

#include "iqsmppabstractpdu.h"

class IQSMPPUnbindPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPUnbindPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;
};

#endif // IQSMPPUNBINDPDU_H
