#include "iqsmppsubmitsmoptions.h"
#include <QSharedData>

IQSMPPSubmitSMOptionsPrivate::IQSMPPSubmitSMOptionsPrivate():
    serviceType(""),
    sourceAddrTON(IQSMPP::UnknownTON),
    sourceAddrNPI(IQSMPP::UnknownNPI),
    sourceAddr(""),
    destAddrTON(IQSMPP::UnknownTON),
    destAddrNPI(IQSMPP::UnknownNPI),
    destinationAddr(""),
    priorytiLevel(IQSMPP::Level0),
    replaceIfPresent(false),
    dataCoding(IQSMPP::SMSCDefaultAlphabet),
    smDefaultMsgId(0),
    shortMessage(QByteArray()),
    esmClass(0)
{
}


IQSMPPSubmitSMOptions::IQSMPPSubmitSMOptions():
    _data(new IQSMPPSubmitSMOptionsPrivate)
{
}

void IQSMPPSubmitSMOptions::setServiceType(const QString &serviceType)
{
    if (serviceType.length() > 5)
        return;
    _data->serviceType = serviceType;
}

void IQSMPPSubmitSMOptions::setSourceAddresTON(const IQSMPP::TON ton)
{
    _data->sourceAddrTON = ton;
}

void IQSMPPSubmitSMOptions::setSourceAddresNPI(const IQSMPP::NPI npi)
{
    _data->sourceAddrNPI = npi;
}

void IQSMPPSubmitSMOptions::setSourceAddres(const QString &sourceAddres)
{
    if (sourceAddres.length() > 20)
        return;
    _data->sourceAddr = sourceAddres;
}

void IQSMPPSubmitSMOptions::setDestinationAddresTON(const IQSMPP::TON ton)
{
    _data->destAddrTON = ton;
}

void IQSMPPSubmitSMOptions::setDestinationAddresNPI(const IQSMPP::NPI npi)
{
    _data->destAddrNPI = npi;
}

void IQSMPPSubmitSMOptions::setDestinationAddres(const QString &destinationAddres)
{
    if (destinationAddres.length() > 20)
        return;
    _data->destinationAddr = destinationAddres;
}

void IQSMPPSubmitSMOptions::setPriorytiLevel(const IQSMPP::PriorytiLevel level)
{
    _data->priorytiLevel = level;
}

void IQSMPPSubmitSMOptions::setReplaceIfPresent(const bool replaceIfPresent)
{
    _data->replaceIfPresent = replaceIfPresent;
}

void IQSMPPSubmitSMOptions::setDataCodingType(const IQSMPP::DataCodingTypes type)
{
    _data->dataCoding = type;
}

void IQSMPPSubmitSMOptions::setSMDefaultMessageId(const quint8 id)
{
    _data->smDefaultMsgId = id;
}

void IQSMPPSubmitSMOptions::setShortMessage(const QByteArray &message)
{
    if (message.length() > 254)
        return;
    _data->shortMessage = message;
}

void IQSMPPSubmitSMOptions::setESMClass(const quint8 esmClass)
{
    _data->esmClass = esmClass;
}
