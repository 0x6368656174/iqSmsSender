#ifndef IQSMPPOPTIONALPARAMETER_H
#define IQSMPPOPTIONALPARAMETER_H

#include <QObject>
#include "iqsmpp.h"
#include "iqsmppabstractoptionalparameter.h"

template< class T >
class IQSMPPOptionalParameter: public IQSMPPAbstractOptionalParameter
{
public:
    IQSMPPOptionalParameter(const T &defaultVaule);

//    IQSMPPOptionalParameter(const IQSMPPAbstractOptionalParameter &ref);

    inline T value() const {return _value;}

    inline bool cOctetString() const {return _cOctetString;}

    void setValue(const T &value);

    void setCOctetString(const bool cOtet);

    virtual QByteArray encode() const;

    virtual bool decode(const QByteArray &byteArray);
private:
    T _value;
    bool _cOctetString;
    bool _valueSets;
};
#endif // IQSMPPOPTIONALPARAMETER_H
