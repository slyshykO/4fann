/*
* file   : traindata.hpp created 2012.3.11
* class  : TrainData
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef TRAINDATA_HPP
#define TRAINDATA_HPP

#include <doublefann.h>
#include <QString>

class TrainingData
{
public:
    TrainingData();
    TrainingData(const TrainingData &data);

    TrainingData & operator= (const TrainingData &data);

    ~TrainingData();

    bool isNull();
    struct fann_train_data* data();

    QString fileName();
    void setFileName(const QString & fn);
    bool loadFromFile(const QString & fn = QString());
private:
    void destroyData();
protected:
    /* The neural_net class has direct access to the training data */
    friend class NeuralNetwork;

    /* Pointer to the encapsulated training data */
    struct fann_train_data* m_train_data;
    QString m_fileName;
};

#endif // TRAINDATA_HPP
