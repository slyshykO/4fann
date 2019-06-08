/*
* file   : neuralnetwork.hpp created 2012.3.11
* class  : NeuralNetwork
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef NEURALNETWORK_HPP
#define NEURALNETWORK_HPP

#include <QString>
#include <QVector>
#include <QStringList>
#include <QFile>
#include <QMutex>
#include <QFutureWatcher>
#include <QRunnable>

#include <doublefann.h>
#include "trainingdata.hpp"

class NeuralNetwork : public QObject
{
    Q_OBJECT
public:

    enum ENeuralState
    {
        WaitTraining = 0,
        Training,
        Ready
    };

    NeuralNetwork(QObject * parent = 0);
    ~NeuralNetwork();

    bool isNull();
    QString toString();

    void trainOnData(const TrainingData &data, unsigned int max_epochs,
                        unsigned int epochs_between_reports, float desired_error);
    void trainOnData(const TrainingData &data);
    void cascadeTrainOnData(const TrainingData &data);
    fann_type * run (fann_type *fInput);
    bool initAnn();

    QString annDirDefinition() const;
    void setAnnDirDefinition( const QString &file );
    QString fileName();
    void setFileName( const QString &file );
    QString testFileName();
    void setTestFileName( const QString &file );

    void setNetType(uint type);
    int  netType();
    void setNumInput(uint num);
    uint numInput() const;
    void setNumOutput(uint num);
    uint numOutput() const;
    uint numHiddenLayers();
    QVector<uint> numNeuronsHidden();
    void setNumNeuronsHidden(QVector<uint>&& numNeuronsHidden);

    float desiredError();
    void setDesiredError(float desired_error);
    float bitFailLimit();
    void setBitFailLimit(double);
    uint maxEpochs();
    void setMaxEpochs(uint maxEpochs);
    uint epochsBetweenReports();
    void setEpochsBetweenReports(uint epochsBetweenReports);
    void setMaxNeurons(uint );
    uint maxNeurons();
    void setNeuronsBetweenReports(uint);
    uint neuronsBetweenReports();


    void setConnectionRate(fann_type value);
    fann_type connectionRate();
    void setLearningRate(fann_type value);
    fann_type learningRate();
    void setTrainAlgorithm(int value);
    int trainAlgorithm();
    int activationHidden();
    void setActivationHidden(int);
    int activationOutput();
    void setActivationOutput(int);
    int trainErrorFunc();
    void setTrainErrorFunc(int);


    double hiddenActivationSteppness();
    void setHiddenActivationSteppness(double v);
    double outputActivationSteppness();
    void setOutputActivationSteppness(double v);
    double quickPropDecayFactor();
    void setQuickPropDecayFactor(double v);
    double RPROPIncreaseFactor();
    void setRPROPIncreaseFactor(double v);
    bool initWeights();
    void setInitWeights(bool b);

    double quickPropMuFactor();
    void setQuickPropMuFactor(double v);
    double RPROPDecreaseFactor();
    void setRPROPDecreaseFactor(double v);
    bool shuffleTrainData();
    void setShuffleTrainData(bool b);
    double momentum();
    void setMomentum(double v);
    uint RPROPMinStep();
    void setRPROPMinStep(uint v);
    uint RPROPMaxStep();
    void setRPROPMaxStep(uint v);
    uint maxNumberOfNeurons();
    void setMaxNumberOfNeurons(uint v);
    double outputChangeFraction();
    void setOutputChangeFraction(double v);
    uint outputStagnationEpochs();
    void setOutputStagnationEpochs(uint v);
    double weightMultiplier();
    void setWeightMultiplier(double v);
    double candidateChangeFraction();
    void setCandidateChangeFraction(double v);
    uint candidateStagnationEpochs();
    void setCandidateStagnationEpochs(uint v);
    uint candidateLimit();
    void setCandidateLimit(uint v);
    uint maximumOutEpochs();
    void setMaximumOutEpochs(uint v);
    uint maximumCandidateEpochs();
    void setMaximumCandidateEpochs(uint v);
    uint numberOfCandidateGroups();
    void setnumberOfCandidateGroups(uint v);

    void setSaveTrain(bool value);
    bool saveTrain();

    bool useSavedDataTrain();
    void setUseSavedDataTrain(bool value);

    unsigned int getTotalConnections();
    void setFuture( const QFuture<void> & future );

    float getMSE();
    void detectMinAnn();


public:
    // Потокобезопасные функции
    void addToHistory(float mse, uint bitFail, uint epochs);
    QVector<double> mseHistory();
    QVector<double> bitFailHistory();
    QVector<double> mseTestHistory();
    QVector<double> bitFailTestHistory();
    QVector<double> epochsHistory();
    int epoch();
    int state();

signals:
    void stateChanged(int);

private:
    /** Файлов со всеми определениями нейронные сети */
    QString m_annDirDefinition;
    QString m_fileName;

    /** Указатель на нейронную сеть */
    struct fann * m_ann = nullptr;
    /** Временно хранит среднеквадратическую ошибку для теста */
    double m_testMse = 10000.0;
    struct fann * m_minTestAnn = nullptr;
    /** Временно хранит среднеквадратическую ошибку для тренировки */
    double m_trainMse = 10000.0;
    struct fann * m_minTrainAnn = nullptr;
    /** Тренировочные данные для тестов*/
    QString m_testFileName;
    TrainingData m_testData;




    /** Тип нейронной сети*/
    uint m_netType = FANN_NETTYPE_LAYER;
    /** Количество входов нейронной сети*/
    unsigned int m_num_input;
    /** Количество выходов нейронной сети*/
    unsigned int m_num_output = 0;
    /** Количество нейронов в скрытых слоях */
    QVector<uint> m_num_neurons_hidden;
    /** Желаемая ошибка */
    float m_desired_error = ((float) 0.001) ;
    fann_type m_bitFailLimit = ((fann_type)0.035);
    fann_type m_connectionRate = ((fann_type) 0.5);
    fann_type m_learningRate = ((fann_type) 0.7);
    /** Желаемый алгоритм обучения */
    int m_trainAlgorithm;
    /** Тип обучения, который будет использоваться */
    int m_trainType;
    /** Функця активации скрытых слоев*/
    int m_activationHidden = 0;
    /**  Функця активации выходов*/
    int m_activationOutput = 0;
    /** Функция ошибки */
    int m_trainErrorFunc;

    uint m_max_epochs = 500000;
    uint m_epochs_between_reports = 1000;

    uint m_max_neurons = 10;
    uint m_neurons_between_reports = 1;
    bool isCascadeTrain = false;

    /** Указывает, следует ли сохранить обучение */
    bool m_saveTrain = false;

    /** Указывает на количество тренировок, которые вносятся в сети */
    int m_numTrainnings = 0;

    double m_hiddenActivationSteppness = 0.5;
    double m_outputActivationSteppness = 0.5;
    double m_quickPropDecayFactor = -0.0001;
    double m_RPROPIncreaseFactor = 1.2;
    bool m_initWeights = false;
    double m_quickPropMuFactor = 1.75;
    double m_RPROPDecreaseFactor = 0.5;
    bool m_shuffleTrainData = false;
    double m_momentum = 0.;
    uint m_RPROPMinStep = 0;
    uint m_RPROPMaxStep = 50;
    uint m_maxNumberOfNeurons = 10;
    double m_outputChangeFraction = 0.01;
    uint m_outputStagnationEpochs = 12;
    double m_weightMultiplier = 0.4;
    double m_candidateChangeFraction = 0.01;
    uint m_candidateStagnationEpochs = 12;
    uint m_candidateLimit = 1000;
    uint m_maximumOutEpochs = 150;
    uint m_maximumCandidateEpochs = 150;
    uint m_numberOfCandidateGroups = 2;

    QMutex m_mutex;
    QFutureWatcher<void> m_trainFutureWatcher;
    QVector<float> m_mseHistory;
    QVector<uint> m_bitFailHistory;
    QVector<float> m_mseTestHistory;
    QVector<uint> m_bitFailTestHistory;
    QVector<uint> m_epochHistory;
    int m_epoch = 0;

    ENeuralState m_state = {WaitTraining};

private slots:
   void trainFinished();
   void trainStarted();
   void trainPaused();
private:
   void setState( ENeuralState );
   void resetParams();
   void annDestroy();

   friend class NetTraining;
private:
   QMutex need_stop_mutex_;
   bool   need_stop_ = false;
public:
   void setNeedStop(bool s);
   bool needStop();
public slots:
   void stopTraining();
};

/* Type: fann_callback_type
   This callback function can be called during training when using <fann_train_on_data>,
   <fann_train_on_file> or <fann_cascadetrain_on_data>.

    >typedef int (FANN_API * fann_callback_type) (struct fann *ann, struct fann_train_data *train,
    >											  unsigned int max_epochs,
    >                                             unsigned int epochs_between_reports,
    >                                             float desired_error, unsigned int epochs);

    The callback can be set by using <fann_set_callback> and is very usefull for doing custom
    things during training. It is recommended to use this function when implementing custom
    training procedures, or when visualizing the training in a GUI etc. The parameters which the
    callback function takes is the parameters given to the <fann_train_on_data>, plus an epochs
    parameter which tells how many epochs the training have taken so far.

    The callback function should return an integer, if the callback function returns -1, the training
    will terminate.

    Example of a callback function:
        >int FANN_API test_callback(struct fann *ann, struct fann_train_data *train,
        >				            unsigned int max_epochs, unsigned int epochs_between_reports,
        >				            float desired_error, unsigned int epochs)
        >{
        >	printf("Epochs     %8d. MSE: %.5f. Desired-MSE: %.5f\n", epochs, fann_get_MSE(ann), desired_error);
        >	return 0;
        >}

    See also:
        <fann_set_callback>, <fann_train_on_data>
 */

class NetTraining : public QRunnable
{
    NetTraining( NeuralNetwork * net, TrainingData * data);
    ~NetTraining();
    void run();
private:
    NeuralNetwork * m_net;
    TrainingData * m_data;
};


#endif // NEURALNETWORK_HPP
