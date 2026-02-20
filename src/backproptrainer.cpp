#include "backproptrainer.h"

#include <QFile>
#include <QString>

#include <iostream>

// ---------------------------------------------------------------------------
// BackPropTrainerWorker
// ---------------------------------------------------------------------------

BackPropTrainerWorker::BackPropTrainerWorker(int id, const Net *net,
                                             bool useTrainData)
    : m_id(id), m_useTrainData(useTrainData),
      m_netCopy(std::make_unique<Net>(net)) {}

BackPropTrainerWorker::~BackPropTrainerWorker() {
  if (m_thread.joinable())
    m_thread.join();
}

bool BackPropTrainerWorker::start(DataSetLoader *dataSets) {
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_running)
      return false;
    m_errorRate = 0.0;
  }

  try {
    m_thread = std::thread(run, this, dataSets, m_useTrainData);
  } catch (const std::exception &e) {
    std::cerr << "Thread start failed: " << e.what() << std::endl;
    return false;
  }

  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_running = true;
  }
  return true;
}

double BackPropTrainerWorker::getErrorRate() const {
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_errorRate;
}

bool BackPropTrainerWorker::isRunning() const {
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_running;
}

Net *BackPropTrainerWorker::getNetCopy() {
  std::lock_guard<std::mutex> lock(m_mutex);
  return m_netCopy.get();
}

void BackPropTrainerWorker::setErrorRate(double value) {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_errorRate = value;
}

void BackPropTrainerWorker::setRunningStatus(bool running) {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_running = running;
}

void BackPropTrainerWorker::run(BackPropTrainerWorker *self,
                                DataSetLoader *dataSets, bool useTrainData) {
  const auto &data =
      useTrainData ? dataSets->trainData() : dataSets->testData();
  const unsigned dataSize = static_cast<unsigned>(data.size());
  constexpr int NUM_OUTPUTS = 10;

  double errorCount = 0.0;
  double output[NUM_OUTPUTS];

  for (unsigned i = 0; i < dataSize; ++i) {
    self->getNetCopy()->feedForward(data[i]->pixels.get());
    self->getNetCopy()->getResults(output);

    // Find predicted digit
    unsigned predicted = 0;
    double maxVal = output[0];
    for (int o = 1; o < NUM_OUTPUTS; o++) {
      if (output[o] > maxVal) {
        maxVal = output[o];
        predicted = o;
      }
    }

    if (predicted != data[i]->label)
      errorCount += 1.0;
  }

  double errorRate = errorCount / dataSize;
  self->setErrorRate(errorRate);
  self->setRunningStatus(false);

  // Log results
  const char *tag = useTrainData ? " -> [TRAIN]: " : " => ";
  double accuracy = (1.0 - errorRate) * 100.0;
  std::cout << tag << self->getId() << ": acc: " << accuracy << " %"
            << std::endl;

  // Append to log file
  QString filename = useTrainData ? "train_values.txt" : "test_values.txt";
  QFile out(filename);
  if (out.open(QIODevice::ReadWrite | QIODevice::Append)) {
    out.write(QString("%1%2: acc: %3 %\n")
                  .arg(tag)
                  .arg(self->getId())
                  .arg(accuracy)
                  .toUtf8());
    out.close();
  }
}

// ---------------------------------------------------------------------------
// BackPropTrainer
// ---------------------------------------------------------------------------

bool BackPropTrainer::test(int id, Net *net, DataSetLoader *dataSets,
                           bool useTrainData) {
  auto worker = std::make_unique<BackPropTrainerWorker>(id, net, useTrainData);
  bool ok = worker->start(dataSets);
  m_workers.push_back(std::move(worker));
  return ok;
}

int BackPropTrainer::isOverfitting() const {
  double lastError = 999.9;
  int overfittingScore = 0;

  for (const auto &worker : m_workers) {
    if (!worker->isRunning()) {
      double error = worker->getErrorRate();
      if (error > lastError)
        overfittingScore++;
      lastError = error;
    }
  }

  return overfittingScore;
}
