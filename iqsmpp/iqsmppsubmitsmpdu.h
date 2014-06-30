#ifndef IQSMPPSUBMITSMPDU_H
#define IQSMPPSUBMITSMPDU_H

#include "iqsmppabstractpdu.h"
#include "iqsmppsubmitsmoptions.h"

class IQSMPPSubmitSMPDU : public IQSMPPAbstractPDU
{
public:
    IQSMPPSubmitSMPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    IQSMPPSubmitSMPDU(const IQSMPPSubmitSMOptions &options, const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual IQSMPPSubmitSMOptions* options();

    virtual IQSMPP::Commands command() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu);

protected:
    virtual QByteArray encodeBody() const;

    virtual QHash<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter> > optionalParameters() const;

private:
    IQSMPPSubmitSMOptions _submitSMOptions;
};

#endif // IQSMPPSUBMITSMPDU_H
