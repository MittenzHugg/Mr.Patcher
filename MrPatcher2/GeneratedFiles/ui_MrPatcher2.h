/********************************************************************************
** Form generated from reading UI file 'MrPatcher2.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MRPATCHER2_H
#define UI_MRPATCHER2_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MrPatcher2Class
{
public:
    QAction *file_openROM_action;
    QAction *export_decomp_action;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *out_rom_label;
    QLineEdit *out_rom_lineEdit;
    QToolButton *out_rom_button;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QGridLayout *patch_grid;
    QComboBox *patch_comboBox;
    QLabel *patch_file_label;
    QLineEdit *patch_line;
    QToolButton *patch_select_button;
    QLabel *patch_comboBox_label;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *in_rom_label;
    QLineEdit *in_rom_lineEdit;
    QToolButton *in_rom_button;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *patchROM_button;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MrPatcher2Class)
    {
        if (MrPatcher2Class->objectName().isEmpty())
            MrPatcher2Class->setObjectName(QString::fromUtf8("MrPatcher2Class"));
        MrPatcher2Class->resize(443, 406);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MrPatcher2/Resources/MrPatch.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MrPatcher2Class->setWindowIcon(icon);
        file_openROM_action = new QAction(MrPatcher2Class);
        file_openROM_action->setObjectName(QString::fromUtf8("file_openROM_action"));
        export_decomp_action = new QAction(MrPatcher2Class);
        export_decomp_action->setObjectName(QString::fromUtf8("export_decomp_action"));
        export_decomp_action->setEnabled(false);
        centralWidget = new QWidget(MrPatcher2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        out_rom_label = new QLabel(centralWidget);
        out_rom_label->setObjectName(QString::fromUtf8("out_rom_label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        out_rom_label->setFont(font);

        horizontalLayout_2->addWidget(out_rom_label);

        out_rom_lineEdit = new QLineEdit(centralWidget);
        out_rom_lineEdit->setObjectName(QString::fromUtf8("out_rom_lineEdit"));
        out_rom_lineEdit->setEnabled(false);
        out_rom_lineEdit->setReadOnly(true);

        horizontalLayout_2->addWidget(out_rom_lineEdit);

        out_rom_button = new QToolButton(centralWidget);
        out_rom_button->setObjectName(QString::fromUtf8("out_rom_button"));
        out_rom_button->setEnabled(false);

        horizontalLayout_2->addWidget(out_rom_button);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        scrollArea = new QScrollArea(centralWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setEnabled(true);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 423, 246));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        patch_grid = new QGridLayout();
        patch_grid->setSpacing(6);
        patch_grid->setObjectName(QString::fromUtf8("patch_grid"));
        patch_comboBox = new QComboBox(scrollAreaWidgetContents);
        patch_comboBox->setObjectName(QString::fromUtf8("patch_comboBox"));
        patch_comboBox->setEnabled(false);

        patch_grid->addWidget(patch_comboBox, 1, 1, 1, 1);

        patch_file_label = new QLabel(scrollAreaWidgetContents);
        patch_file_label->setObjectName(QString::fromUtf8("patch_file_label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(patch_file_label->sizePolicy().hasHeightForWidth());
        patch_file_label->setSizePolicy(sizePolicy);
        patch_file_label->setFont(font);

        patch_grid->addWidget(patch_file_label, 0, 0, 1, 1);

        patch_line = new QLineEdit(scrollAreaWidgetContents);
        patch_line->setObjectName(QString::fromUtf8("patch_line"));
        patch_line->setEnabled(false);
        patch_line->setReadOnly(true);

        patch_grid->addWidget(patch_line, 1, 0, 1, 1);

        patch_select_button = new QToolButton(scrollAreaWidgetContents);
        patch_select_button->setObjectName(QString::fromUtf8("patch_select_button"));
        patch_select_button->setEnabled(false);

        patch_grid->addWidget(patch_select_button, 1, 2, 1, 1);

        patch_comboBox_label = new QLabel(scrollAreaWidgetContents);
        patch_comboBox_label->setObjectName(QString::fromUtf8("patch_comboBox_label"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(patch_comboBox_label->sizePolicy().hasHeightForWidth());
        patch_comboBox_label->setSizePolicy(sizePolicy1);
        patch_comboBox_label->setFont(font);

        patch_grid->addWidget(patch_comboBox_label, 0, 1, 1, 1);


        gridLayout_3->addLayout(patch_grid, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 1, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        in_rom_label = new QLabel(centralWidget);
        in_rom_label->setObjectName(QString::fromUtf8("in_rom_label"));
        in_rom_label->setFont(font);

        horizontalLayout->addWidget(in_rom_label);

        in_rom_lineEdit = new QLineEdit(centralWidget);
        in_rom_lineEdit->setObjectName(QString::fromUtf8("in_rom_lineEdit"));
        in_rom_lineEdit->setEnabled(true);
        in_rom_lineEdit->setReadOnly(true);

        horizontalLayout->addWidget(in_rom_lineEdit);

        in_rom_button = new QToolButton(centralWidget);
        in_rom_button->setObjectName(QString::fromUtf8("in_rom_button"));

        horizontalLayout->addWidget(in_rom_button);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        patchROM_button = new QPushButton(centralWidget);
        patchROM_button->setObjectName(QString::fromUtf8("patchROM_button"));
        patchROM_button->setEnabled(false);

        horizontalLayout_3->addWidget(patchROM_button);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 1);

        MrPatcher2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MrPatcher2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 443, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        MrPatcher2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MrPatcher2Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MrPatcher2Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MrPatcher2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MrPatcher2Class->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuFile->addAction(file_openROM_action);
        menuFile->addSeparator();
        menuFile->addAction(export_decomp_action);

        retranslateUi(MrPatcher2Class);

        QMetaObject::connectSlotsByName(MrPatcher2Class);
    } // setupUi

    void retranslateUi(QMainWindow *MrPatcher2Class)
    {
        MrPatcher2Class->setWindowTitle(QCoreApplication::translate("MrPatcher2Class", "Mr. Patcher", nullptr));
        file_openROM_action->setText(QCoreApplication::translate("MrPatcher2Class", "Open ROM", nullptr));
        export_decomp_action->setText(QCoreApplication::translate("MrPatcher2Class", "Export Decompressed", nullptr));
        out_rom_label->setText(QCoreApplication::translate("MrPatcher2Class", "Output ROM:", nullptr));
        out_rom_button->setText(QCoreApplication::translate("MrPatcher2Class", "...", nullptr));
        patch_file_label->setText(QCoreApplication::translate("MrPatcher2Class", "Patch File", nullptr));
        patch_select_button->setText(QCoreApplication::translate("MrPatcher2Class", "+", nullptr));
        patch_comboBox_label->setText(QCoreApplication::translate("MrPatcher2Class", "Level", nullptr));
        in_rom_label->setText(QCoreApplication::translate("MrPatcher2Class", "Unmodified ROM:", nullptr));
        in_rom_button->setText(QCoreApplication::translate("MrPatcher2Class", "...", nullptr));
        patchROM_button->setText(QCoreApplication::translate("MrPatcher2Class", "Patch ROM", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MrPatcher2Class", "File", nullptr));
        menuTools->setTitle(QCoreApplication::translate("MrPatcher2Class", "Tools", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MrPatcher2Class: public Ui_MrPatcher2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MRPATCHER2_H