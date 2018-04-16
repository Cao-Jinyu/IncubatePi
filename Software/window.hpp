#ifndef WINDOW_H
#define WINDOW_H

#include <qwt/qwt_thermo.h>
#include <qwt/qwt_knob.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include "TempReader.hpp"
#include "PID.hpp"

#include <QBoxLayout>

/* 
    This class implements a window for displaying the the current and 
    target temperatures on a graph. It also implements a control knob
    for setting the target temperature.
      
*/
class Window : public QWidget {
	
	// must include the Q_OBJECT macro for for the Qt signals/slots framework to work with this class
	Q_OBJECT

    public:
    
        // Creates a new Window with the specified PID controller and temperature sensor.
	    Window(PID *pid, TempReader *temp); 

        // Called when plotted data is to be updated
	    void timerEvent( QTimerEvent * );

    public slots:
    
        // Updates the target temperature when the knob is turned
	    void setTargetTemp(double targetTemp);

    private:
    
	    QwtKnob      *knob;            // Knob object for setting target temperature
	    QwtPlot      *plot;            // Graph plot object for displaying current and target temperatures
	    QwtPlotCurve *curveCurrent;    // Current ambient temperature plot 
	    QwtPlotCurve *curveTarget;     // Target ambient temperature plot
        PID          *pidControl;      // PID controller associated with the graph plots
        TempReader   *tempRead;        // Temperature sensor associated with the graph plots

	    QVBoxLayout  *vLayout;         // Vertical layout element from Qt
	    QHBoxLayout  *hLayout;         // Horizontal layout element from Qt
    
	    static const int plotDataSize = (25 * 10 * 1);   // Plot x-axis size

	    double xDataCurrent[plotDataSize];    // X-axis data for the current ambient temperature plot
	    double yDataCurrent[plotDataSize];    // y-axis data for the current ambient temperature plot
        double xDataTarget[plotDataSize];     // X-axis data for the target ambient temperature plot
	    double yDataTarget[plotDataSize];     // Y-axis data for the target ambient temperature plot
            
};

#endif 
