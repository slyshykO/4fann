/*
* file   : netpanel.cpp created 2012.3.12
* class  : NetPanel
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "netpanel.hpp"
#include "ui_netpanel.h"

#include <doublefann.h>
#include <QFileDialog>
#include <QTextStream>

NetPanel::NetPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetPanel)
{
    ui->setupUi(this);
    initControls();
}

NetPanel::~NetPanel()
{
    delete ui;
}

QVector<uint> NetPanel::hiddenLayers()
{
    QVector<uint> res;
    for(int i = 0 ; i < ui->hiddenLayers->columnCount(); ++i)
        {
            QSpinBox * spin = qobject_cast<QSpinBox *>(ui->hiddenLayers->cellWidget(0, i));
            if(spin)
                res.append(spin->value());
        }
    return res;
}

uint NetPanel::numInput()
{
    return m_numInput;
}

uint NetPanel::numOutput()
{
    return m_numOutput;
}

uint NetPanel::netType()
{
    return ui->netTypeCombo->currentIndex();
}

uint NetPanel::stopFuncType()
{
    return ui->stopFuncCombo->currentIndex();
}

uint NetPanel::errorFunkType()
{
    return ui->errorFuncCombo->currentIndex();
}

uint NetPanel::traingMethod()
{
    return ui->trainMethodCombo->currentIndex();
}

uint NetPanel::activationHiddens()
{
    return ui->activationFuncHiddenCombo->currentIndex();
}

uint NetPanel::activationOutputs()
{
    return ui->activationFuncOutputCombo->currentIndex();
}

uint NetPanel::maxEpochs()
{
    return ui->epochSpin->value();
}

uint NetPanel::epochsBetweenReports()
{
    return ui->reportSpin->value();
}

float NetPanel::desiredError()
{
    return ui->desiredMSESpin->value();
}

double NetPanel::bitFailLimit()
{
    return ui->bitFailLimitSpin->value();
}

bool NetPanel::isNormalTrain()
{
    return ui->normalTrainRadioBtn->isChecked();
}

QString NetPanel::trainingFileName()
{
    return ui->trainEdit->text();
}

QString NetPanel::testFileName()
{
    return ui->testEdit->text();
}

QString NetPanel::saveDirName()
{
    return ui->saveDirEdit->text();
}

double NetPanel::connectionRate()
{
    return ui->connectionRateSpin->value();
}

double NetPanel::hiddenActivationSteppness()
{
    return ui->hiddenActivationSteppnessSpin->value();
}

double NetPanel::outputActivationSteppness()
{
    return ui->outputActivationStepnessSpin->value();
}

double NetPanel::quickPropDecayFactor()
{
    return ui->quickPropDecayFactorSpin->value();
}

double NetPanel::RPROPIncreaseFactor()
{
    return ui->RPROPIncreaseFactorSpin->value();
}

bool NetPanel::initWeights()
{
    return ui->initWeights->checkState();
}

double NetPanel::learningRate()
{
    return ui->learningRateSpin->value();
}

double NetPanel::quickPropMuFactor()
{
    return ui->quickPropMuFactorSpin->value();
}

double NetPanel::RPROPDecreaseFactor()
{
    return ui->RPROPDecreaseFactorSpin->value();
}

bool NetPanel::shuffleTrainData()
{
    return ui->shuffleTrainData->checkState();
}

double NetPanel::momentum()
{
    return ui->momentumSpin->value();
}

uint NetPanel::RPROPMinStep()
{
    return ui->RPROPMinStepSpin->value();
}

uint NetPanel::RPROPMaxStep()
{
    return ui->RPROPMaxStepSpin->value();
}

uint NetPanel::maxNumberOfNeurons()
{
    return ui->maxNumberOfNeuronsSpin->value();
}

double NetPanel::outputChangeFraction()
{
    return ui->outputChangeFractionSpin->value();
}

uint NetPanel::outputStagnationEpochs()
{
    return ui->outputStagnationEpochsSpin->value();
}

double NetPanel::weightMultiplier()
{
    return ui->weightMultiplierSpin->value();
}

double NetPanel::candidateChangeFraction()
{
    return ui->candidateChangeFractionSpin->value();
}

uint NetPanel::candidateStagnationEpochs()
{
    return ui->candidateStagnationEpochsSpin->value();
}

uint NetPanel::candidateLimit()
{
    return ui->candidateLimitSpin->value();
}

uint NetPanel::maximumOutEpochs()
{
    return ui->maximumOutEpochsSpin->value();
}

uint NetPanel::maximumCandidateEpochs()
{
    return ui->maximumCandidateEpochsSpin->value();
}

uint NetPanel::numberOfCandidateGroups()
{
    return ui->numberOfCandidateGroupsSpin->value();
}


void NetPanel::openTrainFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(0,tr("Open trainibg data file"));
    ui->trainEdit->setText(fileName);
    if( !fileName.isNull() )
        {
            QVector<int> head = readTrainHead(fileName);
            if( head.size() == 3)
                {
                    ui->inpOutlabel->setText(QString(tr("Inputs : %1 ; Outputs : %2"))
                                             .arg(head[1]).arg(head[2]));
                    m_numInput = head[1];
                    m_numOutput = head[2];
                    if(ui->hiddenLayers->columnCount() == 0)
                        addHidden(head[1]);
                }
        }

}

QVector<int> NetPanel::readTrainHead(const QString &fn)
{
    QVector<int> res;
    QFile file(fn);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream text( &file );
            QString head = text.readLine();
            QRegExp re_int("[0-9]+");
            int pos = 0;
            while((pos = re_int.indexIn(head, pos)) != -1)
                {
                    int tmp = re_int.cap(0).toInt();
                    res.append(tmp);
                    pos += re_int.matchedLength();
                }
        }
    return res;
}

void NetPanel::openTestFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(0,tr("Open test data file"));
    ui->testEdit->setText(fileName);
}

void NetPanel::openSaveDir()
{
    QString fileName;
    fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->saveDirEdit->setText(fileName);
}

void NetPanel::hiddenLayerChanged(int i)
{
    if( i == ui->hiddenLayers->columnCount()) return;
    if( i > ui->hiddenLayers->columnCount() )
        {
            int res = i - ui->hiddenLayers->columnCount();
            for(int it = 0; it < res; ++it)
                {
                    addHidden();
                }
        }
    else
        {
            ui->hiddenLayers->setColumnCount(i);
        }
}

void NetPanel::addHidden(int q)
{
    QSpinBox * spin = new QSpinBox();
    spin->setMaximum(10000);
    spin->setMinimumWidth(50);
    spin->setValue(q);
    if(ui->hiddenLayers->rowCount() == 0)
        ui->hiddenLayers->setRowCount(1);
    ui->hiddenLayers->setColumnCount(ui->hiddenLayers->columnCount()+1);
    ui->hiddenLayers->setCellWidget(0,ui->hiddenLayers->columnCount()-1,spin);
}

void NetPanel::initControls()
{
    for(int i = 0; i < 2; ++i)
        {
            ui->netTypeCombo->addItem(tr(FANN_NETTYPE_NAMES[i]));
        }
    for(int i = 0; i < 2; ++i)
        {
            ui->errorFuncCombo->addItem(tr(FANN_ERRORFUNC_NAMES[i]));

        }
    for(int i = 0; i < 2; ++i)
        {
            ui->stopFuncCombo->addItem(tr(FANN_STOPFUNC_NAMES[i]));
        }
    for(int i = 0; i < 18; ++i)
        {
            ui->activationFuncHiddenCombo->addItem(tr(FANN_ACTIVATIONFUNC_NAMES[i]));
            ui->activationFuncOutputCombo->addItem(tr(FANN_ACTIVATIONFUNC_NAMES[i]));
        }
    ui->activationFuncHiddenCombo->setCurrentIndex(static_cast<int>(FANN_SIGMOID));
    ui->activationFuncOutputCombo->setCurrentIndex(static_cast<int>(FANN_SIGMOID));
    for(int i = 0; i < 5; ++i)
        {
            ui->trainMethodCombo->addItem(tr(FANN_TRAIN_NAMES[i]));
        }
    ui->trainMethodCombo->setCurrentIndex(static_cast<int>(FANN_TRAIN_RPROP));

    connect(ui->openTrainBtn, SIGNAL(clicked()), this, SLOT(openTrainFile()) );
    connect(ui->openTestBtn , SIGNAL(clicked()), this, SLOT(openTestFile()) );
    connect(ui->saveDirBtn  , SIGNAL(clicked()), this, SLOT(openSaveDir()) );

    connect(ui->hiddenSpin, SIGNAL(valueChanged(int)), this, SLOT(hiddenLayerChanged(int)) );

}


