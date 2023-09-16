#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <unistd.h>
#include <QApplication>
#include <iostream>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataSets = new DataSetLoader();
    dataSets->loadSets("C:/Users/Markus/Documents/Programier Zeug/Qt-Projekte/MNIST-Net/dataset");

    net = new Net("784_SUM_TANH,"
                  "256_SUM_TANH,"
                  "128_SUM_TANH,"
                  "010_SUM_SMAX",
                  0.025);

}


MainWindow::~MainWindow()
{
    delete ui;
    delete dataSets;
}


void MainWindow::on_pushButtonStart_clicked()
{
    double* values = new double[10];

    for(int i = 0; i < 100000; i++) {
        ui->label_iteration->setText(QString::number(i));
        QApplication::processEvents();

        int res_num = -1;
        double res_val = 0.0;

        Data * e = dataSets->randomTrainData(0, -1);
        if(i % 500 == 0) {
            ui->label->setPixmap(QPixmap::fromImage( e->img->scaled(280, 280) ));
            ui->label->update();
            QApplication::processEvents();
        }

//        for(int i = 0; i < 28*28; i++)
//            std::cout << e->img_d[i] << " " << (i % 28 == 0 ? "\n" : "");
//        std::cout << std::endl;
//        sleep(5);

        net->feedForward(e->img_d);
        net->getResults(values);
        for(int i = 0; i < 10; i++) {
            if(values[i] > res_val) {
                res_val = values[i];
                res_num = i;
            }
        }
//        for(int i = 0; i < 10; i++)
//            std::cout << values[i] << " ";
//        std::cout << std::endl;



        for(unsigned i = 0; i < 10; i++) {
            if(i == e->num)
                values[i] = 0.99;
            else
                values[i] = 0.01;
        }
        net->backProp(values, ui->doubleSpinBoxLearnRate->value(), ui->doubleSpinBox_Monumentum->value());
//        std::cout << i << ": in: " <<  e->num << " out: " << res_num << " error: " << net->recentAverrageError() << ((i % 100 == 0) ? "\n" : "" ) << std::endl;
        if(!(i%100)){
            ui->label_errorrrate->setText(QString::number(net->recentAverrageError()));
            QApplication::processEvents();
        }
        if(i % 500 == 0) {
            ui->label_num->setText(QString::number(res_num));
            ui->label_num_cor->setText(QString::number(e->num));

            QApplication::processEvents();
        }

        QApplication::processEvents();
    }


    delete[] values;
}

