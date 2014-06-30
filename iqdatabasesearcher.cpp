#include "iqdatabasesearcher.h"
#include "iqsettingmanager.h"
#include "iqhistoryloger.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QStringList>
#include <QDebug>

IQDataBaseSearcher::IQDataBaseSearcher(QObject *parent) :
    QObject(parent),
    _timer(new QTimer(this)),
    _running(false)
{
    connect(_timer, SIGNAL(timeout()), this, SLOT(searchNewReservation()));
}

void IQDataBaseSearcher::start()
{
    _timer->setInterval(IQSettingManager::updateInterval()*1000);
    _lastReservationId = IQSettingManager::lastReservationId();

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "IQDataBaseSearcher");
    db.setDatabaseName(IQSettingManager::odbcDataSource());
    db.setUserName(IQSettingManager::odbcUserName());
    db.setPassword(IQSettingManager::odbcPassword());

    if (!db.open())
    {
        IQHistoryLoger::write(db.lastError().text(), true);
    }

    _timer->start();
    _running = true;
    emit stateChanged(true);
}

void IQDataBaseSearcher::stop()
{
    QSqlDatabase db = QSqlDatabase::database("IQDataBaseSearcher");
    if (db.isValid() && db.isOpen())
    {
        db.close();
        QSqlDatabase::removeDatabase("IQDataBaseSearcher");
    }
    _timer->stop();
    _running = false;
    emit stateChanged(false);
}

void IQDataBaseSearcher::searchNewReservation()
{
    QSqlDatabase db = QSqlDatabase::database("IQDataBaseSearcher");

    QSqlQuery query (db);

    QString prepareStr ("SELECT DISTINCT "
                        "[Reservation].[ID], "
                        "[Reservation].[ReservationNumber], "
                        "[Reservation].[ReservationWord], "
                        "[Reservation].[ReservationDate], "
                        "[Reservation].[VoidDate], "
                        "[Reservation].[SecretQuery], "
                        "[Reservation].[SecretResponse], "
                        "[v_rSeanceTotals].[StartTime] "
                        "FROM "
                        "[Reservation], "
                        "[ReservedSeat], "
                        "[v_rSeanceTotals] "
                        "WHERE "
                        "[Reservation].[ID] = [ReservedSeat].[ReservationID] "
                        "AND [ReservedSeat].[PerformanceID] = [v_rSeanceTotals].[PerformanceID] "
                        "AND [Reservation].[ID] > :lastReservationId "
                        "AND [Reservation].[ReservationDate] > DATEADD(month, -1, GETDATE()) "
                        "AND [Reservation].[VoidDate] IS NOT NULL "
                        "ORDER BY [Reservation].[ReservationDate]");

    if (!query.prepare(prepareStr))
    {
        IQHistoryLoger::write(db.lastError().text(), true);
        return;
    }

    query.bindValue(":lastReservationId", _lastReservationId);

    if (!query.exec())
    {
        IQHistoryLoger::write(db.lastError().text(), true);
        return;
    }

    int reservationIdIndex = query.record().indexOf("ID");
    int reservationNumberIndex = query.record().indexOf("ReservationNumber");
    int reservationWorldIndex = query.record().indexOf("ReservationWord");
    int reservationDateIndex = query.record().indexOf("ReservationDate");
    //    int voidDateIndex = query.record().indexOf("VoidDate");
    int secretQueryIndex = query.record().indexOf("SecretQuery");
    int startTimeIndex = query.record().indexOf("StartTime");
    //    int sercretResponseIndex = query.record().indexOf("SecretResponse");

    while (query.next())
    {      
        _lastReservationId = query.value(reservationIdIndex).toUInt();

        QString secretQuery = query.value(secretQueryIndex).toString();
        //Проверим, чтоб в секретном вопросе хранился номер телефона
        qint64 phoneNumber = parsePhone(secretQuery);
        //Если там не номер телефона, то выходим
        if (phoneNumber < 0)
            continue;

        qint64 reservationNumber = query.value(reservationNumberIndex).toLongLong();
        QDateTime reservationDate = query.value(reservationDateIndex).toDateTime();
        QString reservationWorld = query.value(reservationWorldIndex).toString();
        QDateTime startTime = query.value(startTimeIndex).toDateTime();

        //Сообщим о том, что есть новая бронь
        emit newReservationFound(phoneNumber, reservationNumber, reservationDate, startTime, reservationWorld);
    }

    //Сохраним номер последней брони в настройках
    IQSettingManager::setLastReservationId(_lastReservationId);
}

qint64 IQDataBaseSearcher::parsePhone(const QString &phone) const
{
    //Сначала попробуем просто сконвертировать строку в цифры
    qint64 phoneInt = phone.toLongLong();
    //Если удалось и разрядность числа - 11 цифр, а так же телефон начинается с 8 или 7, то все хорошр
    if (phoneInt > 9999999999 && phoneInt < 100000000000
            && (phone.at(0) == '7' || phone.at(0) == '8'))
    {
        QString phoneStr = phone;
        //Заменим 8 на 7
        phoneStr.replace(0, 1, '7');
        return phoneStr.toLongLong();
    }

    //Иначе будем ситать, что телефон задон в каком-то хитром формате, но это все еще телефон

    //Для начала удалим все символы, которые не являются цифрами
    QStringList digets;
    digets << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9";

    QString digetsPhone;

    for (int i = 0; i < phone.length(); i++)
    {
        if (digets.contains(phone.at(i)))
            digetsPhone.append(phone.at(i));
    }

    //Должна остаться строка длиной 11 символов, иначе это не телефон
    if (digetsPhone.length() != 11)
        return -1;

    //Начинаться строка должна с 8 или 7, иначе это не телефон
    if (digetsPhone.at(0) != '7' && digetsPhone.at(0) != '8')
        return -1;

    //Заменим первую 8 на 7
    digetsPhone.replace(0, 1, '7');

    //Вернем телефон
    return digetsPhone.toLongLong();
}
