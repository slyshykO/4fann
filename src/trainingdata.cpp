/*
* file   : traindata.cpp created 2012.3.11
* class  : TrainData
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "trainingdata.hpp"
#include <QFile>
#include <QDebug>

TrainingData::TrainingData()
    :m_train_data(0)
{

}

TrainingData::TrainingData(const TrainingData &data)
    :m_train_data(0)
{
    destroyData();
    if (data.m_train_data != NULL)
    {
        m_train_data = fann_duplicate_train_data(data.m_train_data);
    }
    m_fileName = data.m_fileName;
}

TrainingData & TrainingData::operator= (const TrainingData &data)
{
    if( this != &data)
        {
            destroyData();
            if (data.m_train_data != NULL)
            {
                m_train_data = fann_duplicate_train_data(data.m_train_data);
            }
            m_fileName = data.m_fileName;
        }
    return *this;
}

TrainingData::~TrainingData()
{
    destroyData();
}

bool TrainingData::isNull()
{
    return (m_train_data == 0);
}

fann_train_data *TrainingData::data()
{
    return m_train_data;
}

void TrainingData::destroyData()
{
    if(m_train_data != 0)
        {
            fann_destroy_train(m_train_data);
            m_train_data = 0;
        }
}

QString TrainingData::fileName()
{
    return m_fileName;
}

void TrainingData::setFileName(const QString &fn)
{
    m_fileName = fn;
}

bool TrainingData::loadFromFile(const QString &fn)
{
    QString fileName;

    if(fn.isNull())
        fileName = m_fileName;
    else
        fileName = fn;

    destroyData();
    m_train_data = fann_read_train_from_file(fileName.toLocal8Bit().data());
    if( m_train_data != 0 )
        m_fileName = fileName;

    return (m_train_data != 0);
}

