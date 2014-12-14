/*
* file   : netpanel.hpp created 2012.3.12
* class  : NetPanel
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef NETPANEL_HPP
#define NETPANEL_HPP

#include <QWidget>


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

    QVector<uint> hiddenLayers() const;
    uint numInput() const;
    uint numOutput() const;

    uint netType() const;
    uint stopFuncType() const;
    uint errorFunkType() const;
    uint traingMethod() const;
    uint activationHiddens() const;
    uint activationOutputs() const;
    uint maxEpochs() const;
    uint epochsBetweenReports() const;
    float desiredError() const;
    double bitFailLimit() const;
    bool isNormalTrain() const;

    QString trainingFileName() const;
    QString testFileName() const;
    QString saveDirName() const;

    double connectionRate() const;
    double hiddenActivationSteppness() const;
    double outputActivationSteppness() const;
    double quickPropDecayFactor() const;
    double RPROPIncreaseFactor() const;
    bool initWeights() const;
    double learningRate() const;
    double quickPropMuFactor() const;
    double RPROPDecreaseFactor() const;
    bool shuffleTrainData() const;
    double momentum() const;
    uint RPROPMinStep() const;
    uint RPROPMaxStep() const;
    uint maxNumberOfNeurons() const;
    double outputChangeFraction() const;
    uint outputStagnationEpochs() const;
    double weightMultiplier() const;
    double candidateChangeFraction() const;
    uint candidateStagnationEpochs() const;
    uint candidateLimit() const;
    uint maximumOutEpochs() const;
    uint maximumCandidateEpochs() const;
    uint numberOfCandidateGroups() const;

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
