/********************************************************************************
** Form generated from reading UI file 'Form.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_3;
    QLabel *label_3;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(276, 228);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        radioButton = new QRadioButton(Form);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout->addWidget(radioButton);

        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAutoFillBackground(false);
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 4);"));
        label->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        radioButton_2 = new QRadioButton(Form);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_2->addWidget(radioButton_2);

        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("background-color: rgb(21, 255, 5);"));

        horizontalLayout_2->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_3 = new QRadioButton(Form);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        horizontalLayout_3->addWidget(radioButton_3);

        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(17, 29, 255);"));

        horizontalLayout_3->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 219, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer = new QSpacerItem(238, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton = new QPushButton(Form);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_4->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_4);


        retranslateUi(Form);
        QObject::connect(pushButton, SIGNAL(clicked()), Form, SLOT(close()));
        QObject::connect(radioButton_3, SIGNAL(clicked(bool)), radioButton_2, SLOT(setDisabled(bool)));
        QObject::connect(radioButton_3, SIGNAL(clicked(bool)), radioButton, SLOT(setDisabled(bool)));
        QObject::connect(radioButton_2, SIGNAL(clicked(bool)), radioButton, SLOT(setDisabled(bool)));
        QObject::connect(radioButton_2, SIGNAL(clicked(bool)), radioButton_3, SLOT(setDisabled(bool)));
        QObject::connect(radioButton, SIGNAL(clicked(bool)), radioButton_2, SLOT(setDisabled(bool)));
        QObject::connect(radioButton, SIGNAL(clicked(bool)), radioButton_3, SLOT(setDisabled(bool)));

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        radioButton->setText(QCoreApplication::translate("Form", "Vermell", nullptr));
        label->setText(QCoreApplication::translate("Form", "  VERMELL", nullptr));
        radioButton_2->setText(QCoreApplication::translate("Form", "Verd", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "  VERD", nullptr));
        radioButton_3->setText(QCoreApplication::translate("Form", "Blau", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "  BLAU", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "&Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
