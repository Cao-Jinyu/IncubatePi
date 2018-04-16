#include "window.hpp"
#include <iostream>

Window::Window(PID *pid, TempReader *tempRead){
	
	// Attach pointers to PID controller and temperature sensor
    pidControl = pid;
    this->tempRead = tempRead;

    // Create knob for setting the target temperature
	knob = new QwtKnob;
	knob->setValue(pid->get_required_value() / 4);
	knob->setKnobWidth(100);
	connect( knob, SIGNAL(valueChanged(double)), SLOT(setTargetTemp(double)) );
	
	// Initialise plot data sets
    for( int index=0; index<plotDataSize; ++index ){
		xDataCurrent[index] = index;
		xDataTarget[index] = index;
		yDataCurrent[index] = 0;
		yDataTarget[index] = 0;
	}

    // Create new plots for both the current and target temperatures
	curveTarget = new QwtPlotCurve;
    curveCurrent = new QwtPlotCurve;
	plot = new QwtPlot;
    
	// Make a plot curve from the data and attach it to the plot
	curveCurrent->setSamples(xDataCurrent, yDataCurrent, plotDataSize);
	curveCurrent->attach(plot);
    curveTarget->setSamples(xDataTarget, yDataTarget, plotDataSize);
    curveTarget->attach(plot);
    
    // Update and display the plot
	plot->replot();
	plot->show();

	// Set up the window layout 
	vLayout = new QVBoxLayout;
	vLayout->addWidget(knob);
	hLayout = new QHBoxLayout;
	hLayout->addLayout(vLayout);
	hLayout->addWidget(plot);
	setLayout(hLayout);
}

void Window::timerEvent( QTimerEvent * ){   

    // Read the current and target temperatures
	double currentTemp = tempRead->readTemp();
    double targetTemp = pidControl->get_required_value();

	// Add the new input to the plot
	memmove( yDataCurrent, yDataCurrent+1, (plotDataSize-1) * sizeof(double) );
    memmove( yDataTarget, yDataTarget+1, (plotDataSize-1) * sizeof(double) );
	yDataCurrent[plotDataSize-1] = currentTemp;
    yDataTarget[plotDataSize-1] = targetTemp;
	curveCurrent->setSamples(xDataCurrent, yDataCurrent, plotDataSize);
	curveTarget->setSamples(xDataTarget, yDataTarget, plotDataSize);
	
	// Update the plot
	plot->replot();
}

void Window::setTargetTemp(double targetTemp){
	
	// Update the target temperature of the PID controller
    this->pidControl->update_required_value(4.0 * targetTemp);
}
