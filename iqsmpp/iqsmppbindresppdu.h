#ifndef IQSMPPBINDRESPPDU_H
#define IQSMPPBINDRESPPDU_H

#include "iqsmppabstractpdu.h"
#include "iqsmppbindrespoptions.h"

class IQSMPPBindRespPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPBindRespPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    IQSMPPBindRespPDU(const IQSMPPBindRespOptions &options, const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPPBindRespOptions* options();

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;

private:
    IQSMPPBindRespOptions _bindRespOptions;
};

#endif // IQSMPPBINDRESPPDU_H
