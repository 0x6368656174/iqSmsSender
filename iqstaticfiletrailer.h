#ifndef IQSTATICFILETRAILER_H
#define IQSTATICFILETRAILER_H

#include <QObject>

class IQStaticFileTrailer : public QObject
{
    Q_OBJECT
public:
    explicit IQStaticFileTrailer(QObject *parent = 0);
    
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

#endif // IQSTATICFILETRAILER_H
