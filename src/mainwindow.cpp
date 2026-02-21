#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "architecturedialog.h"
#include "datasetloader.h"
#include "dialogueber.h"
#include "net.h"
#include "trainingworker.h"

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

  if (!dataSets->loadSets(":/mnist")) {
    std::cerr << "Failed to load MNIST dataset from :/mnist" << std::endl;
  }

  setupChart();

  // Display network architecture (commas → newlines)
  ui->labelArchitecture->setText(
      QString::fromStdString(net->getTopologyStr()).replace(',', '\n'));

  // Setup Über dialog
  diaUber = new DialogUeber(
      QApplication::applicationDirPath() + "/../MNIST-NetMaintenanceTool.exe",
      "M$RKUS", "MNIST-Net", PROGRAM_VERSION, Qt::blue, this, false, true);
  diaUber->setPixmap(QPixmap(":/icons/appicon.ico").scaled(128, 128));
  diaUber->setContributorList({"Markus-Huber"});
  diaUber->setDescription("https://github.com/M4RKUS28/MNIST-NN",
                          QFile(":/docs/short_description.txt"),
                          "github.com/M4RKUS28/MNIST-NN");
  diaUber->setIssueWebsite("https://github.com/M4RKUS28/MNIST-NN/issues");
}

MainWindow::~MainWindow() {
  // Stop any running training
  if (m_trainWorker)
    m_trainWorker->stop();
  if (m_trainThread && m_trainThread->isRunning())
    m_trainThread->wait();
  delete m_trainWorker;
  m_trainWorker = nullptr;

  delete diaUber;
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
// Training — start / stop via worker thread
// ---------------------------------------------------------------------------

void MainWindow::on_pushButtonStartStop_clicked() {
  if (running) {
    // --- STOP pressed ---
    ui->pushButtonStartStop->setEnabled(false);
    ui->pushButtonStartStop->setText("Stopping...");
    if (m_trainWorker)
      m_trainWorker->stop();
    return;
  }

  // --- START pressed ---
  running = true;
  ui->pushButtonStartStop->setText("Stop");
  ui->pushButtonStartStop->setToolTip("Stop training after current iteration");
  ui->pushButtonEditArch->setEnabled(false);

  // Create worker and move to thread
  m_trainWorker = new TrainingWorker(net.get(), dataSets.get(), trainIndex,
                                     ui->doubleSpinBoxLearnRate->value(),
                                     ui->doubleSpinBoxMomentum->value(),
                                     ui->spinBoxBatchSize->value());

  m_trainThread = new QThread(this);
  m_trainWorker->moveToThread(m_trainThread);

  // Wire up signals
  connect(m_trainThread, &QThread::started, m_trainWorker,
          &TrainingWorker::run);

  connect(m_trainWorker, &TrainingWorker::iterationUpdated, this,
          &MainWindow::onIterationUpdated);
  connect(m_trainWorker, &TrainingWorker::sampleReady, this,
          &MainWindow::onSampleReady);
  connect(m_trainWorker, &TrainingWorker::testAccuracyReady, this,
          &MainWindow::onTestAccuracyReady);
  connect(m_trainWorker, &TrainingWorker::trainAccuracyReady, this,
          &MainWindow::onTrainAccuracyReady);
  connect(m_trainWorker, &TrainingWorker::trainingFinished, this,
          &MainWindow::onTrainingFinished);

  // Quit the thread event loop once run() finishes (direct so it works
  // even when the main event loop is blocked in wait())
  connect(m_trainWorker, &TrainingWorker::trainingFinished, m_trainThread,
          &QThread::quit, Qt::DirectConnection);

  m_trainThread->start();
}

// ---------------------------------------------------------------------------
// Worker signal handlers (run on GUI thread)
// ---------------------------------------------------------------------------

void MainWindow::onIterationUpdated(unsigned iter, unsigned epoch) {
  ui->labelIteration->setText(QString::number(iter));
  ui->labelEpoch->setText(QString::number(epoch));

  // Push current hyperparameter values to the worker (atomic setters)
  if (m_trainWorker) {
    m_trainWorker->setLearningRate(ui->doubleSpinBoxLearnRate->value());
    m_trainWorker->setMomentum(ui->doubleSpinBoxMomentum->value());
    m_trainWorker->setBatchSize(ui->spinBoxBatchSize->value());
  }
}

void MainWindow::onSampleReady(QImage image, int predicted, unsigned label) {
  ui->labelSampleImage->setPixmap(QPixmap::fromImage(image));
  ui->labelSampleImage->update();
  ui->labelPredicted->setText(QString::number(predicted));
  ui->labelCorrect->setText(QString::number(label));
}

void MainWindow::onTestAccuracyReady(double iteration, double accuracy) {
  addTestAccuracyPoint(iteration, accuracy);
  ui->labelTestAccuracy->setText(QString::number(accuracy, 'f', 2) + "%");
}

void MainWindow::onTrainAccuracyReady(double iteration, double accuracy) {
  addTrainAccuracyPoint(iteration, accuracy);
  ui->labelTrainAccuracy->setText(QString::number(accuracy, 'f', 2) + "%");
}

void MainWindow::onTrainingFinished(unsigned lastTrainIndex) {
  if (!m_trainThread)
    return; // Already cleaned up (e.g. by Reset)

  trainIndex = lastTrainIndex;
  cleanupTraining();

  running = false;
  ui->pushButtonStartStop->setText("Start");
  ui->pushButtonStartStop->setToolTip("Start training the neural network");
  ui->pushButtonStartStop->setEnabled(true);
  ui->pushButtonEditArch->setEnabled(true);
}

void MainWindow::cleanupTraining() {
  if (m_trainThread) {
    m_trainThread->wait();
    delete m_trainWorker;
    m_trainWorker = nullptr;
    m_trainThread->deleteLater();
    m_trainThread = nullptr;
  }
}

// ---------------------------------------------------------------------------
// Save / Load
// ---------------------------------------------------------------------------

void MainWindow::on_pushButton_save_clicked() { net->saveTo("mynet.csv"); }

void MainWindow::on_pushButton_load_clicked() { net->loadFrom("mynet.csv"); }

void MainWindow::on_pushButtonReset_clicked() {
  if (running) {
    // Stop training first and wait for thread to finish
    if (m_trainWorker)
      m_trainWorker->stop();
    if (m_trainThread && m_trainThread->isRunning())
      m_trainThread->wait();
    delete m_trainWorker;
    m_trainWorker = nullptr;
    delete m_trainThread;
    m_trainThread = nullptr;
    running = false;
  }

  // Recreate network with same topology (reinitialises weights)
  net = std::make_unique<Net>(net->getTopologyStr(), 0.025);

  // Reset chart
  testAccuracySeries->clear();
  trainAccuracySeries->clear();
  testAccuracySeries->append(0, 0);
  trainAccuracySeries->append(0, 0);
  axisX->setRange(0, 10000);

  // Reset counters
  trainIndex = 0;
  ui->labelIteration->setText("0");
  ui->labelEpoch->setText("0");
  ui->labelTestAccuracy->setText("-");
  ui->labelTrainAccuracy->setText("-");

  // Update architecture label
  ui->labelArchitecture->setText(
      QString::fromStdString(net->getTopologyStr()).replace(',', '\n'));

  ui->pushButtonStartStop->setText("Start");
  ui->pushButtonStartStop->setEnabled(true);
  ui->pushButtonEditArch->setEnabled(true);

  std::cout << "Network reset" << std::endl;
}

void MainWindow::on_pushButtonEditArch_clicked() {
  if (running)
    return;

  QString currentTopology = QString::fromStdString(net->getTopologyStr());
  ArchitectureDialog dlg(currentTopology, this);

  if (dlg.exec() == QDialog::Accepted) {
    QString newTopology = dlg.topologyString();
    std::cout << "New topology: " << newTopology.toStdString() << std::endl;

    // Recreate network with new topology
    net = std::make_unique<Net>(newTopology.toStdString(), 0.025);

    // Update label
    ui->labelArchitecture->setText(newTopology.replace(',', '\n'));

    // Reset chart
    testAccuracySeries->clear();
    trainAccuracySeries->clear();
    testAccuracySeries->append(0, 0);
    trainAccuracySeries->append(0, 0);
    axisX->setRange(0, 10000);

    // Reset training counter
    trainIndex = 0;
    ui->labelIteration->setText("0");
    ui->labelEpoch->setText("0");
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (m_trainWorker)
    m_trainWorker->stop();
  if (m_trainThread && m_trainThread->isRunning())
    m_trainThread->wait();
  QMainWindow::closeEvent(event);
}

void MainWindow::on_pushButton_ueber_clicked() { diaUber->show(); }
