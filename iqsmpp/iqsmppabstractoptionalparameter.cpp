#include "iqsmppabstractoptionalparameter.h"

IQSMPPAbstractOptionalParameter::IQSMPPAbstractOptionalParameter():
    _maxLength(0),
    _tag(IQSMPP::UndefinedOptionalParameter)
{
}

IQSMPPAbstractOptionalParameter::~IQSMPPAbstractOptionalParameter()
{
}

void IQSMPPAbstractOptionalParameter::setTag(const IQSMPP::OptionalParameters tag)
{
    _tag = tag;
}

void IQSMPPAbstractOptionalParameter::setMaxLength(const quint16 maxLength)
{
    _maxLength = maxLength;
}
