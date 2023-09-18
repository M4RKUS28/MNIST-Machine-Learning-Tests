#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "datasetloader.h"
#include "net.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonStart_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_load_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
    DataSetLoader * dataSets;
    Net * net;
    bool running;
    size_t index;


};
#endif // MAINWINDOW_H
