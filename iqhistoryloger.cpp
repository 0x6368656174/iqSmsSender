#include "iqhistoryloger.h"
#include <QStringList>
#include <QDateTime>
#include <QDebug>
#include <QHeaderView>


QTableWidget * IQHistoryLoger::_tableWidget = NULL;

IQHistoryLoger::IQHistoryLoger()
{
}

void IQHistoryLoger::setDestinationTableWidget(QTableWidget *tableWidget)
{
    _tableWidget = tableWidget;
    _tableWidget->clear();
    _tableWidget->setRowCount(0);
    _tableWidget->setColumnCount(2);
    _tableWidget->setHorizontalHeaderLabels(QStringList() << QObject::tr("Time") << QObject::tr("Message"));
    _tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    _tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void IQHistoryLoger::write(const QString &string, const bool error)
{
    if (!_tableWidget)
        return;

    _tableWidget->insertRow(_tableWidget->rowCount());

    QTableWidgetItem *dateItem = new QTableWidgetItem(QDateTime::currentDateTime().toString());
    if (error)
        dateItem->setBackgroundColor(QColor("red"));
    QTableWidgetItem *messageItem = new QTableWidgetItem(string);
    if (error)
        messageItem->setBackgroundColor(QColor("red"));

    _tableWidget->setItem(_tableWidget->rowCount() - 1, 0, dateItem);
    _tableWidget->setItem(_tableWidget->rowCount() - 1, 1, messageItem);
}
