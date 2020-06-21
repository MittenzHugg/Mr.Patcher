/********************************************************************************
** Form generated from reading UI file 'QtModelViewWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMODELVIEWWIDGET_H
#define UI_QTMODELVIEWWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>
#include "glmodelwidget.h"

QT_BEGIN_NAMESPACE

class Ui_QtModelViewWidget
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QListView *Model_List_View;
    GLModelWidget *openGLWidget;

    void setupUi(QWidget *QtModelViewWidget)
    {
        if (QtModelViewWidget->objectName().isEmpty())
            QtModelViewWidget->setObjectName(QString::fromUtf8("QtModelViewWidget"));
        QtModelViewWidget->resize(958, 550);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QtModelViewWidget->sizePolicy().hasHeightForWidth());
        QtModelViewWidget->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(QtModelViewWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(QtModelViewWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy1);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 274, 530));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents->setSizePolicy(sizePolicy2);
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        Model_List_View = new QListView(scrollAreaWidgetContents);
        Model_List_View->setObjectName(QString::fromUtf8("Model_List_View"));
        sizePolicy1.setHeightForWidth(Model_List_View->sizePolicy().hasHeightForWidth());
        Model_List_View->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(Model_List_View, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);

        openGLWidget = new GLModelWidget(QtModelViewWidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));

        gridLayout->addWidget(openGLWidget, 0, 1, 1, 1);


        retranslateUi(QtModelViewWidget);

        QMetaObject::connectSlotsByName(QtModelViewWidget);
    } // setupUi

    void retranslateUi(QWidget *QtModelViewWidget)
    {
        QtModelViewWidget->setWindowTitle(QCoreApplication::translate("QtModelViewWidget", "Mr. Patcher - Model Viewer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtModelViewWidget: public Ui_QtModelViewWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMODELVIEWWIDGET_H
