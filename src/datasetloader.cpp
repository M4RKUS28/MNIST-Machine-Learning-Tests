#include "datasetloader.h"

#include <QDataStream>
#include <QDebug>

// ---------------------------------------------------------------------------
// Sample
// ---------------------------------------------------------------------------

Sample::Sample(unsigned cols, unsigned rows)
    : pixels(std::make_unique<double[]>(cols * rows)),
      image(std::make_unique<QImage>(cols, rows, QImage::Format_Grayscale8)),
      width(cols), height(rows) {}

// ---------------------------------------------------------------------------
// DataSetLoader
// ---------------------------------------------------------------------------

DataSetLoader::DataSetLoader() = default;

bool DataSetLoader::loadSets(const QString &dirPath) {
  QFile trainImagesFile(dirPath + "/train-images.idx3-ubyte");
  QFile trainLabelsFile(dirPath + "/train-labels.idx1-ubyte");
  QFile testImagesFile(dirPath + "/t10k-images.idx3-ubyte");
  QFile testLabelsFile(dirPath + "/t10k-labels.idx1-ubyte");

  if (!trainImagesFile.open(QIODevice::ReadOnly) ||
      !trainLabelsFile.open(QIODevice::ReadOnly) ||
      !testImagesFile.open(QIODevice::ReadOnly) ||
      !testLabelsFile.open(QIODevice::ReadOnly)) {
    qDebug() << "Failed to open dataset files:"
             << trainImagesFile.errorString();
    return false;
  }

  readImages(trainImagesFile, m_trainData);
  readLabels(trainLabelsFile, m_trainData);
  readImages(testImagesFile, m_testData);
  readLabels(testLabelsFile, m_testData);

  trainImagesFile.close();
  trainLabelsFile.close();
  testImagesFile.close();
  testLabelsFile.close();

  return true;
}

Sample *DataSetLoader::randomTrainSample(int minIdx, int maxIdx) {
  if (m_trainData.empty())
    return nullptr;

  int maxBound = (maxIdx < 0) ? static_cast<int>(m_trainData.size()) : maxIdx;
  if (minIdx < 0 || minIdx >= static_cast<int>(m_trainData.size()) ||
      maxBound <= minIdx)
    return nullptr;

  std::uniform_int_distribution<int> dist(minIdx, maxBound - 1);
  return m_trainData[dist(rng)].get();
}

void DataSetLoader::readImages(QFile &file,
                               std::vector<std::unique_ptr<Sample>> &data) {
  QDataStream in(&file);
  in.setByteOrder(QDataStream::BigEndian);

  quint32 magicNumber, numImages, numRows, numCols;
  in >> magicNumber >> numImages >> numRows >> numCols;

  qDebug() << "Magic:" << magicNumber << "Images:" << numImages << "->"
           << numRows << "x" << numCols;

  for (quint32 i = 0; i < numImages; ++i) {
    auto sample = std::make_unique<Sample>(numCols, numRows);

    for (quint32 y = 0; y < numRows; ++y) {
      for (quint32 x = 0; x < numCols; ++x) {
        quint8 pixelValue;
        in >> pixelValue;

        sample->pixels[y * numCols + x] =
            static_cast<double>(pixelValue) / 255.0;
        quint8 inverted = 255 - pixelValue;
        sample->image->setPixel(x, y, qRgb(inverted, inverted, inverted));
      }
    }

    data.push_back(std::move(sample));
  }
}

void DataSetLoader::readLabels(QFile &file,
                               std::vector<std::unique_ptr<Sample>> &data) {
  QDataStream in(&file);
  in.setByteOrder(QDataStream::BigEndian);

  quint32 magicNumber, numLabels;
  in >> magicNumber >> numLabels;

  for (quint32 i = 0; i < numLabels; ++i) {
    quint8 label;
    in >> label;
    data[i]->label = label;
  }
}
