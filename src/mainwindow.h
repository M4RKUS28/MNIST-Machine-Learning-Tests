#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define PROGRAM_VERSION "1.0.3"

#include <QMainWindow>
#include <QThread>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <memory>

class DataSetLoader;
class Net;
class DialogUeber;
class TrainingWorker;
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
  void on_pushButton_ueber_clicked();

  // Slots for TrainingWorker signals (queued, run on GUI thread)
  void onIterationUpdated(unsigned iter, unsigned epoch);
  void onSampleReady(QImage image, int predicted, unsigned label);
  void onTestAccuracyReady(double iteration, double accuracy);
  void onTrainAccuracyReady(double iteration, double accuracy);
  void onTrainingFinished(unsigned lastTrainIndex);

private:
  void setupChart();
  void addTestAccuracyPoint(double iteration, double accuracy);
  void addTrainAccuracyPoint(double iteration, double accuracy);
  void cleanupTraining();

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
  unsigned trainIndex = 0;
  DialogUeber *diaUber = nullptr;

  // Training thread
  QThread *m_trainThread = nullptr;
  TrainingWorker *m_trainWorker = nullptr;
};

#endif // MAINWINDOW_H
