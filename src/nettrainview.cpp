/*******************************************************************************
*  file    : nettrainview.cpp
*  created : 14.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#include "nettrainview.hpp"
#include <QDebug>
#include <QTextEdit>
#include <QBoxLayout>
#include <plot.hpp>

NetTrainView::NetTrainView(QWidget *parent) :
    QWidget(parent)
{
    m_msePlot     = new TPlot(this);
    m_bitFailPlot = new TPlot(this);
    m_bitFailPlot->setMinimumSize(100,100);
    m_msePlot->setMinimumSize(100,100);
    m_msePlot->replot();
    m_msePlot->addCurve(tr("MSE"), QVector<QPointF>());
    m_msePlot->addCurve(tr("Test MSE"), QVector<QPointF>());
    m_bitFailPlot->addCurve(tr("Bit fail"), QVector<QPointF>());
    m_bitFailPlot->addCurve(tr("Test Bit fail"), QVector<QPointF>());

    m_netText = new QTextEdit(this);
    //m_netText->setMaximumSize(150,150);
    m_netText->setMaximumWidth(500);

    QVBoxLayout * plot_layout = new QVBoxLayout();
    plot_layout->addWidget(m_msePlot);
    plot_layout->addWidget(m_bitFailPlot);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(m_netText);
    layout->addLayout(plot_layout);
    layout->setStretchFactor(m_netText,1);
    layout->setStretchFactor(plot_layout,2);

    this->setLayout(layout);
}

void NetTrainView::setNetText(const QString &txt)
{
    m_netText->setText(txt);
}

void NetTrainView::setMSEData(const QVector<float> & data, const QVector<uint> &steps)
{
    QVector<QPointF> crv;
    for(int i = 0; i < data.size(); i++)
        crv.append(QPointF(steps[i],data[i]));

    m_msePlot->curve(0)->setSamples(crv);
    m_msePlot->replot();
}

void NetTrainView::setBitFailData(const QVector<uint> & data, const QVector<uint> &steps)
{
    QVector<QPointF> crv;
    for(int i = 0; i < data.size(); i++)
        crv.append(QPointF(steps[i],data[i]));

    m_bitFailPlot->curve(0)->setSamples(crv);
    m_bitFailPlot->replot();
}

void NetTrainView::setTestMSEData(const QVector<float> &data, const QVector<uint> &steps)
{
    QVector<QPointF> crv;
    for(int i = 0; i < data.size(); i++)
        crv.append(QPointF(steps[i],data[i]));

    m_msePlot->curve(1)->setSamples(crv);
    m_msePlot->replot();
}

void NetTrainView::setTestBitFailData(const QVector<uint> &data, const QVector<uint> &steps)
{
    QVector<QPointF> crv;
    for(int i = 0; i < data.size(); i++)
        crv.append(QPointF(steps[i],data[i]));

    m_bitFailPlot->curve(1)->setSamples(crv);
    m_bitFailPlot->replot();
}
