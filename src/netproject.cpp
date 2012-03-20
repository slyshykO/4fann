/*
* file   : netproject.cpp created 2012.3.11
* class  : NetProject
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "netproject.hpp"
#include "neuralnetwork.hpp"

NetProject::NetProject(QObject *parent) :
    QObject(parent)
{
}

void NetProject::addNet(NeuralNetwork *net)
{
    m_nets.append(net);
    connect(net, SIGNAL(stateChanged(int)), this, SIGNAL(projectChanged()) );
    emit projectChanged();
}

QList<NeuralNetwork *> *NetProject::nets()
{
    return &m_nets;
}
