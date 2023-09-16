#ifndef DATASETLOADER_H
#define DATASETLOADER_H

#include <QString>
#include <QImage>
#include <QFile>
#include <vector>
#include <ctime>


struct Data
{
    Data(const int &numCols, const int &numRows);

    ~Data();


    QImage * img;
    double * img_d;
    unsigned num;
};


class DataSetLoader
{
public:
    DataSetLoader();
    ~DataSetLoader();

    void loadSets(QString dirpath);

    std::vector<Data *> trainData() const;
    std::vector<Data *> testData() const;

    Data *randomTrainData(int min = 0, int max = -1);

private:
    std::vector<Data *> m_trainData;
    std::vector<Data *> m_testData;

    void readImages(QFile& file, std::vector<Data *>& data);
    void readLabels(QFile& file, std::vector<Data *>& data);
    unsigned char readByte(QFile& file);
    unsigned int readInt32(QFile& file);

};

#endif // DATASETLOADER_H
