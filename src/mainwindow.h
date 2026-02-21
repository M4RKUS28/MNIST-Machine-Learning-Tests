#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <memory>

#include "backproptrainer.h"
class DataSetLoader;
class Net;

// QT_CHARTS_USE_NAMESPACE
class QLineSeries;
class QChart;
class QChartView;
class QValueAxis;

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

protected:
  void closeEvent(QCloseEvent *event) override;

private slots:
  void on_pushButtonStartStop_clicked();
  void on_pushButton_save_clicked();
  void on_pushButton_load_clicked();
  void on_pushButtonEditArch_clicked();
  void on_pushButtonReset_clicked();

private:
  void setupChart();
  void addTestAccuracyPoint(double iteration, double accuracy);
  void addTrainAccuracyPoint(double iteration, double accuracy);

  Ui::MainWindow *ui;
  std::unique_ptr<DataSetLoader> dataSets;
  std::unique_ptr<Net> net;

  // Chart
  QChartView *chartView = nullptr;
  QChart *chart = nullptr;
  QLineSeries *testAccuracySeries = nullptr;
  QLineSeries *trainAccuracySeries = nullptr;
  QValueAxis *axisX = nullptr;
  QValueAxis *axisY = nullptr;

  bool running = false;
  size_t trainIndex = 0;

  // Evaluators (member so stop/close can cancel workers)
  BackPropTrainer testEvaluator;
  BackPropTrainer trainEvaluator;
};

#endif // MAINWINDOW_H
