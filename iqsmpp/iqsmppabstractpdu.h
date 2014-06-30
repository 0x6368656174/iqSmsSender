#ifndef IQSMPPABSTRACTPDU_H
#define IQSMPPABSTRACTPDU_H

#include <QByteArray>
#include <QAtomicInt>
#include <QSharedPointer>

#include "iqsmpp.h"
#include "iqsmppoptionalparameter.h"
#include "iqsmppabstractoptions.h"

class IQSMPPAbstractPDU
{
public:
    IQSMPPAbstractPDU(const quint32 sequenceNumber = IQSMPPAbstractPDU::AutoIncrement);

    virtual ~IQSMPPAbstractPDU();

    static const quint32 AutoIncrement = 0;

    static bool decodeHeader(const QByteArray &pdu,
                             quint32 *length,
                             IQSMPP::Commands *command,
                             IQSMPP::CommandStatus *commandStatus,
                             quint32 *sequenceNumber);

    QByteArray encode() const;

    virtual IQSMPP::CommandStatus decode(const QByteArray &pdu) = 0;

    virtual IQSMPP::Commands command() const = 0;

    virtual IQSMPPAbstractOptions * options() {return NULL;}

    void setSequenceNumber(const quint32 number);

    void setCommandStatus(const IQSMPP::CommandStatus status);

    template<class T>
    QSharedPointer<IQSMPPOptionalParameter<T> > optionalParameter(const IQSMPP::OptionalParameters tag)
    {
        if (!_optionalParametersWasSets)
        {
            _optionalParameters = optionalParameters();
            _optionalParametersWasSets = true;
        }
        if (_optionalParameters.contains(tag))
            return qSharedPointerDynamicCast<IQSMPPOptionalParameter<T> >(_optionalParameters[tag]);

        return QSharedPointer<IQSMPPOptionalParameter<T> >();
    }

    inline quint32 sequenceNumber() const {return _sequenceNumber;}

    inline IQSMPP::CommandStatus commandStatus() const {return _commandStatus;}

protected:
    virtual QByteArray encodeBody() const = 0;

    virtual QHash<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter> > optionalParameters() const {return QHash<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter > >();}

    QString readCStringFromStream(QDataStream &stream) const;

    void encodeOptionalParameters(QByteArray &optionalParametersBA);

    void encodeOptionalParameters(QDataStream &stream);

private:
    static QAtomicInt _autoSequenceNumber;

    quint32 _sequenceNumber;
    IQSMPP::CommandStatus _commandStatus;
    QHash<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter> > _optionalParameters;
    bool _optionalParametersWasSets;
};

#endif // IQSMPPABSTRACTPDU_H
