#ifndef BACKPROPTRAINER_H
#define BACKPROPTRAINER_H

#include "datasetloader.h"
#include "net.h"

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

/**
 * @brief Worker that evaluates a network copy on a dataset in a background
 * thread.
 *
 * Each worker owns a copy of the network and computes classification
 * accuracy on either the training or test set.
 */
class BackPropTrainerWorker {
public:
  BackPropTrainerWorker(int id, const Net *net, bool useTrainData);
  ~BackPropTrainerWorker();

  // Non-copyable (owns thread + net copy)
  BackPropTrainerWorker(const BackPropTrainerWorker &) = delete;
  BackPropTrainerWorker &operator=(const BackPropTrainerWorker &) = delete;

  bool start(DataSetLoader *dataSets);

  double getErrorRate() const;
  bool isRunning() const;

  int getId() const { return m_id; }
  Net *getNetCopy();

private:
  void setErrorRate(double value);
  void setRunningStatus(bool running);

  static void run(BackPropTrainerWorker *self, DataSetLoader *dataSets,
                  bool useTrainData);

  int m_id;
  double m_errorRate = 0.0;
  bool m_running = false;
  bool m_useTrainData;
  std::thread m_thread;
  mutable std::mutex m_mutex;
  std::unique_ptr<Net> m_netCopy;
};

/**
 * @brief Manages multiple BackPropTrainerWorker instances for parallel
 * evaluation.
 */
class BackPropTrainer {
public:
  BackPropTrainer() = default;
  ~BackPropTrainer() = default;

  bool test(int id, Net *net, DataSetLoader *dataSets,
            bool useTrainData = false);

  /**
   * @brief Heuristic overfitting check by counting sequential error increases.
   */
  int isOverfitting() const;

  const std::vector<std::unique_ptr<BackPropTrainerWorker>> &workers() const {
    return m_workers;
  }

private:
  std::vector<std::unique_ptr<BackPropTrainerWorker>> m_workers;
};

#endif // BACKPROPTRAINER_H
