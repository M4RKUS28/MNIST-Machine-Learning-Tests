#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

class DataSetLoader;
class Net;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_pushButtonStart_clicked();
  void on_pushButton_save_clicked();
  void on_pushButton_load_clicked();
  void on_pushButton_stop_clicked();

private:
  Ui::MainWindow *ui;
  std::unique_ptr<DataSetLoader> dataSets;
  std::unique_ptr<Net> net;
  bool running = false;
  size_t trainIndex = 0;
};

#endif // MAINWINDOW_H
