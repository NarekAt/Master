#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class QwtLegend;
class QwtPlotGrid;
class QwtPlotCurve;
class ScrollZoomer;
class QwtPlotMagnifier;
class QwtPlotPanner;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void saveToImage();

private:
    Ui::MainWindow* ui;

    QwtLegend* m_legend;
    QwtPlotGrid* m_grid; 
    QwtPlotCurve** m_curves;
    ScrollZoomer* m_zoomer;
    QwtPlotMagnifier* m_magnifier;
    QwtPlotPanner* m_panner;
};

#endif // MAINWINDOW_H
