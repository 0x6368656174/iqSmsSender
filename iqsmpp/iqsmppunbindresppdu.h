#ifndef IQSMPPUNBINDRESPPDU_H
#define IQSMPPUNBINDRESPPDU_H

#include "iqsmppabstractpdu.h"

class IQSMPPUnbindRespPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPUnbindRespPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;
};

#endif // IQSMPPUNBINDRESPPDU_H
