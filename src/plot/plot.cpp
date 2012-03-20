/*
* file   : plot.cpp created 2012.3.7
* class  : TPlot
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "plot.hpp"
#include <QBoxLayout>
#include <qwt_legend_label.h>

TPlot::TPlot(QWidget *parent) :
    QWidget(parent)
{
    plot_ = new QwtPlot(this);
    legend_ = new QwtLegend;
    //legend_->setDefaultItemMode( QwtLegendData::Checkable );
    plot_->insertLegend( legend_, QwtPlot::TopLegend );

    zoom_ = new QwtChartZoom(plot_, this);
    zoom_->setRubberBandColor(Qt::red);

    connect( legend_, SIGNAL( checked( QwtPlotItem *, bool, int ) ),
        SLOT( showCurve( QwtPlotItem *, bool ) ) );

    QVBoxLayout * plot_layout = new QVBoxLayout();
    plot_layout->addWidget(plot_);
    setLayout(plot_layout);
}

void TPlot::addCurve(const QString & title, const QVector<QPointF> &data)
{
    const char *colors[] =
    {
        "LightSalmon",
        "HotPink",
        "Fuchsia",
        "PaleGreen",
        "PaleTurquoise",
        "SteelBlue",
        "Cornsilk",
        "Peru",
        "Maroon"
    };
    const int numColors = sizeof( colors ) / sizeof( colors[0] );

    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle( title );
    curve->setPen( QPen( QColor( colors[ curves_.size() % numColors ] ), 2 ) );
    curve->setSamples(data);
    curve->attach( plot_ );

    curve->setStyle(QwtPlotCurve::Lines);

    curves_.append(curve);
}

QwtPlotCurve *TPlot::curve(int idx)
{
    if( idx < curves_.size() )
        return curves_.at(idx);

    return 0;
}

QwtPlot * TPlot::plot()
{
    return plot_;
}

void TPlot::clear()
{
    std::for_each(curves_.begin(), curves_.end(), [](QwtPlotCurve * c){c->detach();delete c;});
    curves_.clear();
}

void TPlot::replot()
{
    plot_->replot();
}

void TPlot::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible( on );

    QList<QWidget *> legendWidgets =
        qobject_cast<QwtLegend *>( plot_->legend() )->legendWidgets( item );

    if ( legendWidgets.size() == 1 )
    {
        QwtLegendLabel *legendLabel =
            qobject_cast<QwtLegendLabel *>( legendWidgets[0] );

        if ( legendLabel )
            legendLabel->setChecked( on );
    }

    plot_->replot();
}
