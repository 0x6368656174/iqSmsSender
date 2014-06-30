#include "smssendermainwindow.h"
#include "ui_smssendermainwindow.h"
#include "iqsettingmanager.h"
#include "iqhistoryloger.h"
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

SMSSenderMainWindow::SMSSenderMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SMSSenderMainWindow),
    _databaseSearcher(new IQDataBaseSearcher(this)),
    _smsSender(new IQSMSSender(this)),
    _webAccesServer(new IQWebAccesServer(this)),
    _databaseTrailer(new IQDatabaseTrailer(this)),
    _staticFileTrailer(new IQStaticFileTrailer(this))
{
    ui->setupUi(this);

    IQHistoryLoger::setDestinationTableWidget(ui->historyTableWidget);

    connect(ui->cinemaNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsCinemaName(QString)));

    connect(ui->reservationNotificationEnableCheckBox, SIGNAL(toggled(bool)), ui->updateIntervalLabel, SLOT(setEnabled(bool)));
    connect(ui->reservationNotificationEnableCheckBox, SIGNAL(toggled(bool)), ui->updateIntervalSpinBox, SLOT(setEnabled(bool)));
    connect(ui->reservationNotificationEnableCheckBox, SIGNAL(toggled(bool)), ui->reservationNorificationSenderTextGroupBox, SLOT(setEnabled(bool)));
    connect(ui->reservationNotificationEnableCheckBox, SIGNAL(toggled(bool)), this, SLOT(saveToSettingsEnableNotificationOfReservation(bool)));
    connect(ui->updateIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(saveToSettingsUpdateInterval(int)));

    connect(ui->subscribeMoviePosterEnableCheckBox, SIGNAL(toggled(bool)), ui->moviePosterFileLabel, SLOT(setEnabled(bool)));
    connect(ui->subscribeMoviePosterEnableCheckBox, SIGNAL(toggled(bool)), ui->moviePosterFileLineEdit, SLOT(setEnabled(bool)));
    connect(ui->subscribeMoviePosterEnableCheckBox, SIGNAL(toggled(bool)), ui->moviePosterFileBrowsePushButton, SLOT(setEnabled(bool)));
    connect(ui->subscribeMoviePosterEnableCheckBox, SIGNAL(toggled(bool)), ui->moviePosterSourceLabel, SLOT(setEnabled(bool)));
    connect(ui->subscribeMoviePosterEnableCheckBox, SIGNAL(toggled(bool)), ui->moviePosterSourceComboBox, SLOT(setEnabled(bool)));
    connect(ui->subscribeMoviePosterEnableCheckBox, SIGNAL(toggled(bool)), this, SLOT(saveToSettingsEnableSubscribleMovePoster(bool)));
    connect(ui->moviePosterSourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToSettingsSubscribleMovePosterSource(int)));
    connect(ui->moviePosterSourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onMoviePosterSourceCurrentIndexChanged(int)));
    connect(ui->moviePosterFileLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsSubscribleMovePosterFile(QString)));
    connect(ui->moviePosterFileBrowsePushButton, SIGNAL(clicked()), this, SLOT(browseSourceFile()));

    connect(ui->sourceNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsODBCDataSource(QString)));
    connect(ui->dataBaseNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsODBCDatabaseName(QString)));
    connect(ui->userNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsODBCUserName(QString)));
    connect(ui->passwordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsODBCPassword(QString)));

    connect(ui->smppHostNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsSMPPHostName(QString)));
    connect(ui->smppHostPortSpinBox, SIGNAL(valueChanged(int)), this, SLOT(saveToSettingsSMPPHostPort(int)));
    connect(ui->smppSystemIDLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsSMPPSystemId(QString)));
    connect(ui->smppPasswordLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsSMPPPassword(QString)));

    connect(ui->smppSourceAddresNPIComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToSettingsSMPPSourceNPI(int)));
    connect(ui->smppSourceAddresTONComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToSettingsSMPPSourceTON(int)));
    connect(ui->smppSourceAddresLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsSMPPSourceAddres(QString)));
    connect(ui->smppDestinationalAddresNPIComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToSettingsSMPPDestinationNPI(int)));
    connect(ui->smppDestinationalAddresTONComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveToSettingsSMPPDestinationTON(int)));
    connect(ui->smppSendEnqueireLinkIntervalSpinBox, SIGNAL(valueChanged(int)), this, SLOT(saveToSettingsSMPPEnqueireLinkSendInterval(int)));
    connect(ui->smppSMSPerSecondsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(saveToSettingsSMPPSMSPerSecond(int)));

    connect(ui->webAccesEnableCheckBox, SIGNAL(toggled(bool)), ui->webAccesPortLabel, SLOT(setEnabled(bool)));
    connect(ui->webAccesEnableCheckBox, SIGNAL(toggled(bool)), ui->webAccesPortSpinBox, SLOT(setEnabled(bool)));
    connect(ui->webAccesEnableCheckBox, SIGNAL(toggled(bool)), ui->webAccesSecretKeyLabel, SLOT(setEnabled(bool)));
    connect(ui->webAccesEnableCheckBox, SIGNAL(toggled(bool)), ui->webAccesSecretKeyLineEdit, SLOT(setEnabled(bool)));
    connect(ui->webAccesEnableCheckBox, SIGNAL(toggled(bool)), this, SLOT(saveToSettingsEnableWebAcces(bool)));
    connect(ui->webAccesPortSpinBox, SIGNAL(valueChanged(int)), this, SLOT(saveToSettingsWebAccesPort(int)));
    connect(ui->webAccesSecretKeyLineEdit, SIGNAL(textChanged(QString)), this, SLOT(saveToSettingsWebAccesSecretKey(QString)));

    connect(ui->senderTextPlainTextEdit, SIGNAL(textChanged()), this, SLOT(saveToSettingsSenderText()));

    connect(ui->startStopPushButton, SIGNAL(clicked()), this, SLOT(startStop()));
    connect(ui->clearHistoryPushButton, SIGNAL(clicked()), this, SLOT(clearHistory()));

    connect(ui->aboutQtLabel, SIGNAL(linkActivated(QString)), QApplication::instance(), SLOT(aboutQt()));
    connect(ui->aboutLabel, SIGNAL(linkActivated(QString)), this, SLOT(showAbout()));

    connect(_databaseSearcher, SIGNAL(stateChanged(bool)), this, SLOT(updateState()));
    connect(_databaseSearcher, SIGNAL(newReservationFound(qint64,qint64,QDateTime,QDateTime,QString)), _smsSender, SLOT(sendReservationNotify(qint64,qint64,QDateTime,QDateTime,QString)));

    connect(_webAccesServer, SIGNAL(newRequestToSendAffishaSMS(qint64)), _databaseTrailer, SLOT(createTrailerForPhone(qint64)));
    connect(_webAccesServer, SIGNAL(newRequestToSendAffishaSMS(qint64)), _staticFileTrailer, SLOT(createTrailerForPhone(qint64)));
    connect(_webAccesServer, SIGNAL(stateChanged(bool)), this, SLOT(updateState()));

    connect(_databaseTrailer, SIGNAL(trailerForPhoneCreated(qint64,QString)), _smsSender, SLOT(sendMessage(qint64,QString)));
    connect(_databaseTrailer, SIGNAL(stateChanged(bool)), this, SLOT(updateState()));

    connect(_staticFileTrailer, SIGNAL(trailerForPhoneCreated(qint64,QString)), _smsSender, SLOT(sendMessage(qint64,QString)));
    connect(_staticFileTrailer, SIGNAL(stateChanged(bool)), this, SLOT(updateState()));

    loadSettings();
}

SMSSenderMainWindow::~SMSSenderMainWindow()
{
    delete ui;
}

void SMSSenderMainWindow::onMoviePosterSourceCurrentIndexChanged(const int index)
{
    if (index > 0)
    {
        ui->moviePosterFileLabel->setEnabled(true);
        ui->moviePosterFileLineEdit->setEnabled(true);
        ui->moviePosterFileBrowsePushButton->setEnabled(true);
    }
    else
    {
        ui->moviePosterFileLabel->setEnabled(false);
        ui->moviePosterFileLineEdit->setEnabled(false);
        ui->moviePosterFileBrowsePushButton->setEnabled(false);
    }
}

void SMSSenderMainWindow::browseSourceFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Text (*.txt);;All files(*)"));
    if (!fileName.isEmpty())
        ui->moviePosterFileLineEdit->setText(fileName);
}

void SMSSenderMainWindow::start()
{
    if (!_databaseSearcher->running())
        startStop();
}

void SMSSenderMainWindow::saveToSettingsCinemaName(const QString &name)
{
    IQSettingManager::setCinemaName(name);
}

void SMSSenderMainWindow::saveToSettingsEnableWebAcces(const bool enable)
{
    IQSettingManager::setEnableWebAcces(enable);
}

void SMSSenderMainWindow::saveToSettingsWebAccesPort(const int port)
{
    IQSettingManager::setWebAccesPort(port);
}

void SMSSenderMainWindow::saveToSettingsWebAccesSecretKey(const QString &key)
{
    IQSettingManager::setWebAccesSecretKey(key);
}

void SMSSenderMainWindow::saveToSettingsUpdateInterval(const int interval)
{
    IQSettingManager::setUpdateInterval(interval);
}

void SMSSenderMainWindow::saveToSettingsODBCDataSource(const QString &odbcDataSource)
{
    IQSettingManager::setODBCDataSource(odbcDataSource);
}

void SMSSenderMainWindow::saveToSettingsODBCDatabaseName(const QString &name)
{
    IQSettingManager::setODBCDatabaseName(name);
}

void SMSSenderMainWindow::saveToSettingsODBCUserName(const QString &userName)
{
    IQSettingManager::setODBCUserName(userName);
}

void SMSSenderMainWindow::saveToSettingsODBCPassword(const QString &password)
{
    IQSettingManager::setODBCPassword(password);
}

void SMSSenderMainWindow::saveToSettingsSenderText()
{
    IQSettingManager::setSenderText(ui->senderTextPlainTextEdit->toPlainText());
}

void SMSSenderMainWindow::saveToSettingsSMPPDestinationNPI(const int npi)
{
    IQSettingManager::setSMPPDestinationNPI(static_cast<IQSMPP::NPI>(npi));
}

void SMSSenderMainWindow::saveToSettingsSMPPDestinationTON(const int ton)
{
    IQSettingManager::setSMPPDestinationTON(static_cast<IQSMPP::TON>(ton));
}

void SMSSenderMainWindow::saveToSettingsSMPPHostName(const QString &hostName)
{
    IQSettingManager::setSMPPHostName(hostName);
}

void SMSSenderMainWindow::saveToSettingsSMPPHostPort(const int port)
{
    IQSettingManager::setSMPPHostPort(port);
}

void SMSSenderMainWindow::saveToSettingsSMPPSourceAddres(const QString &addres)
{
    IQSettingManager::setSMPPSourceAddres(addres);
}

void SMSSenderMainWindow::saveToSettingsSMPPSourceNPI(const int npi)
{
    IQSettingManager::setSMPPSourceNPI(static_cast<IQSMPP::NPI>(npi));
}

void SMSSenderMainWindow::saveToSettingsSMPPSourceTON(const int ton)
{
    IQSettingManager::setSMPPSourceTON(static_cast<IQSMPP::TON>(ton));
}

void SMSSenderMainWindow::saveToSettingsSMPPSystemId(const QString &systemId)
{
    IQSettingManager::setSMPPSystemId(systemId);
}

void SMSSenderMainWindow::saveToSettingsSMPPPassword(const QString &password)
{
    IQSettingManager::setSMPPPassword(password);
}

void SMSSenderMainWindow::saveToSettingsSMPPEnqueireLinkSendInterval(const int intreval)
{
    IQSettingManager::setSMPPEqueireLinkSendInterval(intreval);
}

void SMSSenderMainWindow::saveToSettingsSMPPSMSPerSecond(const int count)
{
    IQSettingManager::setSMPPSMSPerSecond(count);
}

void SMSSenderMainWindow::saveToSettingsEnableNotificationOfReservation(const bool enable)
{
    IQSettingManager::setEnableNotificationOfReservation(enable);
}

void SMSSenderMainWindow::saveToSettingsEnableSubscribleMovePoster(const bool enable)
{
    IQSettingManager::setEnableSubscribeMovePoster(enable);
}

void SMSSenderMainWindow::saveToSettingsSubscribleMovePosterSource(const int index)
{
    IQSettingManager::setSubscribeMovePosterSource(index);
}

void SMSSenderMainWindow::saveToSettingsSubscribleMovePosterFile(const QString file)
{
    IQSettingManager::setSubscribeMovePosterFile(file);
}

void SMSSenderMainWindow::startStop()
{
    if (running())
    {
        _smsSender->stop();
        _databaseSearcher->stop();
        _databaseTrailer->stop();
        _staticFileTrailer->stop();
        _webAccesServer->stop();
    }
    else
    {
        if (_smsSender->start())
        {
            if (IQSettingManager::enableNotificationOfReservation())
                _databaseSearcher->start();
            if (IQSettingManager::enableSubscribeMovePoster() && IQSettingManager::subscribeMovePosterSource() == 0)
                _databaseTrailer->start();
            if (IQSettingManager::enableSubscribeMovePoster() && IQSettingManager::subscribeMovePosterSource() == 1)
                _staticFileTrailer->start();
            if (IQSettingManager::enableWebAcces())
                _webAccesServer->start();
        }
    }
}

bool SMSSenderMainWindow::running() const
{
    return _databaseSearcher->running() || _webAccesServer->running() || _databaseTrailer->running() || _staticFileTrailer->running();
}

void SMSSenderMainWindow::updateState()
{
    if (running())
    {
        ui->startStopPushButton->setText(tr("Stop"));
        ui->statusBar->showMessage(tr("Running..."));
        ui->generalConfigTab->setEnabled(false);
        ui->odbcConfigTab->setEnabled(false);
        ui->smppConfigTab->setEnabled(false);
        ui->webAccesConfigTab->setEnabled(false);
    }
    else
    {
        ui->startStopPushButton->setText(tr("Start"));
        ui->statusBar->showMessage(tr("Stoped..."));
        ui->generalConfigTab->setEnabled(true);
        ui->odbcConfigTab->setEnabled(true);
        ui->smppConfigTab->setEnabled(true);
        ui->webAccesConfigTab->setEnabled(true);
    }
}

void SMSSenderMainWindow::loadSettings()
{
    ui->cinemaNameLineEdit->setText(IQSettingManager::cinemaName());

    ui->reservationNotificationEnableCheckBox->setChecked(IQSettingManager::enableNotificationOfReservation());
    ui->updateIntervalSpinBox->setValue(IQSettingManager::updateInterval());

    ui->subscribeMoviePosterEnableCheckBox->setChecked(IQSettingManager::enableSubscribeMovePoster());
    ui->moviePosterSourceComboBox->setCurrentIndex(IQSettingManager::subscribeMovePosterSource());
    ui->moviePosterFileLineEdit->setText(IQSettingManager::subscribeMovePosterFile());

    ui->sourceNameLineEdit->setText(IQSettingManager::odbcDataSource());
    ui->dataBaseNameLineEdit->setText(IQSettingManager::odbcDatabaseName());
    ui->userNameLineEdit->setText(IQSettingManager::odbcUserName());
    ui->passwordLineEdit->setText(IQSettingManager::odbcPassword());

    ui->smppDestinationalAddresNPIComboBox->setCurrentIndex(static_cast<int>(IQSettingManager::smppDestinationNPI()));
    ui->smppDestinationalAddresTONComboBox->setCurrentIndex(static_cast<int>(IQSettingManager::smppDestinationTON()));
    ui->smppHostNameLineEdit->setText(IQSettingManager::smppHostName());
    ui->smppHostPortSpinBox->setValue(IQSettingManager::smppHostPort());
    ui->smppPasswordLineEdit->setText(IQSettingManager::smppPassword());
    ui->smppSourceAddresLineEdit->setText(IQSettingManager::smppSourceAddres());
    ui->smppSourceAddresNPIComboBox->setCurrentIndex(static_cast<int>(IQSettingManager::smppSourceNPI()));
    ui->smppSourceAddresTONComboBox->setCurrentIndex(static_cast<int>(IQSettingManager::smppSourceTON()));
    ui->smppSystemIDLineEdit->setText(IQSettingManager::smppSystemId());
    ui->smppSendEnqueireLinkIntervalSpinBox->setValue(IQSettingManager::smppEnqueireLinkSendInterval());
    ui->smppSMSPerSecondsSpinBox->setValue(IQSettingManager::smppSMSPerSecond());

    ui->senderTextPlainTextEdit->setPlainText(IQSettingManager::senderText());

    ui->webAccesEnableCheckBox->setChecked(IQSettingManager::enableWebAcces());
    ui->webAccesPortSpinBox->setValue(IQSettingManager::webAccesPort());
    ui->webAccesSecretKeyLineEdit->setText(IQSettingManager::webAccesSecretKey());
}

void SMSSenderMainWindow::clearHistory()
{
    IQHistoryLoger::setDestinationTableWidget(ui->historyTableWidget);
}

void  SMSSenderMainWindow::showAbout()
{
    QMessageBox::about(this,
                       tr("SMSSender about"),
                       tr("SMSSender used to send SMS messages with a reminder about "
                          "booking Best. Data on the bookings are taken from the software "
                          "package ticker-Soft. Used as a gateway cellular operator \"MegaFon\".")
                       );
}
