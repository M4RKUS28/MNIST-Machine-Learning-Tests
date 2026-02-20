#ifndef BACKPROPTRAINER_H
#define BACKPROPTRAINER_H



#include "net.h"
#include <mutex>
#include <thread>
#include "datasetloader.h"


class BackPropTrainerWorker
{
public:
    BackPropTrainerWorker(const int &id, const Net * net, bool useTrainData);
    ~BackPropTrainerWorker();

    bool start( DataSetLoader * dataSets);
    double getErrorRate();
    bool isRunning();
    void setErrorRate(const double &value);
    void setRunningStatus(const bool &isRunning);
    Net *getNet_copy();

    int id;
private:
    static void run(BackPropTrainerWorker *_this, DataSetLoader * dataSets, bool useTrainData);
    double error_rate;
    bool is_running;
    std::thread m_thread;
    std::mutex obj_lock;
    Net * net_copy;
    bool useTrainData;
};



class BackPropTrainer
{
public:
    BackPropTrainer();
    ~BackPropTrainer();

    bool test(int id, Net * net, DataSetLoader * dataSets, bool useTrainData = false);
    int isOverfitting();
    std::vector<BackPropTrainerWorker *> m_workers;

private:


};

#endif // BACKPROPTRAINER_H
