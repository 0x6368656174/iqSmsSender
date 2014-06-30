#ifndef IQSMPPBINDRESPOPTIONS_H
#define IQSMPPBINDRESPOPTIONS_H

#include "iqsmpp.h"
#include "iqsmppabstractoptions.h"
#include <QSharedData>

class IQSMPPBindRespOptionsPrivate : public QSharedData
{
public:
    IQSMPPBindRespOptionsPrivate();

    QString systemId;
    IQSMPP::BindType bindType;
};

class IQSMPPBindRespOptions: public IQSMPPAbstractOptions
{
public:
    IQSMPPBindRespOptions();

    inline QString systemId() const {return _data->systemId;}

    inline IQSMPP::BindType bindType() const {return _data->bindType;}

    void setSystemId(const QString &systemId);

    void setBindType(const IQSMPP::BindType type);

private:
    QSharedDataPointer<IQSMPPBindRespOptionsPrivate> _data;
};

#endif // IQSMPPBINDRESPOPTIONS_H
