#ifndef IQSMSSENDER_H
#define IQSMSSENDER_H

#include <QObject>
#include <QQueue>
#include "iqsmpp/iqsmppesme.h"
#include "iqsmpp/iqsmppsubmitsmpdu.h"


class IQSMSSender : public QObject
{
    Q_OBJECT
public:
    explicit IQSMSSender(QObject *parent = 0);

    bool start();

    void stop();
    
public slots:
    void sendMessage(const qint64 phone, const QString &message);

    void sendReservationNotify(const qint64 phoneNumber,
                               const qint64 reservationNumber,
                               const QDateTime &reservationDateTime,
                               const QDateTime &performanceDateTime,
                               const QString &reservationWorld);

private slots:
    void sendNextSumbitSM();

    bool reconect();

private:
    bool hasRussianSumbols(const QString &string) const;

    IQSMPPESME *_esme;
    quint8 _sarMsgRefNum;

    QTimer *_senderTimer;
    QTimer *_reconectTimer;
    QQueue<IQSMPPSubmitSMPDU> _submitPDUStack;
};

#endif // IQSMSSENDER_H
