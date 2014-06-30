#ifndef IQSMPPBINDPDU_H
#define IQSMPPBINDPDU_H

#include "iqsmppabstractpdu.h"
#include "iqsmppbindoptions.h"

class IQSMPPBindPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPBindPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    IQSMPPBindPDU(const IQSMPPBindOptions &options, const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPPBindOptions* options();

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;

private:
    IQSMPPBindOptions _bindOptions;

};

#endif // IQSMPPBINDPDU_H
