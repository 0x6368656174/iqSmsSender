#ifndef IQWEBACCESSERVER_H
#define IQWEBACCESSERVER_H

#include <QObject>
#include <QTcpServer>

class IQWebAccesServer : public QObject
{
    Q_OBJECT
public:
    explicit IQWebAccesServer(QObject *parent = 0);
    
    inline bool running() const {return _server->isListening();}

    void start();

    void stop();

signals:
    void stateChanged(const bool runnging);

    void newRequestToSendAffishaSMS(const qint64 phone);

private slots:
    void readGetRequest();

private:
    bool _running;
    QTcpServer *_server;    
};

#endif // IQWEBACCESSERVER_H
