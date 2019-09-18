1. Dmitriy, Russkikh.
2. Email: loctyr@gmail.com, Skype: gammon421/
3. Find a job as a C / C ++ programmer.
4. C/C++, Qt, SVN.
5. 
```cpp
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
 * \version alpha
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
```

```cpp
/*
 * Copyright (c) Dmitriy Russkikh <russkih.dv@infotek.ru>
 * All rights reserved
 */

#include "ipv4addressedit.h"
#include <QLineEdit>
#include <QKeyEvent>
#include <QValidator>

const QString IpV4AddressEdit::regExpStr = "^(([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))[.]){3}([01]?[0-9]?[0-9]|2([0-4][0-9]|5[0-5]))$";
const QString IpV4AddressEdit::separator = ".";
const short   IpV4AddressEdit::maxElementValueForAddSeparator = 25;
const short   IpV4AddressEdit::maxCharCountInGroup = 3;


void IpV4AddressEdit::init(){
    setMaxLength(15);
    setValidator(&rv__);
}

bool IpV4AddressEdit::isValid() {
    int pos = 0;
    QString tmp_text = text();
    return rv__.validate(tmp_text, pos) == QValidator::Acceptable;
}

void IpV4AddressEdit::keyPressEvent(QKeyEvent *key_) {
    if (cursorPosition() == text().length()) { //обрабатываем нахождение в конце строки
        QString tmp_text = text();
        int crPos = 0;

        if (key_->key() >= Qt::Key_0 && key_->key() <= Qt::Key_9) {
            int crPos2 = 0;
            QString tmp_text2 = tmp_text + key_->text();
            QString tmp_text3 = tmp_text + separator + key_->text();
            if (rv__.validate(tmp_text2, crPos) == QValidator::Invalid &&
                rv__.validate(tmp_text3, crPos2) != QValidator::Invalid) {
                setText(tmp_text + separator);
            } else {
                crPos2 = tmp_text2.lastIndexOf(separator);// находим предыдущий сепаратор для разделения разрядов адреса
                crPos2++;
                if ((tmp_text2.mid(crPos2).toInt() > maxElementValueForAddSeparator) //если начиная с числа maxElementValueForAddSeparator нажали любую цифру то точно надо ставить разделитель
                    ||
                    (tmp_text2.mid(crPos2).length() >= maxCharCountInGroup)
                ) {
                    QLineEdit::keyPressEvent(key_);
                    tmp_text = text();
                    tmp_text += separator;
                    if (rv__.validate(tmp_text, crPos) != QValidator::Invalid)
                        setText(tmp_text);
                    return;
                }
            }
        }

        if (key_->key() == Qt::Key_Right || key_->key() == Qt::Key_Comma) {
            tmp_text = text() + separator;
            if (rv__.validate(tmp_text, crPos) != QValidator::Invalid)
                setText(tmp_text);
        }
    } else {//если это не конец строки
        int crPos = 0;
        if ((key_->key() >= Qt::Key_0 && key_->key() <= Qt::Key_9) || (Qt::Key_Delete == key_->key()) || (Qt::Key_Backspace == key_->key())) {
            if (selectedText().isEmpty()) {
                int oldCursorPosition = cursorPosition();
                QString left = text().left(cursorPosition());
                QString right = text().mid(cursorPosition());

                QString tmp_text1;
                if (key_->key() >= Qt::Key_0 && key_->key() <= Qt::Key_9) {
                    tmp_text1 = left + key_->text() + right;
                    oldCursorPosition++;
                }
                if (Qt::Key_Delete == key_->key()) {
                    if (right.length()>0 && QChar('.') == right.at(0)) {
                        oldCursorPosition++;
                        left += '.';
                        right = right.mid(1);
                    }
                    if ((left.length() > 0) && (right.mid(1).length() > 0) && (QChar('.') == left.at(left.length()-1)) && (QChar('.') == right.mid(1).at(0))) {//если две точки подряд то улаляем одну
                        right = right.mid(1);
                    }
                    if (0 == left.length() && right.mid(1).length() > 0 && (QChar('.') == right.mid(1).at(0))) {//если после удаления первой станет точка то ее тоже удаляем
                        right = right.mid(1);
                    }
                    tmp_text1 = left + right.mid(1);
                }
                if (Qt::Key_Backspace == key_->key()) {
                    if ((left.length() > 0) && (QChar('.') == left.at(left.length()-1))) {
                        oldCursorPosition--;
                        right.push_front('.');// первую точку переносим
                        left = left.left(left.length()-1);
                    }
                    QString leftleft = left.left(left.length()-1);
                    if ((leftleft.length() > 0) && (right.length() > 0) && (QChar('.') == leftleft.at(leftleft.length()-1)) && (QChar('.') == right.at(0))) {
                        leftleft = leftleft.left(leftleft.length()-1);
                        oldCursorPosition--;
                    }
                    if (0 == leftleft.length() && (right.length() > 0) && (QChar('.') == right.at(0))) {
                        right = right.mid(1);
                    }
                    tmp_text1 = leftleft + right;
                    oldCursorPosition--;
                }

                crPos = 0;
                if (rv__.validate(tmp_text1, crPos) != QValidator::Invalid) {
                    setText(tmp_text1);
                    setCursorPosition(oldCursorPosition);
                    return;
                } else {
                    crPos = 0;
                    QString tmp_text2;
                    if ((Qt::Key_Delete == key_->key()) || (Qt::Key_Backspace == key_->key())) {
                        if (Qt::Key_Delete == key_->key())
                            tmp_text2 = left;
                        if (Qt::Key_Backspace == key_->key())
                            tmp_text2 = left.left(left.length()-1);
                        if (rv__.validate(tmp_text2, crPos) != QValidator::Invalid) {
                            setText(tmp_text2);
                            return;
                        }
                    } else {
                        tmp_text2 = left + key_->text();
                        if (rv__.validate(tmp_text2, crPos) != QValidator::Invalid) {
                            setText(tmp_text2);
                            int crPos2 = tmp_text2.lastIndexOf(separator);// находим предыдущий сепаратор для разделения разрядов адреса
                            crPos2++;
                            if ((tmp_text2.mid(crPos2).toInt() > maxElementValueForAddSeparator) //если начиная с числа maxElementValueForAddSeparator нажали любую цифру то точно надо ставить разделитель
                                ||
                                (tmp_text2.mid(crPos2).length() >= maxCharCountInGroup)
                            ) {
                                tmp_text2 += separator;
                                if (validator()->validate(tmp_text2, crPos) != QValidator::Invalid)
                                    setText(tmp_text2);
                                return;
                            }
                            return;
                        }
                    }
                }
            }
        }
    }

    QLineEdit::keyPressEvent(key_);
}
```
6. Creation of a ground control station UAV. Receiving telemetry data from the aircraft. Sending commands to control UAVs and ground equipment. Reception and processing of video and photo materials received from the payload. Data generation for target designation. Work with cartographic material using Google Map, Yandex Map. Display of moving, point, linear and areal objects. Work with graphics (OpenGL). Creating custom components for Qt, cross-platform programming Window, AstaLinux (CE, SE). C++ 11 standard. Creation of servers and clients for work on the TCP/IP protocol. Development of software documentation in accordance with GOST 19.
7. Izhevsk State Technical University M.T. Kalashnikov, Izhevsk. Instrument Engineering Faculty. Department of Applied Mathematics and Computer Science. Year of graduation 1998. 
8. I read technical literature with a dictionary.