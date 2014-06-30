#ifndef SMSSENDERMAINWINDOW_H
#define SMSSENDERMAINWINDOW_H

#include <QMainWindow>
#include "iqdatabasesearcher.h"
#include "iqsmssender.h"
#include "iqwebaccesserver.h"
#include "iqdatabasetrailer.h"
#include "iqstaticfiletrailer.h"

namespace Ui {
class SMSSenderMainWindow;
}

class SMSSenderMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SMSSenderMainWindow(QWidget *parent = 0);

    void start();

    ~SMSSenderMainWindow();
    
private slots:
    void saveToSettingsCinemaName(const QString &name);
    void saveToSettingsEnableNotificationOfReservation(const bool enable);
    void saveToSettingsEnableSubscribleMovePoster(const bool enable);
    void saveToSettingsSubscribleMovePosterSource(const int index);
    void saveToSettingsSubscribleMovePosterFile(const QString file);
    void saveToSettingsUpdateInterval(const int interval);
    void saveToSettingsODBCDataSource(const QString &odbcDataSource);
    void saveToSettingsODBCDatabaseName(const QString &name);
    void saveToSettingsODBCUserName(const QString &userName);
    void saveToSettingsODBCPassword(const QString &password);
    void saveToSettingsSenderText();
    void saveToSettingsSMPPHostName(const QString &hostName);
    void saveToSettingsSMPPHostPort(const int port);
    void saveToSettingsSMPPSystemId(const QString &systemId);
    void saveToSettingsSMPPPassword(const QString &password);
    void saveToSettingsSMPPSourceNPI(const int npi);
    void saveToSettingsSMPPSourceTON(const int ton);
    void saveToSettingsSMPPSourceAddres(const QString &addres);
    void saveToSettingsSMPPDestinationNPI(const int npi);
    void saveToSettingsSMPPDestinationTON(const int ton);
    void saveToSettingsSMPPEnqueireLinkSendInterval(const int intreval);
    void saveToSettingsSMPPSMSPerSecond(const int count);
    void saveToSettingsEnableWebAcces(const bool enable);
    void saveToSettingsWebAccesPort(const int port);
    void saveToSettingsWebAccesSecretKey(const QString &key);

    void browseSourceFile();

    void startStop();
    void clearHistory();
    void showAbout();

    void updateState();

    void onMoviePosterSourceCurrentIndexChanged(const int index);

private:
    Ui::SMSSenderMainWindow *ui;
    IQDataBaseSearcher *_databaseSearcher;
    IQSMSSender *_smsSender;
    IQWebAccesServer *_webAccesServer;
    IQDatabaseTrailer *_databaseTrailer;
    IQStaticFileTrailer *_staticFileTrailer;

    void loadSettings();
    bool running() const;
};

#endif // SMSSENDERMAINWINDOW_H
