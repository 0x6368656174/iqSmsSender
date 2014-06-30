#include "iqdatabasetrailer.h"
#include "iqhistoryloger.h"
#include "iqsettingmanager.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QDate>
#include <QVariant>

IQDatabaseTrailer::IQDatabaseTrailer(QObject *parent) :
    QObject(parent),
    _running(false)
{
}

void IQDatabaseTrailer::start()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "IQDatabaseTrailer");
    db.setDatabaseName(IQSettingManager::odbcDataSource());
    db.setUserName(IQSettingManager::odbcUserName());
    db.setPassword(IQSettingManager::odbcPassword());

    if (!db.open())
    {
        IQHistoryLoger::write(db.lastError().text(), true);
    }

    _running = true;
    emit stateChanged(true);
}

void IQDatabaseTrailer::stop()
{
    QSqlDatabase db = QSqlDatabase::database("IQDatabaseTrailer");
    if (db.isValid() && db.isOpen())
    {
        db.close();
        QSqlDatabase::removeDatabase("IQDatabaseTrailer");
    }
    _running = false;
    emit stateChanged(false);
}

void IQDatabaseTrailer::createTrailerForPhone(const qint64 phone)
{
    if (!_running)
        return;
    if (phone < 0)
        return;
    emit trailerForPhoneCreated(phone, trailer());
}

QString IQDatabaseTrailer::trailer() const
{
    QString result;

    result += tr("%0 on %1")
            .arg(IQSettingManager::cinemaName())
            .arg(QDate::currentDate().toString("dd.MM.yyyy"));

    QSqlDatabase db = QSqlDatabase::database("IQDatabaseTrailer");

    QSqlQuery query (db);

    QString prepareStr ("SELECT [PerformanceID] AS ID "
                        ",[Name] AS NAME "
                        ",[HallName] AS HALLNAME "
                        ",[StartTime] AS STARTDATETIME "
                        ",[Duration] AS DURATION "
                        ",(SELECT [DisplayName] "
                        "FROM [AgeLimitation], "
                        "[Film], "
                        "[v_Repertoire] "
                        "WHERE [AgeLimitation].[ID] = [Film].[AgeLimitationID] "
                        "AND [Film].[ID] = [v_Repertoire].[FilmID] "
                        "AND [v_Repertoire].[PerformanceID] = [v_rSeanceTotals].[PerformanceID]) AS AGELIMIT "
                        ",[Is3D] AS IS3D "
                        "FROM [v_rSeanceTotals] "
                        "WHERE StartDate >= DATEADD(day,-1,GETDATE()) AND "
                        "StartDate <= DATEADD(day,+1,GETDATE()) "
                        "ORDER BY NAME, STARTDATETIME");

    if (!query.prepare(prepareStr))
    {
        IQHistoryLoger::write(db.lastError().text(), true);
        return tr("Sorry. Trailer not available.");
    }

    if (!query.exec())
    {
        IQHistoryLoger::write(db.lastError().text(), true);
        return tr("Sorry. Trailer not available.");
    }

//    int perfomanceIdIndex = query.record().indexOf("ID");
    int perfomanceNameIndex = query.record().indexOf("NAME");
//    int perfomanceHallNameIndex = query.record().indexOf("HALLNAME");
    int perfomanceStartDateTimeIndex = query.record().indexOf("STARTDATETIME");
//    int perfomanceDurationIndex = query.record().indexOf("DURATION");
    int perfomanceAgeLimitIndex = query.record().indexOf("AGELIMIT");
    int perfomanceIs3DIndex = query.record().indexOf("IS3D");

    QString filmString;
    QString name;

    int currentDay = QDateTime::currentDateTime().date().day();

    while (query.next())
    {
        QString currentRecordName = query.value(perfomanceNameIndex).toString();
        QDateTime currentRecordStartDateTime = query.value(perfomanceStartDateTimeIndex).toDateTime();
        QString currentRecordAgeLimit = query.value(perfomanceAgeLimitIndex).toString();
        bool currentRecordIs3D = query.value(perfomanceIs3DIndex).toBool();

        if (currentRecordStartDateTime.date().day() != currentDay)
            continue;

        if (currentRecordName != name)
        {
            filmString.remove(filmString.length() - 2, 2);
            result += filmString + "\n";
            name = currentRecordName;
            filmString = name;
            if (!currentRecordAgeLimit.isEmpty())
                filmString += ", " + currentRecordAgeLimit;
            if (currentRecordIs3D)
                filmString += ", 3D";
            filmString += ": ";
        }

        filmString += currentRecordStartDateTime.time().toString("hh:mm") + ", ";
    }

    result = result.trimmed();

    return result;
}
