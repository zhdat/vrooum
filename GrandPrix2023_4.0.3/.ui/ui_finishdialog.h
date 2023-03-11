/********************************************************************************
** Form generated from reading UI file 'finishdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINISHDIALOG_H
#define UI_FINISHDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FinnishDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *table;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QPushButton *abortButton;

    void setupUi(QDialog *FinnishDialog)
    {
        if (FinnishDialog->objectName().isEmpty())
            FinnishDialog->setObjectName("FinnishDialog");
        FinnishDialog->resize(299, 88);
        verticalLayout = new QVBoxLayout(FinnishDialog);
        verticalLayout->setObjectName("verticalLayout");
        table = new QWidget(FinnishDialog);
        table->setObjectName("table");
        gridLayout = new QGridLayout(table);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(table);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(table);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        label_3 = new QLabel(table);
        label_3->setObjectName("label_3");

        gridLayout->addWidget(label_3, 0, 2, 1, 1);

        label_4 = new QLabel(table);
        label_4->setObjectName("label_4");

        gridLayout->addWidget(label_4, 0, 3, 1, 1);

        label_5 = new QLabel(table);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 0, 4, 1, 1);


        verticalLayout->addWidget(table);

        abortButton = new QPushButton(FinnishDialog);
        abortButton->setObjectName("abortButton");

        verticalLayout->addWidget(abortButton);


        retranslateUi(FinnishDialog);
        QObject::connect(abortButton, &QPushButton::clicked, FinnishDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(FinnishDialog);
    } // setupUi

    void retranslateUi(QDialog *FinnishDialog)
    {
        FinnishDialog->setWindowTitle(QCoreApplication::translate("FinnishDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Rank</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Driver</span></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Hops</span></p></body></html>", nullptr));
        label_4->setText(QCoreApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Gas</span></p></body></html>", nullptr));
        label_5->setText(QCoreApplication::translate("FinnishDialog", "<html><head/><body><p><span style=\" font-size:12pt; font-weight:600;\">Boosts</span></p></body></html>", nullptr));
        abortButton->setText(QCoreApplication::translate("FinnishDialog", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FinnishDialog: public Ui_FinnishDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINISHDIALOG_H
