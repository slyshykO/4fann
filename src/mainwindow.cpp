/*
* file   : mainwindow.cpp created 2012.3.11
* class  : MainWindow
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "mainwindow.hpp"
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDebug>

#include "netpanel.hpp"
#include "trainingdata.hpp"
#include "start_train_ann_dialog.hpp"
#include "nettrainview.hpp"
#include "thread_count_change_dialog.hpp"

#include <QtConcurrentRun>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),m_currentProject(0)
{
    m_tabs = new QTabWidget(this);
    setCentralWidget(m_tabs);
    resize(1000,700);
    createActions();

    m_taskList = new QListWidget();
    m_taskDock = new QDockWidget(tr("Training list"));
    m_taskDock->setWidget(m_taskList);
    addDockWidget(Qt::LeftDockWidgetArea, m_taskDock, Qt::Vertical);

    m_startDialog = new StartTrainAnnDialog();

    m_currentProject = new NetProject(this);
    connect(m_currentProject, SIGNAL(projectChanged()), this, SLOT(projectChanged()) );

}

void MainWindow::createActions()
{
    QIcon icon;

    m_startTrainAct = new QAction(tr("Add training"), this);
    icon.addPixmap(QPixmap(tr(":/image/add.png")), QIcon::Normal, QIcon::Off);
    m_startTrainAct->setIcon(icon);
    connect(m_startTrainAct, SIGNAL(triggered()), this, SLOT(startTraining()));

    m_changeMaxThreadAct =  new QAction(tr("Change thread quantity"), this);
    icon.addPixmap(QPixmap(tr(":/image/screwdriver.png")), QIcon::Normal, QIcon::Off);
    m_changeMaxThreadAct->setIcon(icon);
    connect(m_changeMaxThreadAct, SIGNAL(triggered()), this, SLOT(changeMaxThreadCount()));


    m_trainBar = new QToolBar(this);
    m_trainBar->addAction(m_startTrainAct);
    m_trainBar->addAction(m_changeMaxThreadAct);
    addToolBar(Qt::TopToolBarArea, m_trainBar);
}

void MainWindow::changeMaxThreadCount()
{
    qDebug()<<__FUNCTION__;
    ThreadCountChangeDialog dialog;
    if(dialog.exec())
        {
            QThreadPool::globalInstance()->setMaxThreadCount(dialog.newThreadCount());
        }
}

void MainWindow::newTrening()
{
    NetPanel * panel = new NetPanel();
    m_tabs->addTab(panel,tr("Новая тренировка *"));
}

void MainWindow::startTraining()
{
    //StartTrainAnnDialog dialog;
    if( m_startDialog->exec())
        {
            NetPanel * panel = m_startDialog->panel();
            if ( panel )
                {
                    NeuralNetwork * net = new NeuralNetwork();
                    net->setNetType(panel->netType());
                    net->setNumInput(panel->numInput());
                    net->setNumOutput(panel->numOutput());
                    net->setNumNeuronsHidden(panel->hiddenLayers());

                    net->setTrainAlgorithm(panel->traingMethod());
                    net->setDesiredError(panel->desiredError());

                    net->setActivationHidden(panel->activationHiddens());
                    net->setActivationOutput(panel->activationOutputs());
                    net->setMaxEpochs(panel->maxEpochs());
                    net->setEpochsBetweenReports(panel->epochsBetweenReports());

                    net->setTrainErrorFunc(panel->errorFunkType());
                    net->setBitFailLimit(panel->bitFailLimit());


                    net->setAnnDirDefinition(panel->saveDirName());

                    net->setHiddenActivationSteppness(panel->hiddenActivationSteppness());
                    net->setOutputActivationSteppness(panel->outputActivationSteppness());
                    net->setQuickPropDecayFactor(panel->quickPropDecayFactor());
                    net->setRPROPIncreaseFactor(panel->RPROPIncreaseFactor());
                    net->setInitWeights(panel->initWeights());
                    net->setQuickPropMuFactor(panel->quickPropMuFactor());
                    net->setRPROPDecreaseFactor(panel->RPROPDecreaseFactor());
                    net->setShuffleTrainData(panel->shuffleTrainData());
                    net->setMomentum(panel->momentum());
                    net->setRPROPMinStep(panel->RPROPMinStep());
                    net->setRPROPMaxStep(panel->RPROPMaxStep());
                    net->setMaxNumberOfNeurons(panel->maxNumberOfNeurons());
                    net->setOutputChangeFraction(panel->outputChangeFraction());
                    net->setOutputStagnationEpochs(panel->outputStagnationEpochs());
                    net->setWeightMultiplier(panel->weightMultiplier());
                    net->setCandidateChangeFraction(panel->candidateChangeFraction());
                    net->setCandidateStagnationEpochs(panel->candidateStagnationEpochs());
                    net->setCandidateLimit(panel->candidateLimit());
                    net->setMaximumOutEpochs(panel->maximumOutEpochs());
                    net->setMaximumCandidateEpochs(panel->maximumCandidateEpochs());
                    net->setnumberOfCandidateGroups(panel->numberOfCandidateGroups());

                    net->setTestFileName(panel->testFileName());

                    TrainingData * trainingData = new TrainingData();
                    qDebug()<<"create data";

                    if ( net->initAnn() )
                        {
                            m_currentProject->addNet(net);

                            if ( trainingData->loadFromFile( panel->trainingFileName() ) )
                                {
                                    QFuture<void> train;
                                    if (panel->isNormalTrain())
                                        train = QtConcurrent::run(net,
                                                                  &NeuralNetwork::trainOnData,
                                                                  *trainingData);
                                    else
                                        train = QtConcurrent::run(net,
                                                                  &NeuralNetwork::cascadeTrainOnData,
                                                                  *trainingData);
                                    net->setFuture(train);
                                }
                        }
                }
        }
    m_startDialog->hide();
}

void MainWindow::projectChanged()
{
    if( m_taskList->count() <= m_currentProject->nets()->size() )
        {
            for( int it = 0; it < m_currentProject->nets()->size() ; ++it )
                {
                    QListWidgetItem * item;
                    if( it < m_taskList->count() )
                        {
                            item = m_taskList->item(it);
                        }
                    else
                        {
                            item = new QListWidgetItem();
                            m_taskList->addItem(item);
                        }

                    item->setText(m_currentProject->nets()->at(it)->fileName());
                    switch(m_currentProject->nets()->at(it)->state())
                        {
                        case NeuralNetwork::WaitTraining :
                            item->setIcon(QIcon(":/image/cigarette.png"));
                            break;
                        case NeuralNetwork::Training :
                            item->setIcon(QIcon(":/image/factory.png"));
                            break;
                        case NeuralNetwork::Ready :
                            item->setIcon(QIcon(":/image/lollipop.png"));
                            break;
                        }

                    NetTrainView * trainView;
                    if( it < m_tabs->count() )
                        {
                            trainView = qobject_cast<NetTrainView *>( m_tabs->widget(it));
                        }
                    else
                        {
                            trainView = new NetTrainView();
                            m_tabs->addTab(trainView,m_currentProject->nets()->at(it)->fileName());
                        }
                    if( trainView )
                        {
                            trainView->setNetText(m_currentProject->nets()->at(it)->toString());
                            trainView->setMSEData(m_currentProject->nets()->at(it)->mseHistory(),
                                                  m_currentProject->nets()->at(it)->epochsHistory());
                            trainView->setBitFailData(m_currentProject->nets()->at(it)->bitFailHistory(),
                                                      m_currentProject->nets()->at(it)->epochsHistory());
                            trainView->setTestMSEData(m_currentProject->nets()->at(it)->mseTestHistory(),
                                                      m_currentProject->nets()->at(it)->epochsHistory());
                            trainView->setTestBitFailData(m_currentProject->nets()->at(it)->bitFailTestHistory(),
                                                      m_currentProject->nets()->at(it)->epochsHistory());
                        }
                }
        }
}


