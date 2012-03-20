/*******************************************************************************
*  file    : nettrainview.hpp
*  created : 14.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#ifndef NETTRAINVIEW_HPP
#define NETTRAINVIEW_HPP

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(TPlot)
QT_FORWARD_DECLARE_CLASS(QTextEdit)


class NetTrainView : public QWidget
{
    Q_OBJECT
public:
    explicit NetTrainView(QWidget *parent = 0);

    void setNetText(const QString & txt);
    void setMSEData(const QVector<float> & , uint step = 1);
    void setBitFailData(const QVector<uint> & , uint step = 1);
private:
    TPlot * m_msePlot;
    TPlot * m_bitFailPlot;

    QTextEdit * m_netText;
};

#endif // NETTRAINVIEW_HPP
