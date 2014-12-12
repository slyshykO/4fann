/*
* file   : neuralnetwork.cpp created 2012.3.11
* class  : NeuralNetwork
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "neuralnetwork.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QMutexLocker>
#include <QThread>
#include <QDateTime>
#include <cmath>
#include <algorithm>

class SleeperThread : public QThread
{
public:
static void msleep(unsigned long msecs)
{
    QThread::msleep(msecs);
}
};

int FANN_API internalCallback(fann *ann, fann_train_data *train, unsigned int max_epochs,
                              unsigned int epochs_between_reports, float desired_error,
                              unsigned int epochs)
{
    Q_UNUSED(train)
    Q_UNUSED(max_epochs)
    Q_UNUSED(epochs_between_reports)
    Q_UNUSED(desired_error)

    if(ann->user_data)
        {
            NeuralNetwork * net = reinterpret_cast<NeuralNetwork *>(ann->user_data);
            qDebug()<<QThread::currentThreadId()<<" epoch:"<<epochs<<" MSE:"<<fann_get_MSE(ann);

            net->detectMinAnn();
            net->addToHistory(fann_get_MSE(ann), fann_get_bit_fail(ann), epochs);
            //test

        }
    //::Sleep(100);
    SleeperThread::msleep(100);
    if( QThread::currentThread()->priority() != QThread::IdlePriority )
        QThread::currentThread()->setPriority(QThread::IdlePriority);
    return 0;
}

NeuralNetwork::NeuralNetwork(QObject *parent)
    : QObject(parent), m_state(WaitTraining),
    m_annDirDefinition(""), m_num_output (0),
    m_desired_error ((float) 0.001),
    m_connectionRate((fann_type) 0.5), m_learningRate((fann_type) 0.7),
    m_max_epochs (500000), m_epochs_between_reports (1000), m_numTrainnings(0),
    m_saveTrain(false), m_ann(0),
    m_activationHidden(0), m_activationOutput(0), m_netType(FANN_NETTYPE_LAYER),
    m_max_neurons(10),m_neurons_between_reports(1),isCascadeTrain(false),
    m_bitFailLimit(0.035),
    m_hiddenActivationSteppness(0.5),
    m_outputActivationSteppness(0.5),
    m_quickPropDecayFactor(-0.0001),
    m_RPROPIncreaseFactor(1.2),
    m_initWeights(false),
    m_quickPropMuFactor(1.75),
    m_RPROPDecreaseFactor(0.5),
    m_shuffleTrainData(false),
    m_momentum(0.0),
    m_RPROPMinStep(0),
    m_RPROPMaxStep(50),
    m_maxNumberOfNeurons(10),
    m_outputChangeFraction(0.01),
    m_outputStagnationEpochs(12),
    m_weightMultiplier(0.4),
    m_candidateChangeFraction(0.01),
    m_candidateStagnationEpochs(12),
    m_candidateLimit(1000),
    m_maximumOutEpochs(150),
    m_maximumCandidateEpochs(150),
    m_numberOfCandidateGroups(2),
    m_testMse(10000),
    m_minTestAnn(0),
    m_trainMse(10000),
    m_minTrainAnn(0)
{
    connect(&m_trainFutureWatcher, SIGNAL(finished()), this, SLOT(trainFinished()) );
    connect(&m_trainFutureWatcher, SIGNAL(started()), this, SLOT(trainStarted()) );
    connect(&m_trainFutureWatcher, SIGNAL(paused()), this, SLOT(trainPaused()) );
}

NeuralNetwork::~NeuralNetwork()
{
    annDestroy();
}

void NeuralNetwork::annDestroy()
{
    if ( m_ann != 0 )
        {
            fann_destroy(m_ann);
            m_ann = 0;
            resetParams();
        }
    if ( m_minTestAnn != 0)
        {
            fann_destroy(m_minTestAnn);
            m_minTestAnn = 0;
        }
    if ( m_minTrainAnn != 0)
        {
            fann_destroy(m_minTrainAnn);
            m_minTrainAnn = 0;
        }
}

bool NeuralNetwork::isNull()
{
    return (m_ann == 0);
}

bool NeuralNetwork::initAnn()
{
    if(m_num_input == 0)
        return false;
    annDestroy();
    unsigned int *layers = new unsigned int[this->numHiddenLayers() + 2];

    // La capa 0 es la de entradas
    layers[0] = m_num_input;
    for ( unsigned int i = 1 ; i < (numHiddenLayers()+1) ; i++ )
        {
            layers[i] = m_num_neurons_hidden.at(i-1);
        }
    // La ultima capa es la de salida
    layers[numHiddenLayers()+1] = m_num_output;
    switch (m_netType)
        {
        case FANN_NETTYPE_LAYER:
            m_ann = fann_create_standard_array(numHiddenLayers()+2, layers);
            break;
        case FANN_NETTYPE_SHORTCUT:
            m_ann = fann_create_shortcut_array(numHiddenLayers()+2, layers);
            break;
        }

    if(m_ann != NULL)
        {
            fann_set_callback(m_ann, internalCallback);
            fann_set_user_data(m_ann, this);//нужно для калбэка

            fann_set_bit_fail_limit(m_ann, m_bitFailLimit);

            fann_set_training_algorithm(m_ann, static_cast<fann_train_enum>(m_trainAlgorithm));
            fann_set_learning_rate( m_ann, m_learningRate );
            fann_set_activation_function_hidden(m_ann,
                                                static_cast<fann_activationfunc_enum>(m_activationHidden));
            fann_set_activation_function_output(m_ann,
                                                static_cast<fann_activationfunc_enum>(m_activationOutput));
            fann_set_train_error_function(m_ann, static_cast<fann_errorfunc_enum>(m_trainErrorFunc) );


            //m_connectionRate((fann_type) 0.5), m_learningRate((fann_type) 0.7)
            fann_set_learning_rate(m_ann, m_learningRate);
//            m_hiddenActivationSteppness(0.5),
            fann_set_activation_steepness_hidden(m_ann, m_hiddenActivationSteppness);
//            m_outputActivationSteppness(0.5),
            fann_set_activation_steepness_output(m_ann, m_outputActivationSteppness);
//            m_quickPropDecayFactor(-0.0001),
            fann_set_quickprop_decay(m_ann, m_quickPropDecayFactor);
//            m_RPROPIncreaseFactor(1.2),
            fann_set_rprop_increase_factor(m_ann, m_RPROPIncreaseFactor);
//            m_initWeights(false),

//            m_quickPropMuFactor(1.75),
            fann_set_quickprop_mu(m_ann, m_quickPropMuFactor);
//            m_RPROPDecreaseFactor(0.5),
            fann_set_rprop_decrease_factor(m_ann, m_RPROPDecreaseFactor);
//            m_shuffleTrainData(false),
//            m_momentum(0.0),
            fann_set_learning_momentum(m_ann, m_momentum);
//            m_RPROPMinStep(0),
            fann_set_rprop_delta_min(m_ann, m_RPROPMinStep);
//            m_RPROPMaxStep(50),
            fann_set_rprop_delta_max(m_ann, m_RPROPMaxStep);
//            m_maxNumberOfNeurons(10),

//            m_outputChangeFraction(0.01),

//            m_outputStagnationEpochs(12),
            fann_set_cascade_output_stagnation_epochs(m_ann, m_outputStagnationEpochs);
//            m_weightMultiplier(0.4),
            fann_set_cascade_weight_multiplier(m_ann, m_weightMultiplier);
//            m_candidateChangeFraction(0.01),
            fann_set_cascade_candidate_change_fraction(m_ann, m_candidateChangeFraction);
//            m_candidateStagnationEpochs(12),
            fann_set_cascade_candidate_stagnation_epochs(m_ann, m_candidateStagnationEpochs);
//            m_candidateLimit(1000),
            fann_set_cascade_candidate_limit(m_ann, m_candidateLimit);
//            m_maximumOutEpochs(150),
            fann_set_cascade_max_out_epochs(m_ann, m_maximumOutEpochs);
//            m_maximumCandidateEpochs(150),
            fann_set_cascade_min_cand_epochs(m_ann, m_maximumCandidateEpochs);
//            m_numberOfCandidateGroups(2)
            fann_set_cascade_num_candidate_groups(m_ann, m_numberOfCandidateGroups);


            setState(WaitTraining);
        }

    return (m_ann != 0 );
}

QString NeuralNetwork::toString()
{
    QString res;
    if(m_ann != 0)
        {
            /* Save network parameters */
            res.append(QString( "num_layers=%1\n").arg( (int)(m_ann->last_layer - m_ann->first_layer))) ;
            res.append(QString( "learning_rate=%1\n").arg( m_ann->learning_rate));
            res.append(QString( "connection_rate=%1\n").arg( m_ann->connection_rate));
            res.append(QString( "network_type=%1\n").arg( FANN_NETTYPE_NAMES[m_ann->network_type]));

            res.append(QString( "learning_momentum=%1\n").arg( m_ann->learning_momentum));
            res.append(QString( "training_algorithm=%1\n").arg( FANN_TRAIN_NAMES[m_ann->training_algorithm]));
            res.append(QString( "train_error_function=%1\n").arg( FANN_ERRORFUNC_NAMES[m_ann->train_error_function]));
            res.append(QString( "train_stop_function=%1\n").arg( FANN_STOPFUNC_NAMES[m_ann->train_stop_function]));
            res.append(QString( "cascade_output_change_fraction=%1\n").arg( m_ann->cascade_output_change_fraction));
            res.append(QString( "quickprop_decay=%1\n").arg( m_ann->quickprop_decay));
            res.append(QString( "quickprop_mu=%1\n").arg( m_ann->quickprop_mu));
            res.append(QString( "rprop_increase_factor=%1\n").arg( m_ann->rprop_increase_factor));
            res.append(QString( "rprop_decrease_factor=%1\n").arg( m_ann->rprop_decrease_factor));
            res.append(QString( "rprop_delta_min=%1\n").arg( m_ann->rprop_delta_min));
            res.append(QString( "rprop_delta_max=%1\n").arg( m_ann->rprop_delta_max));
            res.append(QString( "rprop_delta_zero=%1\n").arg( m_ann->rprop_delta_zero));
            res.append(QString( "cascade_output_stagnation_epochs=%1\n").arg( m_ann->cascade_output_stagnation_epochs));
            res.append(QString( "cascade_candidate_change_fraction=%1\n").arg( m_ann->cascade_candidate_change_fraction));
            res.append(QString( "cascade_candidate_stagnation_epochs=%1\n").arg( m_ann->cascade_candidate_stagnation_epochs));
            res.append(QString( "cascade_max_out_epochs=%1\n").arg( m_ann->cascade_max_out_epochs));
            res.append(QString( "cascade_min_out_epochs=%1\n").arg( m_ann->cascade_min_out_epochs));
            res.append(QString( "cascade_max_cand_epochs=%1\n").arg( m_ann->cascade_max_cand_epochs));
            res.append(QString( "cascade_min_cand_epochs=%1\n").arg( m_ann->cascade_min_cand_epochs));
            res.append(QString( "cascade_num_candidate_groups=%1\n").arg( m_ann->cascade_num_candidate_groups));
        }
    else
        {
            res = "NULL";
        }

    return res;
}

/** \fn NeuralNetwork::resetParams( )
  Resetea los parametros de la red
  */
void NeuralNetwork::resetParams()
{
    //FIXME: - add all variables, or delete fucktion.
    m_num_neurons_hidden.clear();
    m_desired_error = 0;
    m_connectionRate = 0;
    m_learningRate = 0;
    m_max_epochs = 0;
    m_epochs_between_reports = 0;
    m_numTrainnings = 0;
}

void NeuralNetwork::setState(NeuralNetwork::ENeuralState st)
{
    m_state = st;
    emit stateChanged(static_cast<int>(m_state));
}

void NeuralNetwork::trainOnData(const TrainingData &data, unsigned int max_epochs, unsigned int epochs_between_reports, float desired_error)
{
    if ((m_ann != NULL) && (data.m_train_data != NULL))
        {
            m_testData.loadFromFile(m_testFileName);
            setState(Training);
            isCascadeTrain = false;

            if(m_initWeights)
                fann_init_weights(m_ann, data.m_train_data);
            if(m_shuffleTrainData)
                fann_shuffle_train_data(data.m_train_data);

            fann_train_on_data(m_ann, data.m_train_data, max_epochs,
                epochs_between_reports, desired_error);
        }
}

void NeuralNetwork::trainOnData(const TrainingData &data)
{
    if ((m_ann != NULL) && (data.m_train_data != NULL))
        {
            m_testData.loadFromFile(m_testFileName);
            setState(Training);
            isCascadeTrain = false;

            if(m_initWeights)
                fann_init_weights(m_ann, data.m_train_data);
            if(m_shuffleTrainData)
                fann_shuffle_train_data(data.m_train_data);

            fann_train_on_data(m_ann, data.m_train_data, m_max_epochs,
                m_epochs_between_reports, m_desired_error);
        }
}

void NeuralNetwork::cascadeTrainOnData(const TrainingData &data)
{
    if ((m_ann != NULL) && (data.m_train_data != NULL))
        {
            m_testData.loadFromFile(m_testFileName);
            setState(Training);
            isCascadeTrain = true;

            if(m_initWeights)
                fann_init_weights(m_ann, data.m_train_data);
            if(m_shuffleTrainData)
                fann_shuffle_train_data(data.m_train_data);

            fann_cascadetrain_on_data(m_ann, data.m_train_data, m_max_neurons,
                m_neurons_between_reports, m_desired_error);
        }
}

void NeuralNetwork::detectMinAnn()
{
    if( m_ann == 0)
        return;
    if(m_minTrainAnn == 0)
        {
            m_minTrainAnn = fann_copy(m_ann);
        }
    if(m_minTestAnn == 0)
        {
            m_minTestAnn = fann_copy(m_ann);
        }

    if (fann_get_MSE(m_ann) < fann_get_MSE(m_minTrainAnn))
        {
            fann_destroy(m_minTrainAnn);
            m_minTrainAnn = fann_copy(m_ann);
        }

    if( !m_testData.isNull() )
        {
            fann * tmpTestAnn = fann_copy(m_ann);
            fann_reset_MSE(tmpTestAnn);
            fann_test_data(tmpTestAnn,m_testData.data());
            m_mseTestHistory.append(fann_get_MSE(tmpTestAnn));
            m_bitFailTestHistory.append(fann_get_bit_fail(tmpTestAnn));
            if (fann_get_MSE(m_ann) < fann_get_MSE(tmpTestAnn))
                {
                    fann_destroy(m_minTestAnn);
                    m_minTestAnn = tmpTestAnn;
                }
            else
                {
                    fann_destroy(tmpTestAnn);
                }
        }
}

void NeuralNetwork::trainFinished()
{
    //save ann

    QFileInfo fileInfo;
    fileInfo.setFile(QDir(m_annDirDefinition),fileName());
    QString testsave = fileInfo.baseName() + "-minTest." + fileInfo.completeSuffix();
    QString trainsave = fileInfo.baseName() + "-minTrain." + fileInfo.completeSuffix();
    QFileInfo fileInfoTest;
    fileInfoTest.setFile(QDir(m_annDirDefinition),testsave);
    QFileInfo fileInfoTrain;
    fileInfoTrain.setFile(QDir(m_annDirDefinition),trainsave);

    fann_save(m_ann, fileInfo.absoluteFilePath().toLocal8Bit());
    fann_save(m_minTestAnn,fileInfoTest.absoluteFilePath().toLocal8Bit());
    fann_save(m_minTrainAnn,fileInfoTrain.absoluteFilePath().toLocal8Bit());

    //сохранение протокола тренировки
    QFile file( fileInfo.absoluteFilePath().append(".prot"));
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            QTextStream stream(&file);
            stream<<"net:"<<fileName()<<endl;
            stream<<"train method:"<< (isCascadeTrain ? "cascade" : "normal")<<endl;
            stream<<"hidden activation:"<<FANN_ACTIVATIONFUNC_NAMES[m_activationHidden]<<endl;
            stream<<"output activation:"<<FANN_ACTIVATIONFUNC_NAMES[m_activationOutput]<<endl;
            stream<<"max epochs:"<<m_max_epochs<<endl;
            stream<<"epochs between reports:"<<m_epochs_between_reports<<endl;
            stream<<"max neurons:"<<m_max_neurons<<endl;
            stream<<"neurons between reports:"<<m_neurons_between_reports<<endl;
            stream<<"desiered error:"<<m_desired_error<<endl;
            stream<<"error:"<<fann_get_MSE(m_ann)<<endl;
            stream<<"train errors:";
            foreach ( const float & f , m_mseHistory)
                {
                    stream<<f<<" ";
                }
            stream<<endl;
            stream<<"train bit fails:";
            foreach ( const uint & u , m_bitFailHistory)
                {
                    stream<<u<<" ";
                }
            stream<<endl;
            stream<<"epochs:";
            foreach ( const uint & u , m_epochHistory)
                {
                    stream<<u<<" ";
                }
            stream<<endl;
        }

    qDebug()<<"Ann saved to "<<fileInfo.absoluteFilePath();
    setState(Ready);
}

void NeuralNetwork::trainStarted()
{

}

void NeuralNetwork::trainPaused()
{
    setState(WaitTraining);
}

/** \fn NeuralNetwork::run (fann_type *fInput) */
fann_type *NeuralNetwork::run( fann_type *fInput)
{
    fann_type *fOutput = NULL;
    if ( m_ann != NULL )
        {
            fOutput = fann_run( m_ann, fInput );
        }
    return fOutput;
}

QString NeuralNetwork::annDirDefinition() const
{
    return m_annDirDefinition;
}

void NeuralNetwork::setAnnDirDefinition( const QString &file )
{
    m_annDirDefinition = file;
}

QString NeuralNetwork::fileName()
{
    if(m_fileName.isNull())
        {
            QString tmp;
            for(int i = 0; i < m_num_neurons_hidden.size(); ++i )
                {
                    if(i != (m_num_neurons_hidden.size()-1) )
                        tmp.append(QString("%1_").arg(m_num_neurons_hidden[i]));
                    else
                        tmp.append(QString("%1").arg(m_num_neurons_hidden[i]));
                }
            m_fileName = QString("%1-[%2]-%3(%4).ann")
                         .arg(numInput())
                         .arg(tmp)
                         .arg(numOutput())
                         .arg(QDateTime::currentDateTime().toString("hh-mm-ss"));
        }

    return m_fileName;
}

void NeuralNetwork::setFileName(const QString &file)
{
    m_fileName = file;
}

QString NeuralNetwork::testFileName()
{
    return m_testFileName;
}

void NeuralNetwork::setTestFileName(const QString &file)
{
    m_testFileName = file;
}

void NeuralNetwork::setNetType(uint type)
{
    if(m_ann == 0)
        m_netType = type;
}

int NeuralNetwork::netType()
{
    return m_netType;
}

void NeuralNetwork::setNumInput(uint num)
{
    m_num_input = num;
}

uint NeuralNetwork::numInput() const
{
    return m_ann->num_input;
}

void NeuralNetwork::setNumOutput(uint num)
{
    m_num_output = num;
}

uint NeuralNetwork::numOutput() const
{
    return m_num_output;
}

uint NeuralNetwork::numHiddenLayers()
{
    return m_num_neurons_hidden.size();
}

QVector<uint> NeuralNetwork::numNeuronsHidden()
{
    return m_num_neurons_hidden;
}

void NeuralNetwork::setNumNeuronsHidden(QVector<uint> numNeuronsHidden)
{
    this->m_num_neurons_hidden = numNeuronsHidden;
}

float NeuralNetwork::desiredError()
{
    return m_desired_error;
}

void NeuralNetwork::setDesiredError(float desired_error)
{
    m_desired_error = desired_error;
}

float NeuralNetwork::bitFailLimit()
{
    return m_bitFailLimit;
}

void NeuralNetwork::setBitFailLimit(double v)
{
    m_bitFailLimit = v;
}

uint NeuralNetwork::maxEpochs()
{
    return this->m_max_epochs;
}

void NeuralNetwork::setMaxEpochs(uint maxEpochs)
{
    this->m_max_epochs = maxEpochs;
}

uint NeuralNetwork::epochsBetweenReports()
{
    return this->m_epochs_between_reports;
}

void NeuralNetwork::setEpochsBetweenReports(uint epochsBetweenReports)
{
    this->m_epochs_between_reports = epochsBetweenReports;
}

void NeuralNetwork::setMaxNeurons(uint v)
{
    m_max_neurons = v;
}

uint NeuralNetwork::maxNeurons()
{
    return m_max_neurons;
}

void NeuralNetwork::setNeuronsBetweenReports(uint v)
{
    m_neurons_between_reports = v;
}

uint NeuralNetwork::neuronsBetweenReports()
{
    return m_neurons_between_reports;
}

void NeuralNetwork::setConnectionRate(fann_type value)
{
    m_connectionRate = value;
}

fann_type NeuralNetwork::connectionRate()
{
    return m_connectionRate;
}

void NeuralNetwork::setLearningRate(fann_type value)
{
    m_learningRate = value;
}

fann_type NeuralNetwork::learningRate()
{
    return m_learningRate;
}

void NeuralNetwork::setTrainAlgorithm(int value)
{
    m_trainAlgorithm = value;
}

int NeuralNetwork::trainAlgorithm()
{
    return m_trainAlgorithm;
}

int NeuralNetwork::activationHidden()
{
    return m_activationHidden;
}

void NeuralNetwork::setActivationHidden(int a)
{
    m_activationHidden = a;
}

int NeuralNetwork::activationOutput()
{
    return m_activationOutput;
}

void NeuralNetwork::setActivationOutput(int a)
{
    m_activationOutput = a;
}

int NeuralNetwork::trainErrorFunc()
{
    return m_trainErrorFunc;
}

void NeuralNetwork::setTrainErrorFunc(int v)
{
    m_trainErrorFunc = v;
}

double NeuralNetwork::hiddenActivationSteppness()
{
    return m_hiddenActivationSteppness;
}

void NeuralNetwork::setHiddenActivationSteppness(double v)
{
    m_hiddenActivationSteppness = v;
}

double NeuralNetwork::outputActivationSteppness()
{
    return m_outputActivationSteppness;
}

void NeuralNetwork::setOutputActivationSteppness(double v)
{
    m_outputActivationSteppness = v;
}

double NeuralNetwork::quickPropDecayFactor()
{
    return m_quickPropDecayFactor;
}

void NeuralNetwork::setQuickPropDecayFactor(double v)
{
    m_quickPropDecayFactor = v;
}

double NeuralNetwork::RPROPIncreaseFactor()
{
    return m_RPROPIncreaseFactor;
}

void NeuralNetwork::setRPROPIncreaseFactor(double v)
{
    m_RPROPIncreaseFactor = v;
}

bool NeuralNetwork::initWeights()
{
    return m_initWeights;
}

void NeuralNetwork::setInitWeights(bool b)
{
    m_initWeights = b;
}

double NeuralNetwork::quickPropMuFactor()
{
    return m_quickPropMuFactor;
}

void NeuralNetwork::setQuickPropMuFactor(double v)
{
    m_quickPropMuFactor = v;
}

double NeuralNetwork::RPROPDecreaseFactor()
{
    return m_RPROPDecreaseFactor;
}

void NeuralNetwork::setRPROPDecreaseFactor(double v)
{
    m_RPROPDecreaseFactor = v;
}

bool NeuralNetwork::shuffleTrainData()
{
    return m_shuffleTrainData;
}

void NeuralNetwork::setShuffleTrainData(bool b)
{
    m_shuffleTrainData = b;
}

double NeuralNetwork::momentum()
{
    return m_momentum;
}

void NeuralNetwork::setMomentum(double v)
{
    m_momentum = v;
}

uint NeuralNetwork::RPROPMinStep()
{
    return m_RPROPMinStep;
}

void NeuralNetwork::setRPROPMinStep(uint v)
{
    m_RPROPMinStep = v;
}

uint NeuralNetwork::RPROPMaxStep()
{
    return m_RPROPMaxStep;
}

void NeuralNetwork::setRPROPMaxStep(uint v)
{
    m_RPROPMaxStep = v;
}

uint NeuralNetwork::maxNumberOfNeurons()
{
    return m_maxNumberOfNeurons;
}

void NeuralNetwork::setMaxNumberOfNeurons(uint v)
{
    m_maxNumberOfNeurons = v;
}

double NeuralNetwork::outputChangeFraction()
{
    return m_outputChangeFraction;
}

void NeuralNetwork::setOutputChangeFraction(double v)
{
    m_outputChangeFraction = v;
}

uint NeuralNetwork::outputStagnationEpochs()
{
    return m_outputStagnationEpochs;
}

void NeuralNetwork::setOutputStagnationEpochs(uint v)
{
    m_outputStagnationEpochs = v;
}

double NeuralNetwork::weightMultiplier()
{
    return m_weightMultiplier;
}

void NeuralNetwork::setWeightMultiplier(double v)
{
    m_weightMultiplier = v;
}

double NeuralNetwork::candidateChangeFraction()
{
    return m_candidateChangeFraction;
}

void NeuralNetwork::setCandidateChangeFraction(double v)
{
    m_candidateChangeFraction = v;
}

uint NeuralNetwork::candidateStagnationEpochs()
{
    return m_candidateStagnationEpochs;
}

void NeuralNetwork::setCandidateStagnationEpochs(uint v)
{
    m_candidateStagnationEpochs = v;
}

uint NeuralNetwork::candidateLimit()
{
    return m_candidateLimit;
}

void NeuralNetwork::setCandidateLimit(uint v)
{
    m_candidateLimit = v;
}

uint NeuralNetwork::maximumOutEpochs()
{
    return m_maximumOutEpochs;
}

void NeuralNetwork::setMaximumOutEpochs(uint v)
{
    m_maximumOutEpochs = v;
}

uint NeuralNetwork::maximumCandidateEpochs()
{
    return m_maximumCandidateEpochs;
}

void NeuralNetwork::setMaximumCandidateEpochs(uint v)
{
    m_maximumCandidateEpochs = v;
}

uint NeuralNetwork::numberOfCandidateGroups()
{
    return m_numberOfCandidateGroups;
}

void NeuralNetwork::setnumberOfCandidateGroups(uint v)
{
    m_numberOfCandidateGroups = v;
}


float NeuralNetwork::getMSE()
{
    if(m_ann != 0)
        return fann_get_MSE(m_ann);

    return 0;
}

unsigned int NeuralNetwork::getTotalConnections()
{
    if(m_ann != 0)
        return fann_get_total_connections(m_ann);

    return 0;
}

void NeuralNetwork::setFuture(const QFuture<void> &future)
{
    m_trainFutureWatcher.setFuture(future);
}

void NeuralNetwork::setSaveTrain(bool value)
{
    m_saveTrain = value;
}

bool NeuralNetwork::saveTrain()
{
    return m_saveTrain;
}

void NeuralNetwork::addToHistory(float mse, uint bitFail, uint epochs)
{
    QMutexLocker locker(&m_mutex);
    m_mseHistory.append(mse);
    m_bitFailHistory.append(bitFail);
    m_epochHistory.append(epochs);
    m_epoch = epochs;
    emit stateChanged(static_cast<int>(m_state));
}

QVector<double> NeuralNetwork::mseHistory()
{
    QVector<double> res;
    {
        QMutexLocker locker(&m_mutex);
        res.resize(m_mseHistory.size());
        std::copy(m_mseHistory.constBegin(), m_mseHistory.constEnd(), res.begin());
    }
    return res;
}

QVector<double> NeuralNetwork::bitFailHistory()
{
    QVector<double> res;
    {
        QMutexLocker locker(&m_mutex);
        res.resize(m_bitFailHistory.size());
        std::copy(m_bitFailHistory.constBegin(), m_bitFailHistory.constEnd(), res.begin());
    }
    return res;
}

QVector<double> NeuralNetwork::mseTestHistory()
{
    QVector<double> res;
    {
        QMutexLocker locker(&m_mutex);
        res.resize(m_mseTestHistory.size());
        std::copy(m_mseTestHistory.constBegin(), m_mseTestHistory.constEnd(), res.begin());
    }
    return res;
}

QVector<double> NeuralNetwork::bitFailTestHistory()
{
    QVector<double> res;
    {
        QMutexLocker locker(&m_mutex);
        res.resize(m_bitFailTestHistory.size());
        std::copy(m_bitFailTestHistory.constBegin(), m_bitFailTestHistory.constEnd(), res.begin());
    }
    return res;
}

QVector<double> NeuralNetwork::epochsHistory()
{
    QVector<double> res;
    {
        QMutexLocker locker(&m_mutex);
        res.resize(m_epochHistory.size());
        std::copy(m_epochHistory.constBegin(), m_epochHistory.constEnd(), res.begin());
    }
    return res;
}

int NeuralNetwork::epoch()
{
    int res;
    {
        QMutexLocker locker(&m_mutex);
        res = m_epoch;
    }
    return res;
}

int NeuralNetwork::state()
{
    int res;
    {
        QMutexLocker locker(&m_mutex);
        res = static_cast<int>(m_state);
    }
    return res;

}




NetTraining::NetTraining(NeuralNetwork *net, TrainingData *data)
    :QRunnable(), m_net(0), m_data(0)
{
    m_net = net;
    m_data = new TrainingData(*data);
}
NetTraining::~NetTraining()
{
    if(m_data)
        delete m_data;
}

void NetTraining::run()
{
    QThread::currentThread()->setPriority(QThread::IdlePriority);
    if( m_net )
        {
            m_net->trainOnData( *m_data );
            m_net->setState(NeuralNetwork::Ready);
        }
}
