/*******************************************************************************
*  file    : nettrainview.hpp
*  created : 14.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#ifndef NETTRAINVIEW_HPP
#define NETTRAINVIEW_HPP

#include <QWidget>

class QCustomPlot;
class QTextEdit;


class NetTrainView : public QWidget
{
    Q_OBJECT
public:
    explicit NetTrainView(QWidget *parent = 0);

    void setNetText(const QString & txt);
    void setMSEData(const QVector<double> & , const QVector<double> & );
    void setBitFailData(const QVector<double> & , const QVector<double> & );
    void setTestMSEData(const QVector<double> & , const QVector<double> & );
    void setTestBitFailData(const QVector<double> & , const QVector<double> & );
private:
    QCustomPlot * m_msePlot;
    QCustomPlot * m_bitFailPlot;

    QTextEdit * m_netText;
};

#endif // NETTRAINVIEW_HPP
