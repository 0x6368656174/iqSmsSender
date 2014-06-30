#ifndef IQSMPPESME_H
#define IQSMPPESME_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "iqsmppbindoptions.h"
#include "iqsmpp.h"
#include "iqsmppabstractpdu.h"


class IQSMPPESME : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        Connected,
        Disconnected
    };

    explicit IQSMPPESME(QObject *parent = 0);

    bool bindToSMSC();

    bool bindToSMSC(const QString &hostName, const quint16 port);

    bool unbindFromSMSC();

    IQSMPPBindOptions *bindOptions();

    bool sendPDU(const IQSMPPAbstractPDU &pdu);
    
    inline QString hostName() const {return _hostName;}

    inline quint16 hostPort() const {return _hostPort;}

    void setHostName(const QString &hostName);

    void setHostPort(const quint16 hostPort);

    inline State state() const {return _state;}

    inline QString smscSystemId() const {return _smscSystemId;}

    void setEnquireLinkCheckInterval(const int interval);

    int enquireLinkCheckInterval() const;

    void setBindWaitTime(const int time);

    int bindWaitTime() const {return _bindWaitTime;}

signals:
    void pduReserved(const IQSMPPAbstractPDU &pdu);
    void connected();
    void disconnected();

private slots:
    void readPDUFromSocket();
    void checkEnquire();
    
private:
    IQSMPPBindOptions _bindOptions;

    QString _hostName;
    quint16 _hostPort;
    QTcpSocket *_socket;
    State _state;
    QString _smscSystemId;

    int _bindWaitTime;

    quint32 _lastEnquireLinkSequenceNumerSend;
    quint32 _lastEnquireLinkSequenceNumerReseived;
    QTimer *_enquireLinkSenderTimer;

    void sendGenerickNack(const quint32 sequenceNumber, const IQSMPP::CommandStatus error);
    void sendEnquireLinkResp(const quint32 sequenceNumber);

};

#endif // IQSMPPESME_H
