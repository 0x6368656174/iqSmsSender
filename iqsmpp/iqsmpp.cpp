#include "iqsmpp.h"

QString IQSMPP::bytes2String(const QByteArray &arcArray)
{
    QString result;
    result += "0x";

    for (int i = 0; i < arcArray.size(); ++i)
    {
        quint8 bit = arcArray.at(i);
        result += QString("%0 ").arg(QString::number(bit, 16), 2, '0');
    }
    return result;
}
