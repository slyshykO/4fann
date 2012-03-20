/*
* file   : mainwindow.hpp created 2012.3.11
* class  : MainWindow
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTableWidget>

#include "neuralnetwork.hpp"
#include "netproject.hpp"

QT_FORWARD_DECLARE_CLASS(StartTrainAnnDialog)

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
private slots:
    void newTrening();
    void startTraining();
    void projectChanged();
    void changeMaxThreadCount();
private:
    void createActions();
private:
    QTabWidget * m_tabs;
    QList<NeuralNetwork *> m_nets;
    NetProject * m_currentProject;

    QToolBar * m_trainBar;
    QAction  * m_addTrainAct;
    QAction  * m_startTrainAct;
    QAction  * m_changeMaxThreadAct;

    QListWidget * m_taskList;
    QDockWidget * m_taskDock;

    StartTrainAnnDialog * m_startDialog;

};

#endif // MAINWINDOW_HPP
