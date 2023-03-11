/********************************************************************************
** Form generated from reading UI file 'stderrview.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STDERRVIEW_H
#define UI_STDERRVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StdErrView
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *teDriver1;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QPlainTextEdit *teDriver2;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QPlainTextEdit *teDriver3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pbClose;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSlider *fontSize;
    QLabel *label_2;

    void setupUi(QWidget *StdErrView)
    {
        if (StdErrView->objectName().isEmpty())
            StdErrView->setObjectName("StdErrView");
        StdErrView->resize(604, 363);
        verticalLayout = new QVBoxLayout(StdErrView);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(StdErrView);
        tabWidget->setObjectName("tabWidget");
        tab = new QWidget();
        tab->setObjectName("tab");
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName("verticalLayout_2");
        teDriver1 = new QPlainTextEdit(tab);
        teDriver1->setObjectName("teDriver1");

        verticalLayout_2->addWidget(teDriver1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName("verticalLayout_3");
        teDriver2 = new QPlainTextEdit(tab_2);
        teDriver2->setObjectName("teDriver2");

        verticalLayout_3->addWidget(teDriver2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        teDriver3 = new QPlainTextEdit(tab_3);
        teDriver3->setObjectName("teDriver3");

        verticalLayout_4->addWidget(teDriver3);

        tabWidget->addTab(tab_3, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        pbClose = new QPushButton(StdErrView);
        pbClose->setObjectName("pbClose");

        horizontalLayout_2->addWidget(pbClose);

        horizontalSpacer = new QSpacerItem(30, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(StdErrView);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        fontSize = new QSlider(StdErrView);
        fontSize->setObjectName("fontSize");
        fontSize->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(fontSize);

        label_2 = new QLabel(StdErrView);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        horizontalLayout->setStretch(1, 2);

        horizontalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(StdErrView);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(StdErrView);
    } // setupUi

    void retranslateUi(QWidget *StdErrView)
    {
        StdErrView->setWindowTitle(QCoreApplication::translate("StdErrView", "Form", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("StdErrView", "Driver 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("StdErrView", "Driver 2", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("StdErrView", "Driver 3", nullptr));
        pbClose->setText(QCoreApplication::translate("StdErrView", "Close", nullptr));
        label->setText(QCoreApplication::translate("StdErrView", "<html><head/><body><p><span style=\" font-weight:600;\">Font size</span>&nbsp;&nbsp;&nbsp;&nbsp;Small</p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("StdErrView", "Normal", nullptr));
    } // retranslateUi

};

namespace Ui {
    class StdErrView: public Ui_StdErrView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STDERRVIEW_H
