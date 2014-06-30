#ifndef IQSMPPBINDOPTIONS_H
#define IQSMPPBINDOPTIONS_H

#include "iqsmpp.h"
#include "iqsmppabstractoptions.h"
#include <QSharedData>

class IQSMPPBindOptionsPrivate : public QSharedData
{
public:
    IQSMPPBindOptionsPrivate();

    QString systemId;
    QString password;
    QString systemType;
    IQSMPP::InterfaceVersions interfaceVersion;
    IQSMPP::TON addrTON;
    IQSMPP::NPI addrNPI;
    QString addresRange;
    IQSMPP::BindType bindType;
};

class IQSMPPBindOptions: public IQSMPPAbstractOptions
{
public:
    IQSMPPBindOptions();

    inline QString systemId() const {return _data->systemId;}

    inline QString password() const {return _data->password;}

    inline QString systemType() const {return _data->systemType;}

    inline IQSMPP::InterfaceVersions interfaceVersion() const {return _data->interfaceVersion;}

    inline IQSMPP::TON addresTON() const {return _data->addrTON;}

    inline IQSMPP::NPI addresNPI() const {return _data->addrNPI;}

    inline QString addresRange() const {return _data->addresRange;}

    inline IQSMPP::BindType bindType() const {return _data->bindType;}

    void setSystemId(const QString &systemId);

    void setPassword(const QString &password);

    void setSystemType(const QString &systemType);

    void setInterfaceVersion(const IQSMPP::InterfaceVersions version);

    void setAddresTON(const IQSMPP::TON addrTON);

    void setAddresNPI(const IQSMPP::NPI addrNPI);

    void setAddresRange(const QString &addresRange);

    void setBindType(const IQSMPP::BindType bindType);

private:
    QSharedDataPointer<IQSMPPBindOptionsPrivate> _data;
};

#endif // IQSMPPBINDOPTIONS_H
