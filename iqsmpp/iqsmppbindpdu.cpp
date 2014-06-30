#include "iqsmppbindpdu.h"
#include <QDataStream>

IQSMPPBindPDU::IQSMPPBindPDU(const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber),
    _bindOptions(IQSMPPBindOptions())
{
}

IQSMPPBindPDU::IQSMPPBindPDU(const IQSMPPBindOptions &options, const quint32 sequenceNumber):
    IQSMPPAbstractPDU(sequenceNumber),
    _bindOptions(options)
{
}

IQSMPPBindOptions* IQSMPPBindPDU::options()
{
    return &_bindOptions;
}

IQSMPP::Commands IQSMPPBindPDU::command() const
{
    switch (_bindOptions.bindType())
    {
    case IQSMPP::Transmitter:
        return IQSMPP::BindTransmitter;
    case IQSMPP::Receiver:
        return IQSMPP::BindReceiver;
    case IQSMPP::Transceiver:
        return IQSMPP::BindTransceiver;
    }
    return IQSMPP::BindTransmitter;
}

QByteArray IQSMPPBindPDU::encodeBody() const
{
    QByteArray result;
    QDataStream stream (&result, QIODevice::WriteOnly);

    const char * systemId = _bindOptions.systemId().toLatin1().constData();
    int systemIdL = _bindOptions.systemId().length();
    stream.writeRawData(systemId, systemIdL);
    stream << (quint8)0x00;

    const char * password = _bindOptions.password().toLatin1().constData();
    int passwordL = _bindOptions.password().length();
    stream.writeRawData(password, passwordL);
    stream << (quint8)0x00;

    const char * systemType = _bindOptions.systemType().toLatin1().constData();
    int systemTypeL = _bindOptions.systemType().length();
    stream.writeRawData(systemType, systemTypeL);
    stream << (quint8)0x00;

    stream << (quint8)_bindOptions.interfaceVersion();
    stream << (quint8)_bindOptions.addresTON();
    stream << (quint8)_bindOptions.addresNPI();

    const char * addresRange = _bindOptions.addresRange().toLatin1().constData();
    int addresRangeL = _bindOptions.addresRange().length();
    stream.writeRawData(addresRange, addresRangeL);
    stream << (quint8)0x00;

    return result;
}

IQSMPP::CommandStatus IQSMPPBindPDU::decode(const QByteArray &pdu)
{
    quint32 length;
    IQSMPP::Commands command;
    IQSMPP::CommandStatus commandStatus;
    quint32 sequenceNumber;

    if (!decodeHeader(pdu, &length, &command, &commandStatus, &sequenceNumber))
        return IQSMPP::ESME_RSYSERR;

    if (command != IQSMPP::BindReceiver && command != IQSMPP::BindTransceiver && command != IQSMPP::BindTransmitter)
        return IQSMPP::ESME_RSYSERR;

    if (static_cast<int>(length) != pdu.length())
        return IQSMPP::ESME_RINVMSGLEN;

    setCommandStatus(commandStatus);
    setSequenceNumber(sequenceNumber);

    switch (command)
    {
    case IQSMPP::BindReceiver:
        _bindOptions.setBindType(IQSMPP::Receiver);
        break;
    case IQSMPP::BindTransceiver:
        _bindOptions.setBindType(IQSMPP::Transceiver);
        break;
    case IQSMPP::BindTransmitter:
        _bindOptions.setBindType(IQSMPP::Transmitter);
        break;
    default:
        break;
    }

    QByteArray pduCopy (pdu);
    QDataStream stream (&pduCopy, QIODevice::ReadOnly);
    stream.skipRawData(16);

    QString systemId = readCStringFromStream(stream);
    _bindOptions.setSystemId(systemId);

    QString password = readCStringFromStream(stream);
    _bindOptions.setPassword(password);

    QString systemType = readCStringFromStream(stream);
    _bindOptions.setSystemType(systemType);

    quint8 interfaseVersion;
    stream >> interfaseVersion;
    _bindOptions.setInterfaceVersion(static_cast<IQSMPP::InterfaceVersions>(interfaseVersion));

    quint8 addresTON;
    stream >> addresTON;
    _bindOptions.setAddresTON(static_cast<IQSMPP::TON>(addresTON));

    quint8 addresNPI;
    stream >> addresNPI;
    _bindOptions.setAddresNPI(static_cast<IQSMPP::NPI>(addresNPI));

    QString addresRange = readCStringFromStream(stream);
    _bindOptions.setAddresRange(addresRange);

    //Читаем опциональные параметры
    encodeOptionalParameters(stream);

    return IQSMPP::ESME_ROK;
}
