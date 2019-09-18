/*
 * Copyright (c) Dmitriy Russkikh <russkih.dv@infotek.ru>
 * All rights reserved
 */

#ifndef __IPV4_ADDRESS_EDIT_H__
#define __IPV4_ADDRESS_EDIT_H__

#include <QLineEdit>
#include <QRegExpValidator>

/*!
 * \brief Класс IpV4AddressEdit предназначен для ввода с контролем правильности IP адресов 4 версии
 * \author Дмитрий Русских
 */

class IpV4AddressEdit : public QLineEdit {
    Q_OBJECT
public:
    /*!
     * \brief Конструктор аналогичный QLineEdit
     * \param parent_ Указатель на родительский QWidget
     */
    explicit IpV4AddressEdit(QWidget* parent_ = nullptr): QLineEdit(parent_), rv__(QRegExp(regExpStr), this) {init();}
    /*!
     * \brief Конструктор аналогичный QLineEdit
     * \param str_ Строка текста
     * \param parent_ Указатель на родительский QWidget
     */
    explicit IpV4AddressEdit(const QString &str_, QWidget* parent_ = nullptr) : QLineEdit(str_, parent_), rv__(QRegExp(regExpStr), this) {init();}
    /*!
     * \brief Функция проверки валидности введенных данных
     * \return Истина -- данные валидные, Ложь -- данные не валидные
     */
    bool isValid();
public:
    static const QString regExpStr; ///< Строка с регулярным выражением для проверки корректности вводимых данных
    static const QString separator; ///< Разделитель для блоков IP адреса
    static const short maxElementValueForAddSeparator; ///< Максимальное значение элемента при превышении которого автоматически добавляем разделитель
    static const short maxCharCountInGroup; ///< Количество символов группе (между разделителями)
protected:
    /*!
     * \brief Перегруженная функция обрабатывающая нажатие на клавишу
     * \param key_ Указатель на событие нажатой клавиши
     */
    void keyPressEvent(QKeyEvent *key_) Q_DECL_OVERRIDE;
private:
    QRegExpValidator rv__; ///< Регулярное выражение для проверки корректности вводимых данных

    /*!
     * \brief Метод инициализации для вызова из констрктора
     */
    void init();
};

#endif //QIPV4_ADDRESS_EDIT_H
