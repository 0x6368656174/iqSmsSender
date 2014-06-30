#ifndef IQDATABASESEARCHER_H
#define IQDATABASESEARCHER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

class IQDataBaseSearcher : public QObject
{
    Q_OBJECT
public:
    explicit IQDataBaseSearcher(QObject *parent = 0);

    inline bool running() const {return _running;}
    
public slots:
    void start();
    void stop();

signals:
    void stateChanged(const bool running);

    void newReservationFound(const qint64 phoneNumber,
                             const qint64 reservationNumber,
                             const QDateTime &reservationDateTime,
                             const QDateTime &performanseDateTime,
                             const QString &reservationWorld);

private slots:
    void searchNewReservation();

private:
    QTimer *_timer;
    quint32 _lastReservationId;
    bool _running;

    qint64 parsePhone(const QString &phone) const;
};

#endif // IQDATABASESEARCHER_H
