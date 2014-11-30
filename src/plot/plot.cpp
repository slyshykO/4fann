/*
* file   : plot.cpp created 2012.3.7
* class  : TPlot
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "plot.hpp"
#include <QBoxLayout>
#include <qwt_symbol.h>
#include <qwt_legend_label.h>

TPlot::TPlot(QWidget *parent) :
    QWidget(parent)
{
    plot_ = new QwtPlot(this);
    legend_ = new QwtLegend;
    plot_->insertLegend( legend_, QwtPlot::TopLegend );

    zoom_ = new QwtChartZoom(plot_, this);
    zoom_->setRubberBandColor(Qt::red);

    // Делаем легенды кликабельными
    legend_->setDefaultItemMode( QwtLegendData::Checkable );
    connect( legend_, SIGNAL( checked(QVariant,bool,int)) ,
        SLOT( showCurve(QVariant,bool,int) ) );

    QVBoxLayout * plot_layout = new QVBoxLayout();
    plot_layout->addWidget(plot_);
    setLayout(plot_layout);
    this->setMinimumHeight(100);
}

void TPlot::addCurve(const QString & title, const QVector<QPointF> &data)
{
    const char *colors[] =
    {
        "lightseagreen",
        "purple",
        "red",
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
    curve->setTitle  ( title );
    curve->setPen    ( QPen( QColor( colors[ curves_.size() % numColors ] ), 2 ) );
    curve->setSamples( data );
    curve->attach    ( plot_ );

    QwtSymbol * symb = new QwtSymbol(QwtSymbol::VLine);
    symb->setSize(5);
    symb->setColor(curve->pen().color().darker());
    curve->setSymbol(symb);

    curve->setStyle(QwtPlotCurve::Lines);

    curves_.append(curve);
    showCurve(plot_->itemToInfo(curve), true);

    emit newCurve(title);
}

QwtPlotCurve *TPlot::curve(int idx)
{
    if( idx < curves_.size() )
        return curves_.at(idx);

    return 0;
}

QwtPlotCurve *TPlot::curve(const QString &title)
{
    auto curve = std::find_if(curves_.begin(),curves_.end(),[&title](const QwtPlotCurve* crv){return (title == crv->title().text());});
    if(curve != curves_.end())
        return (*curve);
    return nullptr;
}

QwtPlot * TPlot::plot()
{
    return plot_;
}

QwtInterval TPlot::zone()
{
    QwtPlotZoneItem* zone = zones_[QStringLiteral("default")];
    if(zone == 0)
        {
            return QwtInterval(0.,0.);
        }

    return zone->interval();
}

QwtPlotZoneItem *TPlot::zone(const QString &zone_name)
{
    return zones_[zone_name] ;
}

void TPlot::setZone(double s, double e, const QString &zone_name)
{
    QwtPlotZoneItem* zone = zones_[zone_name];
    if(zone == 0)
        {
            zone = new QwtPlotZoneItem();
            zone->setPen( Qt::darkGray );
            zone->setBrush( QColor( "white" /*"#834358"*/ ) );
            zone->setOrientation( Qt::Vertical );
            zone->attach( plot_ );
            zones_[zone_name] = zone;
        }

    if(e > s)
        {
            zone->setInterval(s,e);
        }
    else
        {
            zone->detach();
            delete zone;
            zone = 0;
            zones_.remove(zone_name);
        }

    plot_->replot();
}

void TPlot::clear()
{
    plot_->detachItems(QwtPlotItem::Rtti_PlotItem, true);
    curves_.clear();
    zones_.clear();
}

void TPlot::replot()
{
    plot_->replot();
}

void TPlot::showCurve(const QVariant &itemInfo, bool on, int index)
{
    QwtPlotItem *plotItem = plot_->infoToItem( itemInfo );

    if ( plotItem->rtti() == QwtPlotItem::Rtti_PlotCurve )
        {
            QwtPlotCurve *curve = static_cast<QwtPlotCurve *>( plotItem );
            curve->setVisible(on);

            QwtLegend *lgd = qobject_cast<QwtLegend *>( plot_->legend() );

            QList<QWidget *> legendWidgets =
                lgd->legendWidgets( plot_->itemToInfo( curve ) );

            if ( legendWidgets.size() == 1 )
                {
                    QwtLegendLabel *legendLabel =
                        qobject_cast<QwtLegendLabel *>( legendWidgets[0] );

                    if ( legendLabel )
                        legendLabel->setChecked( on );
                }
        }

    plot_->replot();

    Q_UNUSED(index)
}


