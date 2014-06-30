#ifndef IQDATABASETRAILER_H
#define IQDATABASETRAILER_H

#include <QObject>

class IQDatabaseTrailer : public QObject
{
    Q_OBJECT
public:
    explicit IQDatabaseTrailer(QObject *parent = 0);

    QString trailer() const;

    inline bool running() const {return _running;}
    
public slots:
    void start();
    void stop();

    void createTrailerForPhone(const qint64 phone);

signals:
    void stateChanged(const bool running);

    void trailerForPhoneCreated(const qint64 phone, const QString &trailer);

private:
    bool _running;
    
};

#endif // IQDATABASETRAILER_H
