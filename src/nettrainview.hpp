/*******************************************************************************
*  file    : nettrainview.hpp
*  created : 14.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#ifndef NETTRAINVIEW_HPP
#define NETTRAINVIEW_HPP

#include <QWidget>

class TPlot;
class QTextEdit;


class NetTrainView : public QWidget
{
    Q_OBJECT
public:
    explicit NetTrainView(QWidget *parent = 0);

    void setNetText(const QString & txt);
    void setMSEData(const QVector<float> & , const QVector<uint> & );
    void setBitFailData(const QVector<uint> & , const QVector<uint> & );
    void setTestMSEData(const QVector<float> & , const QVector<uint> & );
    void setTestBitFailData(const QVector<uint> & , const QVector<uint> & );
private:
    TPlot * m_msePlot;
    TPlot * m_bitFailPlot;

    QTextEdit * m_netText;
};

#endif // NETTRAINVIEW_HPP
