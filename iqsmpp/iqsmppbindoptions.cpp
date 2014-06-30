#include "iqsmppbindoptions.h"

IQSMPPBindOptionsPrivate::IQSMPPBindOptionsPrivate():
    systemId(""),
    password(""),
    systemType(""),
    interfaceVersion(IQSMPP::Version34),
    addrTON(IQSMPP::UnknownTON),
    addrNPI(IQSMPP::UnknownNPI),
    addresRange(""),
    bindType(IQSMPP::Transmitter)
{
}



IQSMPPBindOptions::IQSMPPBindOptions():
    _data(new IQSMPPBindOptionsPrivate)
{
}

void IQSMPPBindOptions::setSystemId(const QString &systemId)
{
    if (systemId.length() > 15)
        return;
    _data->systemId = systemId;
}

void IQSMPPBindOptions::setPassword(const QString &password)
{
    if (password.length() > 8)
        return;
    _data->password = password;
}

void IQSMPPBindOptions::setSystemType(const QString &systemType)
{
    if (systemType.length() > 12)
        return;
    _data->systemType = systemType;
}

void IQSMPPBindOptions::setInterfaceVersion(const IQSMPP::InterfaceVersions version)
{
    _data->interfaceVersion = version;
}

void IQSMPPBindOptions::setAddresTON(const IQSMPP::TON addrTON)
{
    _data->addrTON = addrTON;
}

void IQSMPPBindOptions::setAddresNPI(const IQSMPP::NPI addrNPI)
{
    _data->addrNPI = addrNPI;
}

void IQSMPPBindOptions::setAddresRange(const QString &addresRange)
{
    if (addresRange.length() > 40)
        return;
    _data->addresRange = addresRange;
}

void IQSMPPBindOptions::setBindType(const IQSMPP::BindType bindType)
{
    _data->bindType = bindType;
}
