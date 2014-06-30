#include "iqstaticfiletrailer.h"
#include "iqsettingmanager.h"
#include "iqhistoryloger.h"
#include <QFile>

IQStaticFileTrailer::IQStaticFileTrailer(QObject *parent) :
    QObject(parent),
    _running(false)
{
}

void IQStaticFileTrailer::start()
{
    _running = true;
    emit stateChanged(true);
}

void IQStaticFileTrailer::stop()
{
    _running = false;
    emit stateChanged(false);
}

void IQStaticFileTrailer::createTrailerForPhone(const qint64 phone)
{
    if (!_running)
        return;
    if (phone < 0)
        return;
    emit trailerForPhoneCreated(phone, trailer());
}

QString IQStaticFileTrailer::trailer() const
{
    if (!QFile::exists(IQSettingManager::subscribeMovePosterFile()))
    {
        IQHistoryLoger::write(tr("File \"%0\" for trailer not found").arg(IQSettingManager::subscribeMovePosterFile()), true);
        return "";
    }
    QFile file (IQSettingManager::subscribeMovePosterFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        IQHistoryLoger::write(tr("File \"%0\" can not open for read").arg(IQSettingManager::subscribeMovePosterFile()), true);
        return "";
    }

    return file.readAll();
}
