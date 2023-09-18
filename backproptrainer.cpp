#include "backproptrainer.h"


BackPropTrainer::BackPropTrainer()
{

}

BackPropTrainer::~BackPropTrainer()
{

}

bool BackPropTrainerWorker::start(DataSetLoader *dataSets)
{
    if(is_running)
        return false;

    std::lock_guard<std::mutex> locker(obj_lock);
    error_rate = 0.0;
    try {
        m_thread = std::thread(run, this, dataSets, useTrainData);
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }

    is_running = true;
    return true;
}

double BackPropTrainerWorker::getErrorRate()
{
    std::lock_guard<std::mutex> locker(obj_lock);
    return error_rate;
}

bool BackPropTrainerWorker::isRunning()
{
    std::lock_guard<std::mutex> locker(obj_lock);
    return is_running;
}


bool BackPropTrainer::test(int id, Net *net, DataSetLoader * dataSets, bool useTrainData)
{
    auto worker = new BackPropTrainerWorker(id, net, useTrainData);
    this->m_workers.push_back(worker);
    return worker->start(dataSets);
}

int BackPropTrainer::isOverfitting()
{
    double error_rate = 999.9;
    int overfitting_score = 0;

    for (unsigned i = 0; i < this->m_workers.size(); ++i) {
        if(!m_workers.at(i)->isRunning()) {
            if(m_workers.at(i)->getErrorRate() > error_rate) {
                overfitting_score += 1;
            }
            error_rate = m_workers.at(i)->getErrorRate();
        }
    }

    return overfitting_score;
}




void BackPropTrainerWorker::run(BackPropTrainerWorker *_this, DataSetLoader *dataSets, bool useTrainData)
{


    double error_rate = 0.0;
    double output[10];

    ;

    for (unsigned i = 0; i < (useTrainData ? dataSets->trainData() : dataSets->testData())->size(); ++i) {
//        std::cout << _this->id << ": Check: " << i << " / " << dataSets->testData().size() << " -> " << error_rate << std::endl;

        double output_max = -1.0;
        unsigned output_max_num = 999;

        _this->getNet_copy()->feedForward((useTrainData ? dataSets->trainData() : dataSets->testData())->at(i)->img_d);
        _this->getNet_copy()->getResults(output);
        for(int o = 0; o < 10; o++) {
            if(output[o] > output_max) {
                output_max = output[o];
                output_max_num = o;
            }
        }
        if(output_max_num != (useTrainData ? dataSets->trainData() : dataSets->testData())->at(i)->num) {
//            std::cout << "  ---> IN: " << dataSets->testData().at(i)->num << " ist zu " << output_max << " nummer " << output_max_num << std::endl;

            error_rate += 1.0;
        } else {
//            std::cout << "IN: " << dataSets->testData().at(i)->num << " ist zu " << output_max << " nummer " << output_max_num << std::endl;
        }
    }
    error_rate /= (double)(useTrainData ? dataSets->trainData() : dataSets->testData())->size();
    _this->setErrorRate(error_rate);
    _this->setRunningStatus(false);

    std::cout << ((useTrainData) ? " -> [TRAIN]: " : " => ") << "   " << _this->id << ": -> acc: " << (1.0 - error_rate) * 100.0  << " %" << std::endl;
    QFile out(((useTrainData) ? "train_values.txt" : "test_values.txt"));
    out.open(QFile::OpenMode::enum_type::ReadWrite | QFile::OpenMode::enum_type::Append);
    out.write( (QString((useTrainData) ? " -> [TRAIN]: " : " => ") + "   " + QString::number( _this->id ) + ": -> acc: " + QString::number((1.0 - error_rate) * 100.0)  + " %\n").toStdString().c_str());
    out.close();

}

Net *BackPropTrainerWorker::getNet_copy()
{
    std::lock_guard<std::mutex> locker(obj_lock);
    return net_copy;
}

void BackPropTrainerWorker::setErrorRate(const double &value)
{
    std::lock_guard<std::mutex> locker(obj_lock);
    error_rate = value;
}

void BackPropTrainerWorker::setRunningStatus(const bool &isRunning)
{
    std::lock_guard<std::mutex> locker(obj_lock);
    is_running = isRunning;
}


BackPropTrainerWorker::BackPropTrainerWorker(const int &id, const Net *net, bool useTrainData)
    : id(id), error_rate(0.0), is_running(false), net_copy(new Net(net)), useTrainData(useTrainData)
{

}

BackPropTrainerWorker::~BackPropTrainerWorker()
{
    delete net_copy;
}

