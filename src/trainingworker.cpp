#include "trainingworker.h"

#include "datasetloader.h"
#include "net.h"

#include <QFile>
#include <QString>

#include <iostream>

// ---------------------------------------------------------------------------
// Constants (same as original mainwindow.cpp)
// ---------------------------------------------------------------------------

static constexpr int NUM_DIGITS = 10;
static constexpr unsigned TRAIN_SET_SIZE = 60000;
static constexpr unsigned EPOCH_LOG_INTERVAL = TRAIN_SET_SIZE;
static constexpr unsigned UI_UPDATE_INTERVAL = 500;
static constexpr unsigned TEST_INTERVAL = 500;
static constexpr unsigned TRAIN_TEST_INTERVAL = 3000;
static constexpr unsigned CHART_UPDATE_INTERVAL = 500;

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TrainingWorker::TrainingWorker(Net *net, DataSetLoader *dataSets,
                               unsigned startIndex, double learningRate,
                               double momentum, int batchSize, QObject *parent)
    : QObject(parent), m_net(net), m_dataSets(dataSets),
      m_startIndex(startIndex), m_learningRate(learningRate),
      m_momentum(momentum), m_batchSize(batchSize) {}

void TrainingWorker::stop() { m_stop.store(true); }

void TrainingWorker::setLearningRate(double lr) { m_learningRate.store(lr); }
void TrainingWorker::setMomentum(double m) { m_momentum.store(m); }
void TrainingWorker::setBatchSize(int bs) { m_batchSize.store(bs); }

// ---------------------------------------------------------------------------
// Training loop (runs in worker thread)
// ---------------------------------------------------------------------------

void TrainingWorker::run() {
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

  unsigned trainIndex = m_startIndex;

  for (; trainIndex < 3000000 && !m_stop.load(); trainIndex++) {
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

    // Emit iteration / epoch update
    if (trainIndex % UI_UPDATE_INTERVAL == 0) {
      emit iterationUpdated(trainIndex, trainIndex / EPOCH_LOG_INTERVAL);
    }

    // Launch accuracy evaluations periodically
    if (trainIndex % TEST_INTERVAL == 0)
      m_testEvaluator.test(trainIndex, m_net, m_dataSets);
    if (trainIndex % TRAIN_TEST_INTERVAL == 0)
      m_trainEvaluator.test(trainIndex, m_net, m_dataSets, true);

    // Get training sample
    const auto &trainData = m_dataSets->trainData();
    Sample *sample = trainData[trainIndex % TRAIN_SET_SIZE].get();

    // Forward pass
    m_net->feedForward(sample->pixels.get());
    m_net->getResults(values);

    // Find predicted digit
    int predicted = -1;
    double maxVal = 0.0;
    for (int i = 0; i < NUM_DIGITS; i++) {
      if (values[i] > maxVal) {
        maxVal = values[i];
        predicted = i;
      }
    }

    // Emit sample image periodically
    if (trainIndex % UI_UPDATE_INTERVAL == 0) {
      emit sampleReady(sample->image->scaled(280, 280), predicted,
                       sample->label);
    }

    // Create target vector (one-hot with 0.99 / 0.01)
    for (int i = 0; i < NUM_DIGITS; i++)
      values[i] = (static_cast<unsigned>(i) == sample->label) ? 0.99 : 0.01;

    // Backpropagation
    double eta = m_learningRate.load();
    double alpha = m_momentum.load();
    int batchSize = m_batchSize.load();
    m_net->backProp(values, eta, alpha, batchSize > 0);

    if (batchSize > 0 && trainIndex % static_cast<unsigned>(batchSize) == 0)
      m_net->applyBatch();

    // Poll finished accuracy workers and emit results
    if (trainIndex % CHART_UPDATE_INTERVAL == 0) {
      // Test accuracy
      const auto &testWorkers = m_testEvaluator.workers();
      for (size_t i = lastPlottedTestIdx; i < testWorkers.size(); ++i) {
        if (!testWorkers[i]->isRunning()) {
          double accuracy = 100.0 - testWorkers[i]->getErrorRate() * 100.0;
          emit testAccuracyReady(testWorkers[i]->getId(), accuracy);
          lastPlottedTestIdx = i + 1;
        } else {
          break;
        }
      }
      // Train accuracy
      const auto &trainWorkers = m_trainEvaluator.workers();
      for (size_t i = lastPlottedTrainIdx; i < trainWorkers.size(); ++i) {
        if (!trainWorkers[i]->isRunning()) {
          double accuracy = 100.0 - trainWorkers[i]->getErrorRate() * 100.0;
          emit trainAccuracyReady(trainWorkers[i]->getId(), accuracy);
          lastPlottedTrainIdx = i + 1;
        } else {
          break;
        }
      }
    }
  }

  // Cancel any still-running evaluators
  m_testEvaluator.cancelAll();
  m_trainEvaluator.cancelAll();

  emit trainingFinished(trainIndex);
}
