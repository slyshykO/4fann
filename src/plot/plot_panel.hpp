/*******************************************************************************
*  file    : plot_panel.hpp
*  created : 12.04.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#ifndef PLOT_PANEL_HPP
#define PLOT_PANEL_HPP

#include <QWidget>
#include <QGridLayout>

class TPlot;

class TPlotPanel : public QWidget
{
    Q_OBJECT
public:
    explicit TPlotPanel(QWidget *parent = 0);
    TPlot * addPlot(QString name, int row, int col, int rowSpan = 1, int colSpan = 1);
    TPlot * plot(const QString & name);
    QGridLayout* grid();
    const QStringList& seriesNames() const;
signals:
    
private slots:
    void addNewCurve(const QString& ncrv);
private:
    QGridLayout * m_grid;
    QStringList   seriesNames_;
};

#endif // PLOT_PANEL_HPP
