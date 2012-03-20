/*
* file   : netproject.hpp created 2012.3.11
* class  : NetProject
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef NETPROJECT_HPP
#define NETPROJECT_HPP

#include <QObject>
#include <QListWidget>

class NeuralNetwork;

class NetProject : public QObject
{
    Q_OBJECT
public:
    explicit NetProject(QObject *parent = 0);
    void addNet(NeuralNetwork * net);
    QList<NeuralNetwork *> * nets();
signals:
    void projectChanged();
private:
    QString m_proDir;//директория роекта
    QString m_proName;

    QList<NeuralNetwork *> m_nets;

};

#endif // NETPROJECT_HPP
