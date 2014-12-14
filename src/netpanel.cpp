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
#include <QSettings>
#include <QFileInfo>
#include <QDir>

NetPanel::NetPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetPanel),
    m_numInput(0),
    m_numOutput(0)
{
    ui->setupUi(this);
    loadSettings();
    initControls();
}

NetPanel::~NetPanel()
{
    saveSettings();
    delete ui;
}

void NetPanel::loadSettings()
{
    QSettings s;
    s.beginGroup("netpanel");
    last_opened_data_dir_ = s.value("last_opened_dir","./").toString();
    s.endGroup();
}

void NetPanel::saveSettings()
{
    QSettings s;
    s.beginGroup("netpanel");
    s.setValue("last_opened_dir",QVariant(last_opened_data_dir_));
    s.endGroup();
}

QVector<uint> NetPanel::hiddenLayers() const
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

uint NetPanel::numInput() const
{
    return m_numInput;
}

uint NetPanel::numOutput() const
{
    return m_numOutput;
}

uint NetPanel::netType() const
{
    return ui->netTypeCombo->currentIndex();
}

uint NetPanel::stopFuncType() const
{
    return ui->stopFuncCombo->currentIndex();
}

uint NetPanel::errorFunkType() const
{
    return ui->errorFuncCombo->currentIndex();
}

uint NetPanel::traingMethod() const
{
    return ui->trainMethodCombo->currentIndex();
}

uint NetPanel::activationHiddens() const
{
    return ui->activationFuncHiddenCombo->currentIndex();
}

uint NetPanel::activationOutputs() const
{
    return ui->activationFuncOutputCombo->currentIndex();
}

uint NetPanel::maxEpochs() const
{
    return ui->epochSpin->value();
}

uint NetPanel::epochsBetweenReports() const
{
    return ui->reportSpin->value();
}

float NetPanel::desiredError() const
{
    return ui->desiredMSESpin->value();
}

double NetPanel::bitFailLimit()  const
{
    return ui->bitFailLimitSpin->value();
}

bool NetPanel::isNormalTrain()  const
{
    return ui->normalTrainRadioBtn->isChecked();
}

QString NetPanel::trainingFileName() const
{
    return ui->trainEdit->text();
}

QString NetPanel::testFileName() const
{
    return ui->testEdit->text();
}

QString NetPanel::saveDirName() const
{
    return ui->saveDirEdit->text();
}

double NetPanel::connectionRate() const
{
    return ui->connectionRateSpin->value();
}

double NetPanel::hiddenActivationSteppness() const
{
    return ui->hiddenActivationSteppnessSpin->value();
}

double NetPanel::outputActivationSteppness() const
{
    return ui->outputActivationStepnessSpin->value();
}

double NetPanel::quickPropDecayFactor() const
{
    return ui->quickPropDecayFactorSpin->value();
}

double NetPanel::RPROPIncreaseFactor() const
{
    return ui->RPROPIncreaseFactorSpin->value();
}

bool NetPanel::initWeights() const
{
    return ui->initWeights->checkState();
}

double NetPanel::learningRate() const
{
    return ui->learningRateSpin->value();
}

double NetPanel::quickPropMuFactor() const
{
    return ui->quickPropMuFactorSpin->value();
}

double NetPanel::RPROPDecreaseFactor() const
{
    return ui->RPROPDecreaseFactorSpin->value();
}

bool NetPanel::shuffleTrainData() const
{
    return ui->shuffleTrainData->checkState();
}

double NetPanel::momentum() const
{
    return ui->momentumSpin->value();
}

uint NetPanel::RPROPMinStep() const
{
    return ui->RPROPMinStepSpin->value();
}

uint NetPanel::RPROPMaxStep() const
{
    return ui->RPROPMaxStepSpin->value();
}

uint NetPanel::maxNumberOfNeurons() const
{
    return ui->maxNumberOfNeuronsSpin->value();
}

double NetPanel::outputChangeFraction() const
{
    return ui->outputChangeFractionSpin->value();
}

uint NetPanel::outputStagnationEpochs() const
{
    return ui->outputStagnationEpochsSpin->value();
}

double NetPanel::weightMultiplier() const
{
    return ui->weightMultiplierSpin->value();
}

double NetPanel::candidateChangeFraction() const
{
    return ui->candidateChangeFractionSpin->value();
}

uint NetPanel::candidateStagnationEpochs() const
{
    return ui->candidateStagnationEpochsSpin->value();
}

uint NetPanel::candidateLimit() const
{
    return ui->candidateLimitSpin->value();
}

uint NetPanel::maximumOutEpochs() const
{
    return ui->maximumOutEpochsSpin->value();
}

uint NetPanel::maximumCandidateEpochs() const
{
    return ui->maximumCandidateEpochsSpin->value();
}

uint NetPanel::numberOfCandidateGroups() const
{
    return ui->numberOfCandidateGroupsSpin->value();
}


void NetPanel::openTrainFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open trainibg data file"),last_opened_data_dir_);
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
            QFileInfo fi(fileName);
            last_opened_data_dir_ = fi.dir().absolutePath();
        }

}

void NetPanel::openTestFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,tr("Open test data file"),last_opened_data_dir_);
    if( !fileName.isNull() )
        {
            ui->testEdit->setText(fileName);
            QFileInfo fi(fileName);
            last_opened_data_dir_ = fi.dir().absolutePath();
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
    for(int i = 0; i < FANN_NETTYPE_SHORTCUT+1; ++i)
        {
            ui->netTypeCombo->addItem(tr(FANN_NETTYPE_NAMES[i]));
        }
    for(int i = 0; i < FANN_ERRORFUNC_TANH+1; ++i)
        {
            ui->errorFuncCombo->addItem(tr(FANN_ERRORFUNC_NAMES[i]));

        }
    for(int i = 0; i < FANN_STOPFUNC_BIT+1; ++i)
        {
            ui->stopFuncCombo->addItem(tr(FANN_STOPFUNC_NAMES[i]));
        }
    for(int i = 0; i < FANN_COS + 1; ++i)
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


