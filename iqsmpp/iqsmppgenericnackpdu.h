#ifndef IQSMPPGENERICNACKPDU_H
#define IQSMPPGENERICNACKPDU_H

#include "iqsmppabstractpdu.h"

class IQSMPPGenericNackPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPGenericNackPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;

};

#endif // IQSMPPGENERICNACKPDU_H
