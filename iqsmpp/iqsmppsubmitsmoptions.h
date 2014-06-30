#ifndef IQSMPPSUBMITSMOPTIONS_H
#define IQSMPPSUBMITSMOPTIONS_H

#include "iqsmpp.h"
#include "iqsmppabstractoptions.h"
#include <QSharedData>

class IQSMPPSubmitSMOptionsPrivate: public QSharedData
{
public:
    IQSMPPSubmitSMOptionsPrivate();

    QString serviceType;
    IQSMPP::TON sourceAddrTON;
    IQSMPP::NPI sourceAddrNPI;
    QString sourceAddr;
    IQSMPP::TON destAddrTON;
    IQSMPP::NPI destAddrNPI;
    QString destinationAddr;
    IQSMPP::PriorytiLevel priorytiLevel;
    bool replaceIfPresent;
    IQSMPP::DataCodingTypes dataCoding;
    quint8 smDefaultMsgId;
    QByteArray shortMessage;
    quint8 esmClass;
};

class IQSMPPSubmitSMOptions: public IQSMPPAbstractOptions
{
public:
    IQSMPPSubmitSMOptions();

    inline QString serviceType() const {return _data->serviceType;}

    inline IQSMPP::TON sourceAddresTON() const {return _data->sourceAddrTON;}

    inline IQSMPP::NPI sourceAddresNPI() const {return _data->sourceAddrNPI;}

    inline QString sourceAddres() const {return _data->sourceAddr;}

    inline IQSMPP::TON destinationAddresTON() const {return _data->destAddrTON;}

    inline IQSMPP::NPI destinationAddresNPI() const {return _data->destAddrNPI;}

    inline QString destinationAddres() const {return _data->destinationAddr;}

    inline IQSMPP::PriorytiLevel priorytiLevel() const {return _data->priorytiLevel;}

    inline bool replaceIfPresent() const {return _data->replaceIfPresent;}

    inline IQSMPP::DataCodingTypes dataCodingType() const {return _data->dataCoding;}

    inline quint8 smDefaultMessageId() const {return _data->smDefaultMsgId;}

    inline QByteArray shortMessage() const {return _data->shortMessage;}

    inline quint8 esmClass() const {return _data->esmClass;}

    void setServiceType(const QString &serviceType);

    void setSourceAddresTON(const IQSMPP::TON ton);

    void setSourceAddresNPI(const IQSMPP::NPI npi);

    void setSourceAddres(const QString &sourceAddres);

    void setDestinationAddresTON(const IQSMPP::TON ton);

    void setDestinationAddresNPI(const IQSMPP::NPI npi);

    void setDestinationAddres(const QString &destinationAddres);

    void setPriorytiLevel(const IQSMPP::PriorytiLevel level);

    void setReplaceIfPresent(const bool replaceIfPresent);

    void setDataCodingType(const IQSMPP::DataCodingTypes type);

    void setSMDefaultMessageId(const quint8 id);

    void setShortMessage(const QByteArray &message);

    void setESMClass(const quint8 esmClass);

private:
    QSharedDataPointer<IQSMPPSubmitSMOptionsPrivate> _data;
};

#endif // IQSMPPSUBMITSMOPTIONS_H
