#ifndef TRAININGWORKER_H
#define TRAININGWORKER_H

#include <QImage>
#include <QObject>

#include <atomic>

#include "backproptrainer.h"

class Net;
class DataSetLoader;

/**
 * @brief Runs the training loop in a background QThread.
 *
 * Communicates with the GUI thread exclusively through queued signals.
 * Hyperparameters (learning rate, momentum, batch size) can be changed
 * at any time via the atomic setters.
 */
class TrainingWorker : public QObject {
  Q_OBJECT

public:
  TrainingWorker(Net *net, DataSetLoader *dataSets, unsigned startIndex,
                 double learningRate, double momentum, int batchSize,
                 QObject *parent = nullptr);
  ~TrainingWorker() override = default;

  /// Request the worker to stop after the current iteration.
  void stop();

  // Thread-safe parameter setters (atomic)
  void setLearningRate(double lr);
  void setMomentum(double m);
  void setBatchSize(int bs);

signals:
  void iterationUpdated(unsigned iter, unsigned epoch);
  void sampleReady(QImage image, int predicted, unsigned label);
  void testAccuracyReady(double iteration, double accuracy);
  void trainAccuracyReady(double iteration, double accuracy);
  void trainingFinished(unsigned lastTrainIndex);

public slots:
  void run();

private:
  Net *m_net;
  DataSetLoader *m_dataSets;
  unsigned m_startIndex;

  std::atomic<bool> m_stop{false};
  std::atomic<double> m_learningRate;
  std::atomic<double> m_momentum;
  std::atomic<int> m_batchSize;

  BackPropTrainer m_testEvaluator;
  BackPropTrainer m_trainEvaluator;
};

#endif // TRAININGWORKER_H
