#ifndef IQHISTORYLOGER_H
#define IQHISTORYLOGER_H

#include <QObject>
#include <QtWidgets/QTableWidget>

class IQHistoryLoger
{
public:    
    static void write(const QString &string, const bool error = false);

    static void setDestinationTableWidget(QTableWidget *tableWidget);

private:
    explicit IQHistoryLoger();

    static QTableWidget *_tableWidget;
};

#endif // IQHISTORYLOGER_H
