#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "datasetloader.h"
#include "net.h"

#include <QApplication>
#include <QCloseEvent>
#include <QPixmap>
#include <QVBoxLayout>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <iostream>

// ---------------------------------------------------------------------------
// Construction / Destruction
// ---------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      dataSets(std::make_unique<DataSetLoader>()),
      net(std::make_unique<Net>("784_INPUT_LAYER,"
                                "256_SUM_TANH,"
                                "010_SUM_SMAX",
                                0.025)) {
  ui->setupUi(this);

  if (!dataSets->loadSets("../dataset")) {
    std::cerr << "Failed to load MNIST dataset from ../dataset" << std::endl;
  }

  setupChart();

  // Display network architecture
  ui->labelArchitecture->setText("784 INPUT\n256 TANH\n10 SMAX");
}

MainWindow::~MainWindow() {
  running = false;
  delete ui;
  // dataSets and net cleaned up by unique_ptr
  // chart/series owned by chartView (Qt parent-child)
}

// ---------------------------------------------------------------------------
// Chart setup
// ---------------------------------------------------------------------------

void MainWindow::setupChart() {
  chart = new QChart();
  chart->setTitle("Training Progress");
  chart->setAnimationOptions(QChart::NoAnimation);

  // Test accuracy series (blue)
  testAccuracySeries = new QLineSeries();
  testAccuracySeries->setName("Test Accuracy");
  testAccuracySeries->setColor(QColor(41, 128, 185)); // flat blue
  chart->addSeries(testAccuracySeries);

  // Train accuracy series (orange)
  trainAccuracySeries = new QLineSeries();
  trainAccuracySeries->setName("Train Accuracy");
  trainAccuracySeries->setColor(QColor(230, 126, 34)); // flat orange
  chart->addSeries(trainAccuracySeries);

  // X axis: iterations
  axisX = new QValueAxis();
  axisX->setTitleText("Iteration");
  axisX->setLabelFormat("%i");
  axisX->setRange(0, 10000);
  chart->addAxis(axisX, Qt::AlignBottom);
  testAccuracySeries->attachAxis(axisX);
  trainAccuracySeries->attachAxis(axisX);

  // Y axis: accuracy percentage
  axisY = new QValueAxis();
  axisY->setTitleText("Accuracy (%)");
  axisY->setRange(0, 100);
  axisY->setLabelFormat("%.1f");
  chart->addAxis(axisY, Qt::AlignLeft);
  testAccuracySeries->attachAxis(axisY);
  trainAccuracySeries->attachAxis(axisY);

  // Create chart view and insert into placeholder
  chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  auto *layout = new QVBoxLayout(ui->chartPlaceholder);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(chartView);

  // Seed both series with a (0, 0) origin point
  testAccuracySeries->append(0, 0);
  trainAccuracySeries->append(0, 0);
}

void MainWindow::addTestAccuracyPoint(double iteration, double accuracy) {
  testAccuracySeries->append(iteration, accuracy);
  // Auto-scale X axis
  if (iteration > axisX->max())
    axisX->setMax(iteration * 1.2);
}

void MainWindow::addTrainAccuracyPoint(double iteration, double accuracy) {
  trainAccuracySeries->append(iteration, accuracy);
  if (iteration > axisX->max())
    axisX->setMax(iteration * 1.2);
}

// ---------------------------------------------------------------------------
// Training
// ---------------------------------------------------------------------------

static constexpr int NUM_DIGITS = 10;
static constexpr unsigned TRAIN_SET_SIZE = 60000;
static constexpr unsigned EPOCH_LOG_INTERVAL = TRAIN_SET_SIZE;
static constexpr unsigned UI_UPDATE_INTERVAL = 500;
static constexpr unsigned TEST_INTERVAL = 500;
static constexpr unsigned TRAIN_TEST_INTERVAL = 3000;
static constexpr unsigned ERROR_DISPLAY_INTERVAL = 100;
static constexpr unsigned CHART_UPDATE_INTERVAL = 500;

void MainWindow::on_pushButtonStart_clicked() {
  if (running)
    return;
  running = true;

  double values[NUM_DIGITS];
  size_t lastPlottedTestIdx = 0;
  size_t lastPlottedTrainIdx = 0;

  // Log start of new training session
  for (const char *filename : {"train_values.txt", "test_values.txt"}) {
    QFile out(filename);
    if (out.open(QIODevice::ReadWrite | QIODevice::Append)) {
      out.write("New Training...\n");
      out.close();
    }
  }

  for (; trainIndex < 3000000 && running; trainIndex++) {
    // Epoch boundary logging
    if (trainIndex % EPOCH_LOG_INTERVAL == 0) {
      unsigned epoch = trainIndex / EPOCH_LOG_INTERVAL;
      std::cout << "=> Epoch " << epoch << std::endl;

      for (const char *filename : {"train_values.txt", "test_values.txt"}) {
        QFile out(filename);
        if (out.open(QIODevice::ReadWrite | QIODevice::Append)) {
          out.write(QString("=> Epoch %1\n").arg(epoch).toUtf8());
          out.close();
        }
      }
    }

    // Update iteration and epoch counters in UI
    ui->labelIteration->setText(QString::number(trainIndex));
    ui->labelEpoch->setText(QString::number(trainIndex / EPOCH_LOG_INTERVAL));
    QApplication::processEvents();

    // Launch accuracy evaluations periodically
    if (trainIndex % TEST_INTERVAL == 0)
      testEvaluator.test(trainIndex, net.get(), dataSets.get());
    if (trainIndex % TRAIN_TEST_INTERVAL == 0)
      trainEvaluator.test(trainIndex, net.get(), dataSets.get(), true);

    // Get training sample
    const auto &trainData = dataSets->trainData();
    Sample *sample = trainData[trainIndex % TRAIN_SET_SIZE].get();

    // Show sample image periodically
    if (trainIndex % UI_UPDATE_INTERVAL == 0) {
      ui->labelSampleImage->setPixmap(
          QPixmap::fromImage(sample->image->scaled(280, 280)));
      ui->labelSampleImage->update();
      QApplication::processEvents();
    }

    // Forward pass
    net->feedForward(sample->pixels.get());
    net->getResults(values);

    // Find predicted digit
    int predicted = -1;
    double maxVal = 0.0;
    for (int i = 0; i < NUM_DIGITS; i++) {
      if (values[i] > maxVal) {
        maxVal = values[i];
        predicted = i;
      }
    }

    // Create target vector (one-hot with 0.99 / 0.01)
    for (int i = 0; i < NUM_DIGITS; i++)
      values[i] = (static_cast<unsigned>(i) == sample->label) ? 0.99 : 0.01;

    // Backpropagation
    double eta = ui->doubleSpinBoxLearnRate->value();
    double alpha = ui->doubleSpinBoxMomentum->value();
    int batchSize = ui->spinBoxBatchSize->value();
    net->backProp(values, eta, alpha, batchSize > 0);

    if (batchSize > 0 && trainIndex % static_cast<unsigned>(batchSize) == 0)
      net->applyBatch();

    // Update accuracy display and chart
    if (trainIndex % CHART_UPDATE_INTERVAL == 0) {
      // Plot all newly finished test workers
      const auto &testWorkers = testEvaluator.workers();
      for (size_t i = lastPlottedTestIdx; i < testWorkers.size(); ++i) {
        if (!testWorkers[i]->isRunning()) {
          double accuracy = 100.0 - testWorkers[i]->getErrorRate() * 100.0;
          addTestAccuracyPoint(testWorkers[i]->getId(), accuracy);
          ui->labelAccuracy->setText(QString::number(accuracy, 'f', 2) + "%");
          lastPlottedTestIdx = i + 1;
        } else {
          break; // earlier workers must finish before later ones
        }
      }
      // Plot all newly finished train workers
      const auto &trainWorkers = trainEvaluator.workers();
      for (size_t i = lastPlottedTrainIdx; i < trainWorkers.size(); ++i) {
        if (!trainWorkers[i]->isRunning()) {
          double accuracy = 100.0 - trainWorkers[i]->getErrorRate() * 100.0;
          addTrainAccuracyPoint(trainWorkers[i]->getId(), accuracy);
          lastPlottedTrainIdx = i + 1;
        } else {
          break;
        }
      }
      QApplication::processEvents();
    }

    // Update prediction display
    if (trainIndex % UI_UPDATE_INTERVAL == 0) {
      ui->labelPredicted->setText(QString::number(predicted));
      ui->labelCorrect->setText(QString::number(sample->label));
      QApplication::processEvents();
    }

    QApplication::processEvents();
  }
}

// ---------------------------------------------------------------------------
// Save / Load
// ---------------------------------------------------------------------------

void MainWindow::on_pushButton_save_clicked() { net->saveTo("mynet.csv"); }

void MainWindow::on_pushButton_load_clicked() { net->loadFrom("mynet.csv"); }

void MainWindow::on_pushButton_stop_clicked() {
  running = false;
  testEvaluator.cancelAll();
  trainEvaluator.cancelAll();
}

void MainWindow::closeEvent(QCloseEvent *event) {
  running = false;
  testEvaluator.cancelAll();
  trainEvaluator.cancelAll();
  if (this->running) {
    net->saveTo("mynet.csv");
    std::cout << "Auto-saved to mynet.csv" << std::endl;
  }
  QMainWindow::closeEvent(event);
}
