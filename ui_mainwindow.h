/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QLabel *label_num;
    QLabel *label_num_cor;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButtonStart;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_5;
    QLabel *label_iteration;
    QFrame *line;
    QLabel *label_3;
    QLabel *label_errorrrate;
    QFrame *line_2;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBoxLearnRate;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_Monumentum;
    QSpacerItem *verticalSpacer_3;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1139, 804);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");

        horizontalLayout->addWidget(graphicsView);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setMinimumSize(QSize(280, 280));
        label->setMaximumSize(QSize(280, 280));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        label_num = new QLabel(centralwidget);
        label_num->setObjectName("label_num");
        QFont font;
        font.setPointSize(40);
        font.setBold(true);
        label_num->setFont(font);
        label_num->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_num);

        label_num_cor = new QLabel(centralwidget);
        label_num_cor->setObjectName("label_num_cor");
        QFont font1;
        font1.setPointSize(25);
        label_num_cor->setFont(font1);
        label_num_cor->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_num_cor);


        horizontalLayout->addLayout(verticalLayout_2);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButtonStart = new QPushButton(groupBox);
        pushButtonStart->setObjectName("pushButtonStart");

        verticalLayout->addWidget(pushButtonStart);

        verticalSpacer_2 = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");

        verticalLayout->addWidget(label_5);

        label_iteration = new QLabel(groupBox);
        label_iteration->setObjectName("label_iteration");

        verticalLayout->addWidget(label_iteration);

        line = new QFrame(groupBox);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        verticalLayout->addWidget(label_3);

        label_errorrrate = new QLabel(groupBox);
        label_errorrrate->setObjectName("label_errorrrate");
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_errorrrate->sizePolicy().hasHeightForWidth());
        label_errorrrate->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(label_errorrrate);

        line_2 = new QFrame(groupBox);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        doubleSpinBoxLearnRate = new QDoubleSpinBox(groupBox);
        doubleSpinBoxLearnRate->setObjectName("doubleSpinBoxLearnRate");
        doubleSpinBoxLearnRate->setDecimals(4);
        doubleSpinBoxLearnRate->setSingleStep(0.010000000000000);
        doubleSpinBoxLearnRate->setValue(0.200000000000000);

        verticalLayout->addWidget(doubleSpinBoxLearnRate);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");

        verticalLayout->addWidget(label_4);

        doubleSpinBox_Monumentum = new QDoubleSpinBox(groupBox);
        doubleSpinBox_Monumentum->setObjectName("doubleSpinBox_Monumentum");
        doubleSpinBox_Monumentum->setDecimals(4);
        doubleSpinBox_Monumentum->setSingleStep(0.010000000000000);

        verticalLayout->addWidget(doubleSpinBox_Monumentum);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout->addWidget(groupBox);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1139, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        label_num->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_num_cor->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "start", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Durchlauf:", nullptr));
        label_iteration->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Error Rate:", nullptr));
        label_errorrrate->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "LearnRate", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Monumentum:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H