/*
* file   : plot.hpp created 2012.3.7
* class  : TPlot
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef PLOT_HPP
#define PLOT_HPP

#include <QMap>
#include <qwt.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_zoneitem.h>
#include "qwtchartzoom.h"

class QGridLayout;

class TPlot : public QWidget
{
    Q_OBJECT
public:
    explicit TPlot(QWidget *parent = 0);
    void addCurve(const QString &, const QVector< QPointF > &);
    QwtPlotCurve *curve(int idx);
    QwtPlotCurve *curve(const QString& title);
    QwtPlot      *plot ( );
    QwtInterval   zone ( );
    QwtPlotZoneItem* zone(const QString& zone_name);
signals:
    void newCurve(const QString&);
public slots:
    void clear();
    void replot();
    void showCurve( const QVariant &itemInfo, bool on, int index = 0);
    void setZone(double s, double e, const QString& zone_name = "default");
private:
    QwtPlot             *plot_;
    QwtLegend           *legend_;
    QwtChartZoom        *zoom_;
    QMap<QString,QwtPlotZoneItem*> zones_;
    QList<QwtPlotCurve*> curves_;
};

#endif // PLOT_HPP
