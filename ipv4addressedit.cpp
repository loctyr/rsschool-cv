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
