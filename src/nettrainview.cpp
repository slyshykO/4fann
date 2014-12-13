/*******************************************************************************
*  file    : nettrainview.cpp
*  created : 14.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#include "nettrainview.hpp"
#include <QDebug>
#include <QTextEdit>
#include <QBoxLayout>
#include <qcustomplot.h>

NetTrainView::NetTrainView(QWidget *parent) :
    QWidget(parent)
{
    m_msePlot     = new QCustomPlot(this);
    m_bitFailPlot = new QCustomPlot(this);
    m_bitFailPlot->setMinimumSize(100,100);
    m_msePlot->setMinimumSize(100,100);
    m_msePlot->replot();
    m_msePlot->addGraph();
    m_msePlot->addGraph();
    m_bitFailPlot->addGraph();
    m_bitFailPlot->addGraph();
    m_bitFailPlot->replot();

//    m_msePlot->addCurve(tr("MSE"), QVector<QPointF>());
//    m_msePlot->addCurve(tr("Test MSE"), QVector<QPointF>());
//    m_bitFailPlot->addCurve(tr("Bit fail"), QVector<QPointF>());
//    m_bitFailPlot->addCurve(tr("Test Bit fail"), QVector<QPointF>());
    m_msePlot->graph(0)->setPen(QPen(QColor("red")));
    m_msePlot->graph(1)->setPen(QPen(QColor("blue")));

    m_bitFailPlot->graph(0)->setPen(QPen(QColor("cyan")));
    m_bitFailPlot->graph(1)->setPen(QPen(QColor("magenta")));

    m_netText = new QTextEdit(this);
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

void NetTrainView::setMSEData(const QVector<double> & data, const QVector<double> &steps)
{
    m_msePlot->graph(0)->setData(steps,data);
    m_msePlot->graph(0)->rescaleAxes();
    m_msePlot->replot();
}

void NetTrainView::setBitFailData(const QVector<double> &data, const QVector<double> &steps)
{
    m_bitFailPlot->graph(0)->setData(steps,data);
    m_bitFailPlot->graph(0)->rescaleAxes();
    m_bitFailPlot->replot();
}

void NetTrainView::setTestMSEData(const QVector<double> &data, const QVector<double> &steps)
{
    m_msePlot->graph(1)->setData(steps,data);
    m_msePlot->graph(1)->rescaleAxes(true);
    m_msePlot->replot();
}

void NetTrainView::setTestBitFailData(const QVector<double> &data, const QVector<double> &steps)
{
    m_bitFailPlot->graph(1)->setData(steps,data);
    m_bitFailPlot->graph(1)->rescaleAxes(true);
    m_bitFailPlot->replot();
}
