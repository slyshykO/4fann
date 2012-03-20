/*
* file   : plot.hpp created 2012.3.7
* class  : TPlot
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef PLOT_HPP
#define PLOT_HPP

#include <QMainWindow>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include "qwtchartzoom.h"

class TPlot : public QWidget
{
    Q_OBJECT
public:
    explicit TPlot(QWidget *parent = 0);
    void addCurve(const QString &, const QVector< QPointF > &);
    QwtPlotCurve* curve(int idx);
    QwtPlot * plot();
signals:

public slots:
    void clear();
    void replot();
    void showCurve( QwtPlotItem *, bool on );

private:
    QwtPlot * plot_;
    QwtLegend *legend_;
    QwtChartZoom * zoom_;
    QList<QwtPlotCurve*> curves_;
};

#endif // PLOT_HPP
