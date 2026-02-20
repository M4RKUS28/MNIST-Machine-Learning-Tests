#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "backproptrainer.h"
#include "datasetloader.h"
#include "net.h"

#include <QApplication>
#include <QPixmap>

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

  if (!dataSets->loadSets("./dataset")) {
    std::cerr << "Failed to load MNIST dataset from ./dataset" << std::endl;
  }
}

MainWindow::~MainWindow() {
  running = false;
  delete ui;
  // dataSets and net cleaned up by unique_ptr
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

void MainWindow::on_pushButtonStart_clicked() {
  if (running)
    return;
  running = true;

  double values[NUM_DIGITS];
  BackPropTrainer testEvaluator;
  BackPropTrainer trainEvaluator;

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

    // Update iteration counter in UI
    ui->label_iteration->setText(QString::number(trainIndex));
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
      ui->label->setPixmap(QPixmap::fromImage(sample->image->scaled(280, 280)));
      ui->label->update();
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
    double alpha = ui->doubleSpinBox_Monumentum->value();
    int batchSize = ui->spinBoxBatchSize->value();
    net->backProp(values, eta, alpha, batchSize > 0);

    if (batchSize > 0 && trainIndex % static_cast<unsigned>(batchSize) == 0)
      net->applyBatch();

    // Update accuracy display
    if (trainIndex % ERROR_DISPLAY_INTERVAL == 0) {
      const auto &workers = testEvaluator.workers();
      for (int i = static_cast<int>(workers.size()) - 1; i >= 0; --i) {
        if (!workers[i]->isRunning()) {
          double accuracy = 100.0 - workers[i]->getErrorRate() * 100.0;
          ui->label_errorrrate->setText(QString::number(accuracy, 'f', 2) +
                                        "%");
          break;
        }
      }
      QApplication::processEvents();
    }

    // Update prediction display
    if (trainIndex % UI_UPDATE_INTERVAL == 0) {
      ui->label_num->setText(QString::number(predicted));
      ui->label_num_cor->setText(QString::number(sample->label));
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

void MainWindow::on_pushButton_stop_clicked() { running = false; }
