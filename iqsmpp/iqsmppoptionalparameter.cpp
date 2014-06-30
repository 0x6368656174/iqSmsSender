#include "iqsmppoptionalparameter.h"
#include <QDataStream>
#include <QDebug>

template < class T >
IQSMPPOptionalParameter<T>::IQSMPPOptionalParameter(const T &defaultVaule):
    _value(defaultVaule),
    _cOctetString(true),
    _valueSets(false)

{
}

//template < class T >
//IQSMPPOptionalParameter<T>::IQSMPPOptionalParameter(const IQSMPPAbstractOptionalParameter &ref)
//{
//    if(this == &ref)
//        return *this;

//    setTag(ref.tag());
//    setMaxLength(ref.maxLength());

//    IQSMPPOptionalParameter<T> castRef = static_cast<IQSMPPOptionalParameter<T> >(ref);
//    _value = castRef.value();
//    _cOctetString = castRef.cOctetString();
//    _valueSets = castRef._valueSets;

//    return *this;
//}

template < class T >
void IQSMPPOptionalParameter<T>::setValue(const T &value)
{
    _value = value;
    _valueSets = true;
}

template <>
void IQSMPPOptionalParameter<QString>::setValue(const QString &value)
{
    if (value.length() > maxLength())
        return;
    _value = value;
    _valueSets = true;
}

template < class T >
void IQSMPPOptionalParameter<T>::setCOctetString(const bool cOtet)
{
    _cOctetString = cOtet;
}

template <>
QByteArray IQSMPPOptionalParameter<QString>::encode() const
{
    QByteArray result;
    //Если не устанавливали значения для данной опции, то вернем пустой массив, т.к. данная опция
    //не используется
    if (!_valueSets)
        return result;

    if (tag() == IQSMPP::UndefinedOptionalParameter)
        return result;
    if (maxLength() == 0)
        return result;

    QDataStream stream (&result, QIODevice::WriteOnly);
    stream << (quint16) tag();
    stream << (quint16) 0;

    //Запишим данные
    //Если это строка
    QByteArray stringBA = _value.toLatin1();
    const char * stringShar = stringBA.constData();
    int stringLength = stringBA.length();
    stream.writeRawData(stringShar, stringLength);
    if (_cOctetString)
        stream << (quint8)0x00;

    //Сохраним размер
    stream.device()->seek(2);
    stream << (quint16) result.size() - 4;

    return result;
}

template < class T >
QByteArray IQSMPPOptionalParameter<T>::encode() const
{
    QByteArray result;
    //Если не устанавливали значения для данной опции, то вернем пустой массив, т.к. данная опция
    //не используется
    if (!_valueSets)
        return result;

    if (tag() == IQSMPP::UndefinedOptionalParameter)
        return result;
    if (maxLength() == 0)
        return result;

    QDataStream stream (&result, QIODevice::WriteOnly);
    stream << (quint16) tag();
    stream << (quint16) 0;

    //Запишим данные
    if (maxLength() == 1)
    {
        quint8 temp = static_cast<quint8>(_value);
        stream << temp;
    }
    else if (maxLength() == 2)
    {
        quint16 temp = static_cast<quint16>(_value);
        stream << temp;
    }
    else if (maxLength() == 4)
    {
        quint32 temp = static_cast<quint32>(_value);
        stream << temp;
    }

    //Сохраним размер
    stream.device()->seek(2);
    stream << (quint16) (result.size() - 4);

    return result;
}

template <>
bool IQSMPPOptionalParameter<QString>::decode(const QByteArray &byteArray)
{
    if (byteArray.size() < 5)
        return false;

    QByteArray copyBA(byteArray);
    QDataStream stream (&copyBA, QIODevice::ReadOnly);
    quint16 tagId;
    quint16 length;

    stream >> tagId;
    stream >> length;

    if (static_cast<IQSMPP::OptionalParameters>(tagId) != tag())
        return false;

    if (byteArray.size() + 4 != static_cast<int>(length))
        return false;

    QByteArray stringBA = copyBA.mid(4);

    if (!_cOctetString)
        stringBA.append((char)0x00);

    _value = QString::fromLatin1(stringBA);

    return true;

}

template < class T >
bool IQSMPPOptionalParameter<T>::decode(const QByteArray &byteArray)
{
    if (byteArray.size() < 5)
        return false;

    QByteArray copyBA(byteArray);
    QDataStream stream (&copyBA, QIODevice::ReadOnly);
    quint16 tagId;
    quint16 length;

    stream >> tagId;
    stream >> length;

    if (static_cast<IQSMPP::OptionalParameters>(tagId) != tag())
        return false;

    if (byteArray.size() + 4 != static_cast<int>(length))
        return false;

    quint64 temp;
    stream >> temp;
    _value = static_cast<T>(temp);

    return true;
}


template class IQSMPPOptionalParameter<quint8>;
template class IQSMPPOptionalParameter<quint16>;
template class IQSMPPOptionalParameter<quint32>;
