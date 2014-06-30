#include "iqwebaccesserver.h"
#include <QTcpSocket>
#include <QTextStream>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include "iqsettingmanager.h"
#include "iqhistoryloger.h"


IQWebAccesServer::IQWebAccesServer(QObject *parent) :
    QObject(parent),
    _server(new QTcpServer(this))
{
    connect(_server, SIGNAL(newConnection()), this, SLOT(readGetRequest()));
}

void IQWebAccesServer::start()
{
    if(!_server->listen(QHostAddress::Any, IQSettingManager::webAccesPort()))
    {
        IQHistoryLoger::write(tr("Can not listen port %0").arg(IQSettingManager::webAccesPort()), true);
        return;
    }    
    IQHistoryLoger::write(tr("Start listen web comand on port %0").arg(IQSettingManager::webAccesPort()));
    emit stateChanged(running());
}

void IQWebAccesServer::stop()
{
    IQHistoryLoger::write(tr("Stop listen web comand on port %0").arg(IQSettingManager::webAccesPort()));
    if (_server->isListening())
        _server->close();
    emit stateChanged(running());
}

void IQWebAccesServer::readGetRequest()
{
    QTcpSocket *socket = _server->nextPendingConnection();
    if(!socket->waitForReadyRead())
    {
        IQHistoryLoger::write(tr("Read timeout is over"), true);
        socket->close();
        socket->deleteLater();
        return;
    }
    QString request = socket->readAll();

    QStringList requestStrings = request.split("\n");

    if (requestStrings.size() < 1)
    {
        IQHistoryLoger::write(tr("Empy request"), true);
        socket->write("HTTP/1.1 400 Bad request (request empy)\n"
                      "\n");
        socket->waitForBytesWritten();
        socket->close();
        socket->deleteLater();
        return;
    }

    QRegExp re ("GET /sendTrailer\\?(phone=(\\d{11})|secretKey=(\\w+))&(phone=(\\d{11})|secretKey=(\\w+))", Qt::CaseInsensitive);
    if(re.indexIn(requestStrings[0]) == -1)
    {
        IQHistoryLoger::write(tr("Wrong request \"%0\"").arg(request), true);
        socket->write("HTTP/1.1 400 Bad request (required parameters not found)\n"
                      "\n");
        socket->waitForBytesWritten();
        socket->close();
        socket->deleteLater();
        return;
    }

    QStringList getParceList = re.capturedTexts();

    QString phoneStr = !getParceList[2].isEmpty()?getParceList[2]:getParceList[5];
    QString secretStr = !getParceList[3].isEmpty()?getParceList[3]:getParceList[6];

    QString appSecretKey = IQSettingManager::webAccesSecretKey();

    if (secretStr != appSecretKey)
    {
        IQHistoryLoger::write(tr("Wrong secret key \"%0\"").arg(secretStr), true);
        socket->write("HTTP/1.1 401 Unauthorized (wrong secret key)\n"
                      "\n");
        socket->waitForBytesWritten();
        socket->close();
        socket->deleteLater();
        return;
    }

    emit newRequestToSendAffishaSMS(phoneStr.toLongLong());

    socket->write("HTTP/1.1 200 OK\n"
                  "\n");
    socket->waitForBytesWritten();
    socket->close();
    socket->deleteLater();
}
