#include "datasetloader.h"

#include <cstdlib>
#include <ctime>

#include "qdebug.h"
#include <QDebug>
#include <fstream>

#include "backproptrainer.cpp"


Data::Data(const int & numCols, const int & numRows)
{
    img_d = new double[numCols * numRows];
    img = new QImage(numCols, numRows, QImage::Format_Grayscale8);
    num = 0;

}

Data::~Data()
{
    delete[] img_d;
    delete img;
}


DataSetLoader::DataSetLoader()
{
    // Initialize random seed
    std::srand(std::time(0));
    m_trainData = new std::vector<Data *>();
    m_testData = new std::vector<Data *>();

}

DataSetLoader::~DataSetLoader()
{
    for(auto data : *trainData())
        delete data;
    for(auto data : *testData())
        delete data;


    delete m_trainData;
    delete m_testData;
}


bool DataSetLoader::loadSets(QString dirpath)
{
    QString trainImagesPath = dirpath + "/train-images.idx3-ubyte";
    QString trainLabelsPath = dirpath + "/train-labels.idx1-ubyte";
    QString testImagesPath = dirpath + "/t10k-images.idx3-ubyte";
    QString testLabelsPath = dirpath + "/t10k-labels.idx1-ubyte";

    QFile trainImagesFile(trainImagesPath);
    QFile trainLabelsFile(trainLabelsPath);
    QFile testImagesFile(testImagesPath);
    QFile testLabelsFile(testLabelsPath);

    if (trainImagesFile.open(QIODevice::ReadOnly) && trainLabelsFile.open(QIODevice::ReadOnly) &&
        testImagesFile.open(QIODevice::ReadOnly) && testLabelsFile.open(QIODevice::ReadOnly)) {

        readImages(trainImagesFile, m_trainData);
        readLabels(trainLabelsFile, m_trainData);
        readImages(testImagesFile, m_testData);
        readLabels(testLabelsFile, m_testData);

        trainImagesFile.close();
        trainLabelsFile.close();
        testImagesFile.close();
        testLabelsFile.close();
    } else {
        qDebug() << ("Open file failed: " + trainImagesFile.errorString());
        return false;
    }
    return true;
}

std::vector<Data *> *DataSetLoader::trainData() const
{
    return m_trainData;
}

std::vector<Data *> *DataSetLoader::testData() const
{
    return m_testData;
}

Data *DataSetLoader::randomTrainData(int min, int max)
{
    if (max <= -1)
        max = m_trainData->size();  // Wenn max -1 ist, setzen wir max auf die Größe des Vektors

    if (min < 0 || min >= (int)m_trainData->size() || max <= min) {
        return nullptr;  // Ungültige Argumente
    }

    int randomIndex = std::rand() % (max - min) + min;  // Zufälliger Index zwischen min und max

    return m_trainData->at(randomIndex);

}

void DataSetLoader::readImages(QFile& file, std::vector<Data *>* data)
{
    QDataStream in(&file);
    in.setByteOrder(QDataStream::BigEndian);

    quint32 magicNumber;
    in >> magicNumber;
    quint32 numImages;
    in >> numImages;
    quint32 numRows;
    in >> numRows;
    quint32 numCols;
    in >> numCols;

    qDebug( ) << "magicNumber" << magicNumber << " numImages: " << numImages << " -> " << numRows << " * " << numCols;

    for(unsigned int i = 0; i < numImages; ++i)
    {
        Data * newData = new Data(numCols, numRows);

        for(unsigned int y = 0; y < numRows; ++y)
        {
            for(unsigned int x = 0; x < numCols; ++x)
            {
                quint8 pixelValue;
                in >> pixelValue;

                newData->img_d[y * 28 + x] = static_cast<double>(pixelValue) / 255.0;
                pixelValue = 255 - pixelValue;
                newData->img->setPixel(x, y, qRgb(pixelValue, pixelValue, pixelValue));
            }
        }

        data->push_back(newData);
    }

}

void DataSetLoader::readLabels(QFile& file, std::vector<Data *> *data)
{
    QDataStream in(&file);
    in.setByteOrder(QDataStream::BigEndian);

    quint32 magicNumber;
    in >> magicNumber;
    quint32 numLabels;
    in >> numLabels;

    for(unsigned int i = 0; i < numLabels; ++i)
    {
        quint8 label;
        in >> label;
        data->at(i)->num = label;
    }
}




