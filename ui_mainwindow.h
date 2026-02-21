/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QWidget *chartPlaceholder;
    QGroupBox *predict_label_image_group;
    QVBoxLayout *sampleLayout;
    QSpacerItem *sampleTopSpacer;
    QLabel *labelSampleImage;
    QLabel *labelPredictionHeader;
    QLabel *labelPredicted;
    QLabel *labelCorrectHeader;
    QLabel *labelCorrect;
    QSpacerItem *sampleBottomSpacer;
    QGroupBox *groupBoxControls;
    QVBoxLayout *controlsLayout;
    QLabel *labelIterationHeader;
    QLabel *labelIteration;
    QLabel *labelEpochHeader;
    QLabel *labelEpoch;
    QFrame *line1;
    QLabel *labelAccuracyHeader;
    QLabel *labelAccuracy;
    QFrame *line2;
    QLabel *labelLearnRateHeader;
    QDoubleSpinBox *doubleSpinBoxLearnRate;
    QLabel *labelMomentumHeader;
    QDoubleSpinBox *doubleSpinBoxMomentum;
    QLabel *labelBatchSizeHeader;
    QSpinBox *spinBoxBatchSize;
    QFrame *line3;
    QPushButton *pushButton_save;
    QPushButton *pushButton_load;
    QFrame *line4;
    QPushButton *pushButtonStart;
    QPushButton *pushButton_stop;
    QSpacerItem *controlsBottomSpacer;
    QFrame *line5;
    QLabel *labelArchitectureHeader;
    QLabel *labelArchitecture;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(796, 559);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(8);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(6, 6, 6, 6);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        chartPlaceholder = new QWidget(groupBox);
        chartPlaceholder->setObjectName("chartPlaceholder");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chartPlaceholder->sizePolicy().hasHeightForWidth());
        chartPlaceholder->setSizePolicy(sizePolicy);
        chartPlaceholder->setMinimumSize(QSize(300, 300));

        gridLayout->addWidget(chartPlaceholder, 0, 0, 1, 1);


        horizontalLayout->addWidget(groupBox);

        predict_label_image_group = new QGroupBox(centralwidget);
        predict_label_image_group->setObjectName("predict_label_image_group");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(predict_label_image_group->sizePolicy().hasHeightForWidth());
        predict_label_image_group->setSizePolicy(sizePolicy1);
        sampleLayout = new QVBoxLayout(predict_label_image_group);
        sampleLayout->setSpacing(4);
        sampleLayout->setObjectName("sampleLayout");
        sampleTopSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sampleLayout->addItem(sampleTopSpacer);

        labelSampleImage = new QLabel(predict_label_image_group);
        labelSampleImage->setObjectName("labelSampleImage");
        labelSampleImage->setMinimumSize(QSize(280, 280));
        labelSampleImage->setMaximumSize(QSize(400, 400));
        labelSampleImage->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sampleLayout->addWidget(labelSampleImage);

        labelPredictionHeader = new QLabel(predict_label_image_group);
        labelPredictionHeader->setObjectName("labelPredictionHeader");
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        labelPredictionHeader->setFont(font);
        labelPredictionHeader->setTextFormat(Qt::TextFormat::AutoText);
        labelPredictionHeader->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sampleLayout->addWidget(labelPredictionHeader);

        labelPredicted = new QLabel(predict_label_image_group);
        labelPredicted->setObjectName("labelPredicted");
        QFont font1;
        font1.setPointSize(40);
        font1.setBold(true);
        labelPredicted->setFont(font1);
        labelPredicted->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sampleLayout->addWidget(labelPredicted);

        labelCorrectHeader = new QLabel(predict_label_image_group);
        labelCorrectHeader->setObjectName("labelCorrectHeader");
        labelCorrectHeader->setFont(font);
        labelCorrectHeader->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sampleLayout->addWidget(labelCorrectHeader);

        labelCorrect = new QLabel(predict_label_image_group);
        labelCorrect->setObjectName("labelCorrect");
        QFont font2;
        font2.setPointSize(22);
        labelCorrect->setFont(font2);
        labelCorrect->setAlignment(Qt::AlignmentFlag::AlignCenter);

        sampleLayout->addWidget(labelCorrect);

        sampleBottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sampleLayout->addItem(sampleBottomSpacer);


        horizontalLayout->addWidget(predict_label_image_group);

        groupBoxControls = new QGroupBox(centralwidget);
        groupBoxControls->setObjectName("groupBoxControls");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBoxControls->sizePolicy().hasHeightForWidth());
        groupBoxControls->setSizePolicy(sizePolicy2);
        groupBoxControls->setMinimumSize(QSize(180, 0));
        controlsLayout = new QVBoxLayout(groupBoxControls);
        controlsLayout->setObjectName("controlsLayout");
        labelIterationHeader = new QLabel(groupBoxControls);
        labelIterationHeader->setObjectName("labelIterationHeader");

        controlsLayout->addWidget(labelIterationHeader);

        labelIteration = new QLabel(groupBoxControls);
        labelIteration->setObjectName("labelIteration");
        QFont font3;
        font3.setBold(true);
        labelIteration->setFont(font3);

        controlsLayout->addWidget(labelIteration);

        labelEpochHeader = new QLabel(groupBoxControls);
        labelEpochHeader->setObjectName("labelEpochHeader");

        controlsLayout->addWidget(labelEpochHeader);

        labelEpoch = new QLabel(groupBoxControls);
        labelEpoch->setObjectName("labelEpoch");
        labelEpoch->setFont(font3);

        controlsLayout->addWidget(labelEpoch);

        line1 = new QFrame(groupBoxControls);
        line1->setObjectName("line1");
        line1->setFrameShape(QFrame::Shape::HLine);
        line1->setFrameShadow(QFrame::Shadow::Sunken);

        controlsLayout->addWidget(line1);

        labelAccuracyHeader = new QLabel(groupBoxControls);
        labelAccuracyHeader->setObjectName("labelAccuracyHeader");

        controlsLayout->addWidget(labelAccuracyHeader);

        labelAccuracy = new QLabel(groupBoxControls);
        labelAccuracy->setObjectName("labelAccuracy");
        QSizePolicy sizePolicy3(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelAccuracy->sizePolicy().hasHeightForWidth());
        labelAccuracy->setSizePolicy(sizePolicy3);
        labelAccuracy->setFont(font3);

        controlsLayout->addWidget(labelAccuracy);

        line2 = new QFrame(groupBoxControls);
        line2->setObjectName("line2");
        line2->setFrameShape(QFrame::Shape::HLine);
        line2->setFrameShadow(QFrame::Shadow::Sunken);

        controlsLayout->addWidget(line2);

        labelLearnRateHeader = new QLabel(groupBoxControls);
        labelLearnRateHeader->setObjectName("labelLearnRateHeader");

        controlsLayout->addWidget(labelLearnRateHeader);

        doubleSpinBoxLearnRate = new QDoubleSpinBox(groupBoxControls);
        doubleSpinBoxLearnRate->setObjectName("doubleSpinBoxLearnRate");
        doubleSpinBoxLearnRate->setDecimals(6);
        doubleSpinBoxLearnRate->setSingleStep(0.010000000000000);
        doubleSpinBoxLearnRate->setValue(0.000500000000000);

        controlsLayout->addWidget(doubleSpinBoxLearnRate);

        labelMomentumHeader = new QLabel(groupBoxControls);
        labelMomentumHeader->setObjectName("labelMomentumHeader");

        controlsLayout->addWidget(labelMomentumHeader);

        doubleSpinBoxMomentum = new QDoubleSpinBox(groupBoxControls);
        doubleSpinBoxMomentum->setObjectName("doubleSpinBoxMomentum");
        doubleSpinBoxMomentum->setDecimals(4);
        doubleSpinBoxMomentum->setSingleStep(0.010000000000000);
        doubleSpinBoxMomentum->setValue(0.600000000000000);

        controlsLayout->addWidget(doubleSpinBoxMomentum);

        labelBatchSizeHeader = new QLabel(groupBoxControls);
        labelBatchSizeHeader->setObjectName("labelBatchSizeHeader");

        controlsLayout->addWidget(labelBatchSizeHeader);

        spinBoxBatchSize = new QSpinBox(groupBoxControls);
        spinBoxBatchSize->setObjectName("spinBoxBatchSize");
        spinBoxBatchSize->setMaximum(9999);
        spinBoxBatchSize->setValue(64);

        controlsLayout->addWidget(spinBoxBatchSize);

        line3 = new QFrame(groupBoxControls);
        line3->setObjectName("line3");
        line3->setFrameShape(QFrame::Shape::HLine);
        line3->setFrameShadow(QFrame::Shadow::Sunken);

        controlsLayout->addWidget(line3);

        pushButton_save = new QPushButton(groupBoxControls);
        pushButton_save->setObjectName("pushButton_save");

        controlsLayout->addWidget(pushButton_save);

        pushButton_load = new QPushButton(groupBoxControls);
        pushButton_load->setObjectName("pushButton_load");

        controlsLayout->addWidget(pushButton_load);

        line4 = new QFrame(groupBoxControls);
        line4->setObjectName("line4");
        line4->setFrameShape(QFrame::Shape::HLine);
        line4->setFrameShadow(QFrame::Shadow::Sunken);

        controlsLayout->addWidget(line4);

        pushButtonStart = new QPushButton(groupBoxControls);
        pushButtonStart->setObjectName("pushButtonStart");

        controlsLayout->addWidget(pushButtonStart);

        pushButton_stop = new QPushButton(groupBoxControls);
        pushButton_stop->setObjectName("pushButton_stop");

        controlsLayout->addWidget(pushButton_stop);

        controlsBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        controlsLayout->addItem(controlsBottomSpacer);

        line5 = new QFrame(groupBoxControls);
        line5->setObjectName("line5");
        line5->setFrameShape(QFrame::Shape::HLine);
        line5->setFrameShadow(QFrame::Shadow::Sunken);

        controlsLayout->addWidget(line5);

        labelArchitectureHeader = new QLabel(groupBoxControls);
        labelArchitectureHeader->setObjectName("labelArchitectureHeader");

        controlsLayout->addWidget(labelArchitectureHeader);

        labelArchitecture = new QLabel(groupBoxControls);
        labelArchitecture->setObjectName("labelArchitecture");
        QFont font4;
        font4.setPointSize(7);
        labelArchitecture->setFont(font4);
        labelArchitecture->setWordWrap(true);

        controlsLayout->addWidget(labelArchitecture);


        horizontalLayout->addWidget(groupBoxControls);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MNIST-Net", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Progress", nullptr));
        predict_label_image_group->setTitle(QCoreApplication::translate("MainWindow", "Image - Prediction - Label", nullptr));
#if QT_CONFIG(tooltip)
        labelSampleImage->setToolTip(QCoreApplication::translate("MainWindow", "Current training sample image (28x28 scaled to 280x280)", nullptr));
#endif // QT_CONFIG(tooltip)
        labelSampleImage->setText(QString());
        labelPredictionHeader->setText(QCoreApplication::translate("MainWindow", "Prediction:", nullptr));
#if QT_CONFIG(tooltip)
        labelPredicted->setToolTip(QCoreApplication::translate("MainWindow", "Predicted digit by the neural network", nullptr));
#endif // QT_CONFIG(tooltip)
        labelPredicted->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        labelCorrectHeader->setText(QCoreApplication::translate("MainWindow", "Actual Value:", nullptr));
#if QT_CONFIG(tooltip)
        labelCorrect->setToolTip(QCoreApplication::translate("MainWindow", "Ground-truth label from the MNIST dataset", nullptr));
#endif // QT_CONFIG(tooltip)
        labelCorrect->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        groupBoxControls->setTitle(QCoreApplication::translate("MainWindow", "Training Controls", nullptr));
        labelIterationHeader->setText(QCoreApplication::translate("MainWindow", "Iteration:", nullptr));
#if QT_CONFIG(tooltip)
        labelIteration->setToolTip(QCoreApplication::translate("MainWindow", "Current training iteration count", nullptr));
#endif // QT_CONFIG(tooltip)
        labelIteration->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelEpochHeader->setText(QCoreApplication::translate("MainWindow", "Epoch:", nullptr));
#if QT_CONFIG(tooltip)
        labelEpoch->setToolTip(QCoreApplication::translate("MainWindow", "Number of complete passes over the training set", nullptr));
#endif // QT_CONFIG(tooltip)
        labelEpoch->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        labelAccuracyHeader->setText(QCoreApplication::translate("MainWindow", "Accuracy:", nullptr));
#if QT_CONFIG(tooltip)
        labelAccuracy->setToolTip(QCoreApplication::translate("MainWindow", "Latest test accuracy on the MNIST test set", nullptr));
#endif // QT_CONFIG(tooltip)
        labelAccuracy->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        labelLearnRateHeader->setText(QCoreApplication::translate("MainWindow", "Learning Rate:", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBoxLearnRate->setToolTip(QCoreApplication::translate("MainWindow", "Learning rate controls the step size during gradient descent", nullptr));
#endif // QT_CONFIG(tooltip)
        labelMomentumHeader->setText(QCoreApplication::translate("MainWindow", "Momentum:", nullptr));
#if QT_CONFIG(tooltip)
        doubleSpinBoxMomentum->setToolTip(QCoreApplication::translate("MainWindow", "Momentum accelerates SGD by accumulating past gradients", nullptr));
#endif // QT_CONFIG(tooltip)
        labelBatchSizeHeader->setText(QCoreApplication::translate("MainWindow", "Batch Size:", nullptr));
#if QT_CONFIG(tooltip)
        spinBoxBatchSize->setToolTip(QCoreApplication::translate("MainWindow", "Mini-batch size for weight updates (0 = online learning)", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        pushButton_save->setToolTip(QCoreApplication::translate("MainWindow", "Save network weights to mynet.csv", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_save->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_load->setToolTip(QCoreApplication::translate("MainWindow", "Load network weights from mynet.csv", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_load->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
#if QT_CONFIG(tooltip)
        pushButtonStart->setToolTip(QCoreApplication::translate("MainWindow", "Start training the neural network", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButtonStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_stop->setToolTip(QCoreApplication::translate("MainWindow", "Stop training after current iteration", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_stop->setText(QCoreApplication::translate("MainWindow", "Stop", nullptr));
        labelArchitectureHeader->setText(QCoreApplication::translate("MainWindow", "Architecture:", nullptr));
#if QT_CONFIG(tooltip)
        labelArchitecture->setToolTip(QCoreApplication::translate("MainWindow", "Neural network layer topology", nullptr));
#endif // QT_CONFIG(tooltip)
        labelArchitecture->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
