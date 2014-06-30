#ifndef IQSMPP_H
#define IQSMPP_H

#include <QObject>
#include <QTextStream>
#include <QByteArray>

/*!
 * \brief Класс, описывающий основыные перечисления
 */
class IQSMPP
{
public:
    /*!
     * \brief Версии SMPP-протоколов
     */
    enum InterfaceVersions
    {
        Version33, //!< Версия до 3.3 включительно
        Version34 //!< Версия 3.4
    };

    /*!
     * \brief Типы новмеров (TON - Type Of Number)
     */
    enum TON
    {
        UnknownTON, //!< Неизвестный
        International, //!< Международний
        NationalTON, //!< Государственный
        NetworkSpecific, //!< Опеределяемый сетью
        SubscriberNumber, //!< Номер абонента
        Alphanumeric, //!< Алфавитно-цифровой
        Abbreviated //!< Сокращенный
    };

    /*!
     * \brief Планы нумерации (NPI - Numeric Plan Indicator)
     */
    enum NPI
    {
        UnknownNPI, //!< Неизвестный
        ISDN, //!< ISDN (E163/E164)
        Data, //!< Data (X.121)
        Telex, //!< Telex (F.69)
        LandMobile, //!< Land Mobile (E.212)
        NationalNPI, //!< Национальный
        Private, //!< Частный
        ERMES, //!< ERMES
        Internet, //!< Интернет (IP)
        WAPClientId //!< Номер WAP (Его дожен определять WAP форум)
    };

    /*!
     * \brief Типы PDU Bind (Установления связи)
     */
    enum BindType
    {
        Transmitter, //!< На передачу
        Receiver, //!< На прием
        Transceiver //!< И на прием и на передачу
    };

    /*!
     * \brief Комманды
     */
    enum Commands
    {
        InvalidCommands = 0x00000000, //!< Не определенная команда
        GenericNack = 0x80000000, //!< Базовый ответ об ошибке
        BindReceiver = 0x00000001, //!< Установление связи на прием
        BindReceiverResp = 0x80000001, //!< Ответ об установлении связи на прием
        BindTransmitter = 0x00000002, //!< Установление связи на передачу
        BindTransmitterResp = 0x80000002, //!< Ответ об установлении связи на передачу
        QuerySM = 0x00000003, //!< Запрос о статусе предварительно предоставленного сообщения
        QuerySMResp = 0x80000003, //!< Ответ о запросе о статусе предварительно предоставленного сообщения
        SubmitSM = 0x00000004, //!< Отправить короткос сообщение
        SubmitSMResp = 0x80000004, //!< Ответ об отправке короткого сообщения
        DeliverSM = 0x00000005, //!< Прием сообщения от SMSC к ESME
        DeliverSMResp = 0x80000005, //!< Ответ о приеме сообщения
        Unbind = 0x00000006, //!< Закрыть соединение
        UnbindResp = 0x80000006, //!< Ответ о закрытии соединения
        ReplaceSM = 0x00000007, //!< Заменить предварительно предоставленное сообщение
        ReplaceSMResp = 0x80000007, //!< Ответ о замене сообщения
        CancelSM = 0x00000008, //!< Отменить предварительно предоставленное сообщение
        CancelSMResp = 0x80000008, //!< Ответ об отмене предварительно предоставленного сообщения
        BindTransceiver = 0x00000009, //! Установить связь на прием и передачу
        BindTransceiverResp = 0x80000009, //! Ответ об установлении связи на прием и передачу
        OutBind = 0x0000000B, //!< Запрос на установление связи от SMSC к ESME
        EnquireLink = 0x00000015, //!< Запрос на проверку связи
        EnquireLinkResp = 0x80000015, //!< Ответ на запрос о проверке связи
        SubmitMulti = 0x00000021, //!< Доставка сообщения списку получателей
        SubmitMultiResp = 0x80000021, //!< Ответ о доставке сообщения списку получателей
        AlertNotification = 0x00000102, //!< Сообщение о том, что конкретный абонент стал доступен
        DataSM = 0x00000103, //!< Передача данных между SMSC и ESME
        DataSMResp = 0x80000103 //!< Ответ о передачи данных между SMSC и ESME
    };

    /*!
     * \brief Статусы команд (ошибки)
     */
    enum CommandStatus
    {
        ESME_ROK = 0x00000000,
        ESME_RINVMSGLEN = 0x00000001,
        ESME_RINVCMDLEN = 0x00000002,
        ESME_RINVCMDID = 0x00000003,
        ESME_RINVBNDSTS = 0x00000004,
        ESME_RALYBND = 0x00000005,
        ESME_RINVPRTFLG = 0x00000006,
        ESME_RINVREGDLVFLG = 0x00000007,
        ESME_RSYSERR = 0x00000008
    };

    /*!
     * \brief Уровни приоритета
     *
     * Действия уровня зависит от протокола передачи, см. документацию по SMPP
     */
    enum PriorytiLevel
    {
        Level0, //!< Уровень 0
        Level1, //!< Уровень 1
        Level2, //!< Уровень 2
        Level3 //!< Уровень 3
    };

    /*!
     * \brief Типы кодирования данных
     */
    enum DataCodingTypes
    {
        SMSCDefaultAlphabet = 0, //!< Стандарный алфавитно-цифровой
        IA5_ASCII = 1, //!< IA5(CCITT T.50)/ASCII (ANSI X3.4)
        OctetUnspecified = 2, //!< Octet unspecified (8-bit binary)
        Latin1 = 3, //!< Latin1 (ISO-8859-1)
        Octetunspecified = 4, //!< Octetunspecified (8-bit binary)
        JIS = 5, //!< JIS(X 0208-1990)
        Cyrllic = 6, //!< Cyrllic(ISO-8859-5)
        Latin_Hebrew = 7, //!< Latin/Hebrew(ISO-8859-8)
        UCS2 = 8, //!< UCS2(ISO/IEC-10646)
        PictogramEncoding = 9, //!< Кодирование изображений
        MusicCodes = 10,  //!< Кодирование музыки (ISO-2022-JP)
        ExtendedKanjiJIS = 13, //!< ExtendedKanji JIS(X 0212-1990)
        KSC5601 = 14 //!< KSC 5601
    };

    /*!
     * \brief Типы дополнительных параметров
     */
    enum OptionalParameters
    {
        UndefinedOptionalParameter = 0x0000,
        DestAddrSubunit = 0x0005,
        DestNetworkType = 0x0006,
        DestBearerType = 0x0007,
        DestTelematicsId = 0x0008,
        SourceAddrSubunit = 0x000D,
        SourceNetworkType = 0x000E,
        SourceBearerType = 0x000F,
        SourceTelematicsId = 0x0010,
        QosTimeToLive = 0x0017,
        PayloadType = 0x0019,
        AdditionalStatusInfoText = 0x001D,
        ReceiptedMessageId = 0x001E,
        MsMsgWaitFacilities = 0x0030,
        PrivacyIndicator = 0x0201,
        SourceSubaddress = 0x0202,
        DestSubaddress = 0x0203,
        UserMessageReference = 0x0204,
        UserResponseCode = 0x0205,
        SourcePort = 0x020A,
        DestinationPort = 0x020B,
        SarMsgRefNum = 0x020C,
        LanguageIndicator = 0x020D,
        SarTotalSegments = 0x020E,
        SarSegmentSeqnum = 0x020F,
        SCInterfaceVersion = 0x0210,
        CallbackNumPresInd = 0x0302,
        CallbackNumAtag = 0x0303,
        NumberOfMessages = 0x0304,
        CallbackNum = 0x0381,
        DpfResult = 0x0420,
        SetDpf = 0x0421,
        MsAvailabilityStatus = 0x0422,
        NetworkErrorCode = 0x0423,
        MessagePayload = 0x0424,
        DeliveryFailureReason = 0x0425,
        MoreMessagesToSend = 0x0426,
        MessageState = 0x0427,
        UssdServiceOp = 0x0501,
        DisplayTime = 0x1201,
        SmsSignal = 0x1203,
        MsValidity = 0x1204,
        AlertOnMessageDelivery = 0x130C,
        ItsReplyType = 0x1380,
        ItsSessionInfo = 0x1383
    };

    /*!
     * \brief Метод для вывода на печать массива битов в виде hex-строки
     * Метод превращает массив битов в hex-строку удобную для анализа и возращает ее.
     * \param arcArray Массив битов
     * \return HEX-строка
     */
    static QString bytes2String(const QByteArray & arcArray);
};

#endif // IQSMPP_H
