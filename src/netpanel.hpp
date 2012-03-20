/*
* file   : netpanel.hpp created 2012.3.12
* class  : NetPanel
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef NETPANEL_HPP
#define NETPANEL_HPP

#include <QtGui/QWidget>


//trainEdit;
//saveDirEdit;
//netTypeCombo;
//hiddenSpin;
//hiddenLayers;
//stopFuncCombo;
//desiredMSESpin;
//bitFailLimitSpin;
//activationFuncHiddenCombo;
//activationFuncOutputCombo;
//reportSpin;
//epochSpin;
//trainMethodCombo;
//errorFuncCombo;
//connectionRateSpin;
//hiddenActivationSteppnessSpin;
//quickPropDecayFactorSpin;
//RPROPIncreaseFactorSpin;
//initWeights;
//learningRateSpin;
//quickPropMuFactorSpin;
//RPROPDecreaseFactorSpin;
//shuffleTrainData;
//momentumSpin;
//RPROPMinStepSpin;
//RPROPMaxStepSpin;
//maxNumberOfNeuronsSpin;
//outputChangeFractionSpin;
//outputStagnationEpochsSpin;
//weightMultiplierSpin;
//candidateChangeFractionSpin;
//candidateStagnationEpochsSpin;
//candidateLimitSpin;
//maximumOutEpochsSpin;
//maximumCandidateEpochsSpin;
//numberOfCandidateGroupsSpin;


namespace Ui {
class NetPanel;
}

class NetPanel : public QWidget
{
    Q_OBJECT

public:
    explicit NetPanel(QWidget *parent = 0);
    ~NetPanel();

    QVector<uint> hiddenLayers();
    uint numInput();
    uint numOutput();

    uint netType();
    uint stopFuncType();
    uint errorFunkType();
    uint traingMethod();
    uint activationHiddens();
    uint activationOutputs();
    uint maxEpochs();
    uint epochsBetweenReports();
    float desiredError();
    double bitFailLimit();
    bool isNormalTrain();

    QString trainingFileName();
    QString testFileName();
    QString saveDirName();

    double connectionRate();
    double hiddenActivationSteppness();
    double outputActivationSteppness();
    double quickPropDecayFactor();
    double RPROPIncreaseFactor();
    bool initWeights();
    double learningRate();
    double quickPropMuFactor();
    double RPROPDecreaseFactor();
    bool shuffleTrainData();
    double momentum();
    uint RPROPMinStep();
    uint RPROPMaxStep();
    uint maxNumberOfNeurons();
    double outputChangeFraction();
    uint outputStagnationEpochs();
    double weightMultiplier();
    double candidateChangeFraction();
    uint candidateStagnationEpochs();
    uint candidateLimit();
    uint maximumOutEpochs();
    uint maximumCandidateEpochs();
    uint numberOfCandidateGroups();

private slots:
    void openTrainFile();
    void openTestFile();
    void openSaveDir();
    void hiddenLayerChanged(int i);
private:
    void initControls();
    QVector<int> readTrainHead(const QString & fn);
    void addHidden(int q = 0);
private:
    Ui::NetPanel *ui;
    uint m_numInput;
    uint m_numOutput;
};

#endif // NETPANEL_HPP
