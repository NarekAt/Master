#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scrollzoomer.h"
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_legend.h>

#include <QApplication>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QFile>
#include <QDebug>
#include <QLabel>

const QColor plotColors[] = {QColor(Qt::red), 
                             QColor(Qt::green), 
                             QColor(Qt::blue), 
                             QColor(Qt::cyan),
                             QColor(Qt::magenta),
                             QColor(Qt::yellow),
                             QColor(Qt::gray),
                             QColor(Qt::darkRed),
                             QColor(Qt::darkGreen),
                             QColor(Qt::darkBlue)};

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->save->setText("Save as image");
  connect(ui->save, SIGNAL(clicked()), this, SLOT(saveToImage()));
  ui->save->setMaximumWidth(100);
  ui->layout->setAlignment(ui->save, Qt::AlignHCenter);

  ui->plot->setTitle("4-cycle count");
  ui->plot->setAutoReplot();

  m_legend = new QwtLegend(this);
  m_legend->setItemMode(QwtLegend::ReadOnlyItem);
  ui->plot->insertLegend(m_legend, QwtPlot::TopLegend);

  m_grid = new QwtPlotGrid;
  m_grid->enableX(true);
  m_grid->enableXMin(true);
  m_grid->enableY(true);
  m_grid->enableYMin(true);
  m_grid->setMajPen(QPen(Qt::black, 0, Qt::DashLine)); 
  m_grid->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
  m_grid->attach(ui->plot);

  m_zoomer = new ScrollZoomer(ui->plot->canvas());
  m_zoomer->setRubberBandPen(QPen(Qt::blue));
  m_zoomer->setTrackerPen(QPen(Qt::blue));
  m_zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::CTRL);

  m_magnifier = new QwtPlotMagnifier(ui->plot->canvas());
  m_magnifier->setAxisEnabled(QwtPlot::xBottom, true);
  m_magnifier->setAxisEnabled(QwtPlot::yLeft, true);

  m_panner = new QwtPlotPanner(ui->plot->canvas());
  m_panner->setAxisEnabled(QwtPlot::xBottom, true);
  m_panner->setAxisEnabled(QwtPlot::yLeft, true);
  m_panner->setMouseButton(Qt::LeftButton, Qt::CTRL);

  ui->plot->setAxisTitle(QwtPlot::xBottom, "i, time");
  ui->plot->setAxisAutoScale(QwtPlot::xBottom);
  ui->plot->setAxisTitle(QwtPlot::yLeft, "n, count");
  ui->plot->setAxisAutoScale(QwtPlot::yLeft);

  QStringList args = QApplication::arguments();
  m_curves = new QwtPlotCurve*[args.size()];
  for(int k = 1; k < args.size(); ++k)
  {
    //QwtSymbol* symbol = new QwtSymbol();
    //symbol->setStyle(QwtSymbol::Ellipse);
    //symbol->setPen(QColor(Qt::black));
    //symbol->setSize(1);
    //m_curves[k]->setSymbol(symbol);
    QFile dataFile(args[k]);
    qDebug() << args[k];
    if(dataFile.open(QIODevice::ReadOnly))
    {
      QVector<QPointF> samples;
      QTextStream in(&dataFile);
      int N = 0;
      double p = 0.0;
      double mu = 0.0;
      in >> N >> p >> mu;
      m_curves[k] = new QwtPlotCurve(QString("N=%1, p=%2, u=%3").arg(N).arg(p).arg(mu));
      m_curves[k]->setRenderHint(QwtPlotItem::RenderAntialiased);
      m_curves[k]->setPen(QPen(plotColors[(k-1)%(sizeof(plotColors)/sizeof(plotColors[0]))]));
      while(!in.atEnd())
      {
        int i;
        double n;
        in >> i;
        in >> n;
        samples.push_back(QPointF(i, n));
      }
      samples.pop_back();
      m_curves[k]->setSamples(samples);
      m_curves[k]->attach(ui->plot);
    }
  }
  ui->plot->replot();
  m_zoomer->setZoomBase(false);

  ui->plot->canvas()->setCursor(Qt::ArrowCursor);
}

MainWindow::~MainWindow()
{
  delete m_legend;
  delete m_grid;
  delete m_zoomer;
  delete[] m_curves;

  delete ui;
}

void MainWindow::saveToImage()
{
  const double scaleFactor = 1.5; 
  const QSize sz(2048, 1024);

  QImage image(sz, QImage::Format_ARGB32);
  image.fill(QColor(Qt::white).rgb());

  QPainter painter(&image);
  painter.scale(scaleFactor, scaleFactor);

  QwtPlotRenderer renderer;
  renderer.render(ui->plot, &painter, QRectF(QPointF(0,0), sz/scaleFactor));
  if(image.isNull()){
    qDebug("Failed to capture the plot for saving");
    return;
  }
  QString types(	"JPEG file (*.jpeg);;"				// Set up the possible graphics formats
      "Portable Network Graphics file (*.png);;"
      "Bitmap file (*.bmp)");
  QString filter;							// Type of filter
  QString jpegExt=".jpeg", pngExt=".png", tifExt=".tif", bmpExt=".bmp", tif2Ext="tiff";		// Suffix for the files
  QString fn = QFileDialog::getSaveFileName(this, tr("Save Image"),
      QString(), types, &filter);

  if ( !fn.isEmpty() ) {						// If filename is not a null
    if (fn.contains(jpegExt)) {				// Remove file extension is already there
      fn.remove(jpegExt);
    }
    else if (fn.contains(pngExt)) {
      fn.remove(pngExt);
    }
    else if (fn.contains(bmpExt)) {
      fn.remove(bmpExt);
    }
    if (filter.contains(jpegExt)) {				// OR, Test to see if jpeg and save
      fn+=jpegExt;
      image.save( fn, "JPEG" );
    }
    else if (filter.contains(pngExt)) {			// OR, Test to see if png and save
      fn+=pngExt;
      image.save( fn, "PNG" );
    }
    else if (filter.contains(bmpExt)) {			// OR, Test to see if bmp and save
      fn+=bmpExt;
      image.save( fn, "BMP" );
    }
  }
}
