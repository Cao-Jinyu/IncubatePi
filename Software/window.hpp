#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <qwt/qwt_knob.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include "TempReader.hpp"
#include "PID.hpp"

#include <QBoxLayout>

// #include "adcreader.h"

// class definition 'Window'
class Window : public QWidget
{
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

public:
	Window(AmbientTempPID *pid, TempReader *temp); // default constructor - called when a Window is declared without arguments

	~Window();

	void timerEvent( QTimerEvent * );

public slots:
	void setGain(double gain);

// internal variables for the window class
private:
	QwtKnob      *knob;
	QwtPlot      *plot;
	QwtPlotCurve *curveCurrent;
	QwtPlotCurve *curveSet;
    AmbientTempPID *pidControl;
    TempReader *tempRead;

	// layout elements from Qt itself http://qt-project.org/doc/qt-4.8/classes.html
	QVBoxLayout  *vLayout;  // vertical layout
	QHBoxLayout  *hLayout;  // horizontal layout
    
    //With the current refresh rate of 40ms, the graph will only display 1 second of data for every 25 samples
	static const int plotDataSize = (25 * 60 * 10);

	// data arrays for the plot
	double xDataCurrent[plotDataSize];
	double yDataCurrent[plotDataSize];
    double xDataSet[plotDataSize];
	double yDataSet[plotDataSize];

	double gain;
	int count;

//	ADCreader *adcreader;
};

#endif // WINDOW_H
