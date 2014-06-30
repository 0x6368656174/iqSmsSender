#ifndef IQSETTINGMANAGER_H
#define IQSETTINGMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDateTime>
#include "iqsmpp/iqsmpp.h"

class IQSettingManager
{
public:
    static void setCinemaName(const QString &name) {setSettingParam("cinemaName", name);}

    static void setUpdateInterval(const int interval) {setSettingParam("updateInterval", interval);}

    static void setODBCDataSource(const QString &dataSource) {setSettingParam("odbcDataSource", dataSource);}

    static void setODBCUserName(const QString &userName) {setSettingParam("odbcUserName", userName);}

    static void setODBCPassword(const QString &password) {setSettingParam("odbcPassword", password);}

    static void setSenderText(const QString &senderText) {setSettingParam("senderText", senderText);}

    static void setLastReservationId(const quint32 id) {setSettingParam("lastReservationId", id);}

    static void setSMPPHostName(const QString &hostName) {setSettingParam("smppHostName", hostName);}

    static void setSMPPHostPort(const quint16 port) {setSettingParam("smppHostPort", port);}

    static void setSMPPSystemId(const QString &systemId) {setSettingParam("smppSystemId", systemId);}

    static void setSMPPPassword(const QString &password) {setSettingParam("smppPassword", password);}

    static void setSMPPSourceNPI(const IQSMPP::NPI npi) {setSettingParam("smppSourceNPI", static_cast<quint8>(npi));}

    static void setSMPPSourceTON(const IQSMPP::TON ton) {setSettingParam("smppSourceTON", static_cast<quint8>(ton));}

    static void setSMPPSourceAddres(const QString &addres) {setSettingParam("smppSourceAddres", addres);}

    static void setSMPPDestinationNPI(const IQSMPP::NPI npi) {setSettingParam("smppDestinationNPI", static_cast<quint8>(npi));}

    static void setSMPPDestinationTON(const IQSMPP::TON ton) {setSettingParam("smppDestinationTON", static_cast<quint8>(ton));}

    static void setSMPPEqueireLinkSendInterval(const int interval) {setSettingParam("smmpEnqueireLinkSendInterval", interval);}

    static void setSMPPSMSPerSecond(const int count) {setSettingParam("smppSMSPerSecond", count);}

    static void setODBCDatabaseName(const QString &name) {setSettingParam("databaseName", name);}

    static void setEnableNotificationOfReservation(const bool enable) {setSettingParam("enableNotificatonOfReservation", enable);}

    static void setEnableSubscribeMovePoster(const bool enable) {setSettingParam("enableSubscribeMoviePoster", enable);}

    static void setSubscribeMovePosterSource(const int sourceId) {setSettingParam("subscribeMovePosterSource", sourceId);}

    static void setSubscribeMovePosterFile(const QString &file) {setSettingParam("subscribeMovePosterFile", file);}

    static void setEnableWebAcces(const bool enable) {setSettingParam("enableWebAcces", enable);}

    static void setWebAccesPort(const quint16 port) {setSettingParam("webAccesPort", port);}

    static void setWebAccesSecretKey(const QString secretKey) {setSettingParam("webAccesSecretKey", secretKey);}

    static QString cinemaName() {return settingParam("cinemaName", "").toString();}

    static bool enableNotificationOfReservation() {return settingParam("enableNotificatonOfReservation", true).toBool();}

    static bool enableSubscribeMovePoster() {return settingParam("enableSubscribeMoviePoster", true).toBool();}

    static int subscribeMovePosterSource() {return settingParam("subscribeMovePosterSource", 0).toInt();}

    static QString subscribeMovePosterFile() {return settingParam("subscribeMovePosterFile", "").toString();}

    static int updateInterval() {return settingParam("updateInterval", 10).toInt();}

    static QString odbcDataSource() {return settingParam("odbcDataSource", "").toString();}

    static QString odbcUserName() {return settingParam("odbcUserName", "sa").toString();}

    static QString odbcPassword() {return settingParam("odbcPassword", "").toString();}

    static QString odbcDatabaseName() {return settingParam("databaseName", "KINO").toString();}

    static QString senderText() {return settingParam("senderText", "").toString();}

    static quint32 lastReservationId();

    static QString smppHostName() {return settingParam("smppHostName", "127.0.0.1").toString();}

    static quint16 smppHostPort() {return settingParam("smppHostPort", "2775").toUInt();}

    static QString smppSystemId() {return settingParam("smppSystemId", "").toString();}

    static QString smppPassword() {return settingParam("smppPassword", "").toString();}

    static IQSMPP::NPI smppSourceNPI() {return static_cast<IQSMPP::NPI>(settingParam("smppSourceNPI", 0).toUInt());}

    static IQSMPP::TON smppSourceTON() {return static_cast<IQSMPP::TON>(settingParam("smppSourceTON", 0).toUInt());}

    static QString smppSourceAddres() {return settingParam("smppSourceAddres", "").toString();}

    static IQSMPP::NPI smppDestinationNPI() {return static_cast<IQSMPP::NPI>(settingParam("smppDestinationNPI", 0).toUInt());}

    static IQSMPP::TON smppDestinationTON() {return static_cast<IQSMPP::TON>(settingParam("smppDestinationTON", 0).toUInt());}

    static int smppEnqueireLinkSendInterval() {return settingParam("smmpEnqueireLinkSendInterval", 5).toInt();}

    static int smppSMSPerSecond() {return settingParam("smppSMSPerSecond", 2).toInt();}

    static bool enableWebAcces() {return settingParam("enableWebAcces", true).toBool();}

    static quint16 webAccesPort() {return settingParam("webAccesPort", 7532).toInt();}

    static QString webAccesSecretKey() {return settingParam("webAccesSecretKey", "123456").toString();}


private:
    IQSettingManager();
    static void setSettingParam(const QString &key, const QVariant &value);
    static QVariant settingParam(const QString &key, const QVariant &defaultValue = QVariant());
};

#endif // IQSETTINGMANAGER_H
