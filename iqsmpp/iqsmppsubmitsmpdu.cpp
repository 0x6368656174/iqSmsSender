#include "iqsmppsubmitsmpdu.h"
#include <QDataStream>

IQSMPPSubmitSMPDU::IQSMPPSubmitSMPDU(const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber),
    _submitSMOptions(IQSMPPSubmitSMOptions())
{
}

IQSMPPSubmitSMPDU::IQSMPPSubmitSMPDU(const IQSMPPSubmitSMOptions &options, const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber),
    _submitSMOptions(options)
{
}

QHash<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter> > IQSMPPSubmitSMPDU::optionalParameters() const
{
    QHash<IQSMPP::OptionalParameters, QSharedPointer<IQSMPPAbstractOptionalParameter> > result;

    QSharedPointer<IQSMPPOptionalParameter<quint16> > sarMsgRefNum (new IQSMPPOptionalParameter<quint16>(0));
    sarMsgRefNum->setMaxLength(2);
    sarMsgRefNum->setTag(IQSMPP::SarMsgRefNum);

    QSharedPointer<IQSMPPOptionalParameter<quint8> > sarTotalSegments (new IQSMPPOptionalParameter<quint8>(0));
    sarTotalSegments->setMaxLength(1);
    sarTotalSegments->setTag(IQSMPP::SarTotalSegments);

    QSharedPointer<IQSMPPOptionalParameter<quint8> > sarSegmentSegnum (new IQSMPPOptionalParameter<quint8>(0));
    sarSegmentSegnum->setMaxLength(1);
    sarSegmentSegnum->setTag(IQSMPP::SarSegmentSeqnum);

    result[IQSMPP::SarMsgRefNum] = sarMsgRefNum;
    result[IQSMPP::SarTotalSegments] = sarTotalSegments;
    result[IQSMPP::SarSegmentSeqnum] = sarSegmentSegnum;

    return result;
}

IQSMPPSubmitSMOptions* IQSMPPSubmitSMPDU::options()
{
    return &_submitSMOptions;
}

IQSMPP::Commands IQSMPPSubmitSMPDU::command() const
{
    return IQSMPP::SubmitSM;
}

QByteArray IQSMPPSubmitSMPDU::encodeBody() const
{
    QByteArray result;
    QDataStream stream (&result, QIODevice::WriteOnly);

    const char * serviceType = _submitSMOptions.serviceType().toLatin1().constData();
    int serviceTypeL = _submitSMOptions.serviceType().length();
    stream.writeRawData(serviceType, serviceTypeL);
    stream << (quint8)0x00;

    stream << (quint8)_submitSMOptions.sourceAddresTON();
    stream << (quint8)_submitSMOptions.sourceAddresNPI();

    const char * sourceAddres = _submitSMOptions.sourceAddres().toLatin1().constData();
    int sourceAddresL = _submitSMOptions.sourceAddres().length();
    stream.writeRawData(sourceAddres, sourceAddresL);
    stream << (quint8)0x00;

    stream << (quint8)_submitSMOptions.destinationAddresTON();
    stream << (quint8)_submitSMOptions.destinationAddresNPI();

    const char * destinationAddres = _submitSMOptions.destinationAddres().toLatin1().constData();
    int destinationAddresL = _submitSMOptions.destinationAddres().length();
    stream.writeRawData(destinationAddres, destinationAddresL);
    stream << (quint8)0x00;

    stream << (quint8)_submitSMOptions.esmClass(); //TODO esmClass
    stream << (quint8)0x00; //TODO protocolId
    stream << (quint8)_submitSMOptions.priorytiLevel();
    stream << (quint8)0x00; //TODO scheduleDeliveryTime
    stream << (quint8)0x00; //TODO validityPeriod
    stream << (quint8)0x00; //TODO registeredDelivery
    stream << (quint8)_submitSMOptions.replaceIfPresent();
    stream << (quint8)_submitSMOptions.dataCodingType();
    stream << (quint8)_submitSMOptions.smDefaultMessageId();
    stream << (quint8)_submitSMOptions.shortMessage().length();

    const char * shortMessage = _submitSMOptions.shortMessage().constData();
    int shorMessageL = _submitSMOptions.shortMessage().length();
    stream.writeRawData(shortMessage, shorMessageL);

    return result;
}

IQSMPP::CommandStatus IQSMPPSubmitSMPDU::decode(const QByteArray &pdu)
{
    return IQSMPP::ESME_ROK;
}
