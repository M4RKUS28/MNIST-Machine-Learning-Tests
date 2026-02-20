#ifndef DATASETLOADER_H
#define DATASETLOADER_H

#include <QFile>
#include <QImage>
#include <QString>

#include <memory>
#include <random>
#include <vector>

/**
 * @brief A single MNIST sample: image data + label.
 */
struct Sample {
  Sample(unsigned cols, unsigned rows);
  ~Sample() = default;

  // Non-copyable (owns image data)
  Sample(const Sample &) = delete;
  Sample &operator=(const Sample &) = delete;

  std::unique_ptr<double[]> pixels;
  std::unique_ptr<QImage> image;
  unsigned label = 0;
  unsigned width = 0;
  unsigned height = 0;
};

/**
 * @brief Loads MNIST training and test datasets from IDX binary files.
 */
class DataSetLoader {
public:
  DataSetLoader();
  ~DataSetLoader() = default;

  bool loadSets(const QString &dirPath);

  const std::vector<std::unique_ptr<Sample>> &trainData() const {
    return m_trainData;
  }
  const std::vector<std::unique_ptr<Sample>> &testData() const {
    return m_testData;
  }

  Sample *randomTrainSample(int minIdx = 0, int maxIdx = -1);

private:
  void readImages(QFile &file, std::vector<std::unique_ptr<Sample>> &data);
  void readLabels(QFile &file, std::vector<std::unique_ptr<Sample>> &data);

  std::vector<std::unique_ptr<Sample>> m_trainData;
  std::vector<std::unique_ptr<Sample>> m_testData;

  std::mt19937 rng{std::random_device{}()};
};

#endif // DATASETLOADER_H
