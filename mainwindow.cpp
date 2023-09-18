#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <unistd.h>
#include <QApplication>
#include <iostream>
#include "backproptrainer.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataSets = new DataSetLoader();
    if(!dataSets->loadSets("./dataset"))
        exit(10);

    net = new Net("784_INPUT_LAYER,"
                  "1024_SUM_TANH,"
                  "1024_SUM_TANH,"
                  "010_SUM_SMAX",
                  0.025);

    running = false;
    index = 0;
}



MainWindow::~MainWindow()
{
    running = false;
    delete ui;
    delete dataSets;
}


void MainWindow::on_pushButtonStart_clicked()
{
    if(running)
        return;
    running = true;

    double* values = new double[10];
    BackPropTrainer bpt;
    BackPropTrainer bpt_td;

    QFile out1(((true) ? "train_values.txt" : "test_values.txt"));
    out1.open(QFile::OpenMode::enum_type::ReadWrite | QFile::OpenMode::enum_type::Append);
    out1.write( QString("New Training...\n").toStdString().c_str());
    out1.close();

    QFile out2(((false) ? "train_values.txt" : "test_values.txt"));
    out2.open(QFile::OpenMode::enum_type::ReadWrite | QFile::OpenMode::enum_type::Append);
    out2.write( QString("New Training...\n").toStdString().c_str());
    out2.close();

    for(; index < 3000000 && running; index++) {
        if(index % 60000 == 0) {
            std::cout << "=> Epoch " << index / 60000 << std::endl;
//            bpt_td.test(index, net, dataSets);
            QFile out1(((true) ? "train_values.txt" : "test_values.txt"));
            out1.open(QFile::OpenMode::enum_type::ReadWrite | QFile::OpenMode::enum_type::Append);
            out1.write( QString("=> Epoch " + QString::number(index / 60000) + "\n").toStdString().c_str());
            out1.close();

            QFile out2(((false) ? "train_values.txt" : "test_values.txt"));
            out2.open(QFile::OpenMode::enum_type::ReadWrite | QFile::OpenMode::enum_type::Append);
            out2.write( QString("=> Epoch " + QString::number(index / 60000) + "\n").toStdString().c_str());
            out2.close();
        }

        ui->label_iteration->setText(QString::number(index));
        QApplication::processEvents();

        if(index%500 == 0)
            bpt.test(index, net, dataSets);
        if(index%3000 == 0)
            bpt_td.test(index, net, dataSets, true);

        //train net

        int res_num = -1;
        double res_val = 0.0;

        Data * e = dataSets->trainData()->at(index % 60000);//->randomTrainData(0, -1);
        if(index % 500 == 0) {
            ui->label->setPixmap(QPixmap::fromImage( e->img->scaled(280, 280) ));
            ui->label->update();
            QApplication::processEvents();
        }

        net->feedForward(e->img_d);
        net->getResults(values);
        for(int i = 0; i < 10; i++) {
            if(values[i] > res_val) {
                res_val = values[i];
                res_num = i;
            }
        }

        for(unsigned i = 0; i < 10; i++) {
            if(i == e->num)
                values[i] = 0.99;
            else
                values[i] = 0.01;
        }
        net->backProp(values, ui->doubleSpinBoxLearnRate->value(), ui->doubleSpinBox_Monumentum->value());

        if(!(index%100)){
            ui->label_errorrrate->setText(QString::number(net->recentAverrageError()));
            QApplication::processEvents();
        }
        if(index % 500 == 0) {
            ui->label_num->setText(QString::number(res_num));
            ui->label_num_cor->setText(QString::number(e->num));

            QApplication::processEvents();
        }

        QApplication::processEvents();
    }


    delete[] values;
}


void MainWindow::on_pushButton_save_clicked()
{
    net->save_to("mynet.csv");
}


void MainWindow::on_pushButton_load_clicked()
{
    net->load_from("mynet.csv");
}


void MainWindow::on_pushButton_stop_clicked()
{
    running = false;
}

