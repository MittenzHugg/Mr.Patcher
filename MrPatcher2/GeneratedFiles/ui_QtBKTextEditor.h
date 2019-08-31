/********************************************************************************
** Form generated from reading UI file 'QtBKTextEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTBKTEXTEDITOR_H
#define UI_QTBKTEXTEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtBKTextEditor
{
public:

    void setupUi(QWidget *QtBKTextEditor)
    {
        if (QtBKTextEditor->objectName().isEmpty())
            QtBKTextEditor->setObjectName(QString::fromUtf8("QtBKTextEditor"));
        QtBKTextEditor->resize(400, 300);

        retranslateUi(QtBKTextEditor);

        QMetaObject::connectSlotsByName(QtBKTextEditor);
    } // setupUi

    void retranslateUi(QWidget *QtBKTextEditor)
    {
        QtBKTextEditor->setWindowTitle(QCoreApplication::translate("QtBKTextEditor", "QtBKTextEditor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtBKTextEditor: public Ui_QtBKTextEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTBKTEXTEDITOR_H
