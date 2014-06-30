#ifndef IQSMPPABSTRACTOPTIONALPARAMETER_H
#define IQSMPPABSTRACTOPTIONALPARAMETER_H

#include <QObject>
#include "iqsmpp.h"

/*!
 * \brief Абстрактный класс для описания опциональных параметров
 * На основе этого класса создаются все опциональные параметры PDU. Для каждого опционального параметра
 * должны быть установленны его тип (\sa setTag()), его максимальный размер (\sa setMaxLength()), а
 * так же переопределены методы для кодирования (\sa encode()) и декодирования (\sa decode()).
 */
class IQSMPPAbstractOptionalParameter
{
public:
    /*!
     * \brief Конструктор
     */
    IQSMPPAbstractOptionalParameter();

    /*!
     * \brief Деструктор
     */
    virtual ~IQSMPPAbstractOptionalParameter();

    /*!
     * \brief Возвращает тип параметра
     * У каждого параметра есть тип, основанный на \ref IQSMPP::OptionalParameters, данный метод
     * возвращает данный тип.
     * \return Тип параметра
     * \sa setTag()
     */
    inline IQSMPP::OptionalParameters tag() const {return _tag;}

    /*!
     * \brief Возвращает максимальный размер тела параметра в октетах
     * У каждого параметра в спецификации задана его максимальный размер в октетах, данный метод
     * возвращает данный размер.
     * \return Максимальный размер в октетах
     * \sa setMaxLength()
     */
    inline quint16 maxLength() const {return _maxLength;}

    /*!
     * \brief Утснавливает тип параметра
     * У каждого параметра есть тип, основанный на \ref IQSMPP::OptionalParameters. Для каждого
     * параметра данный тип должен быть установлен.
     * \param tag Тип параметра
     * \sa tag()
     */
    void setTag(const IQSMPP::OptionalParameters tag);

    /*!
     * \brief Устанавливает максимальный размер тела параметра
     * У каждого параметра в спецификации задана его максимальный размер в октетах. Для каждого
     * параметра должен быть установлен данный размер.
     * \param maxLength Максимальный размер в октетах.
     * \sa maxLength()
     */
    void setMaxLength(const quint16 maxLength);

    /*!
     * \brief Виртуальный метод для кодирования параметра
     * Данный метод возвращает массив бит, которые содержат закодированный, согласно спецификации
     * SMPP, параметр вместе с заголовком.Все наследники должны переопределять данный метод.
     * \return Закодированный параметр вместе с заголовком
     * \sa decode()
     */
    virtual QByteArray encode() const {return QByteArray();}

    /*!
     * \brief Виртуальный метод для декодирования параметра
     * Данный метод принимает массив бит, которй содержит закодированный, согласно спецификации
     * SMPP, параметр вместе с заголовком, и декодирует его (устанавливает на основе его значение
     * параметра).
     * \param byteArray Массив бит, содержащих параметр вместе с заголовком
     * \return Результат декодирования
     * \sa encode()
     */
    virtual bool decode(const QByteArray &byteArray) {Q_UNUSED (byteArray); return false;}

private:
    quint16 _maxLength;
    IQSMPP::OptionalParameters _tag;
};

#endif // IQSMPPABSTRACTOPTIONALPARAMETER_H
