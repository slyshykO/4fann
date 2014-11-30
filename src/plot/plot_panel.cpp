/*******************************************************************************
*  file    : plot_panel.cpp
*  created : 12.04.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#include "plot_panel.hpp"
#include <QDebug>
#include <plot.hpp>

TPlotPanel::TPlotPanel(QWidget *parent) :
    QWidget(parent)
{
    m_grid = new QGridLayout();
    setLayout(m_grid);
}

TPlot *TPlotPanel::addPlot(QString name, int row, int col, int rowSpan,  int colSpan)
{
    TPlot * plot = new TPlot(this);
    plot->setObjectName(name);
    QObject::connect(plot, SIGNAL(newCurve(QString)), this, SLOT(addNewCurve(QString)) );
    m_grid->addWidget(plot, row, col, rowSpan, colSpan);
    return plot;
}

TPlot *TPlotPanel::plot(const QString &name)
{
    TPlot * plot = 0;

    QList<TPlot*> plotList = findChildren<TPlot*>(QRegExp(name));

    if(plotList.size())
        plot = plotList[0];

    return plot;
}

QGridLayout *TPlotPanel::grid()
{
    return m_grid;
}

const QStringList &TPlotPanel::seriesNames() const
{
    return seriesNames_;
}

void TPlotPanel::addNewCurve(const QString &ncrv)
{
    seriesNames_ << ncrv ;
}

