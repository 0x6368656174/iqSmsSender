#include "iqsmppbindrespoptions.h"

IQSMPPBindRespOptionsPrivate::IQSMPPBindRespOptionsPrivate():
    systemId(""),
    bindType(IQSMPP::Transmitter)
{
}

IQSMPPBindRespOptions::IQSMPPBindRespOptions():
    _data(new IQSMPPBindRespOptionsPrivate)
{
}

void IQSMPPBindRespOptions::setSystemId(const QString &systemId)
{
    if (systemId.length() > 15)
        return;
    _data->systemId = systemId;
}

void IQSMPPBindRespOptions::setBindType(const IQSMPP::BindType bindType)
{
    _data->bindType = bindType;
}
