#include "window.hpp"
// #include "adcreader.h"

#include <cmath>  // for sine stuff


Window::Window(PID *pid, TempReader *tempRead) : gain(40), count(0)
{
    pidControl = pid;
    this->tempRead = tempRead;

	knob = new QwtKnob;
	// Set up the gain knob
	knob->setValue(gain);

	// Use the Qt signals/slots framework to update the gain -
	// Every time the knob is moved, the setGain function will be called
	connect( knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	curveSet = new QwtPlotCurve;
    curveCurrent = new QwtPlotCurve;
	plot = new QwtPlot;
    
	// Make a plot curve from the data and attach it to the plot
	curveCurrent->setSamples(xDataCurrent, yDataCurrent, plotDataSize);
	curveCurrent->attach(plot);
    curveSet->setSamples(xDataSet, yDataSet, plotDataSize);
    curveSet->attach(plot);

	plot->replot();
	plot->show();

	// Set up the layout - knob above thermometer
	vLayout = new QVBoxLayout;
	vLayout->addWidget(knob);

	// Plot to the left of knob and thermometer
	hLayout = new QHBoxLayout;
	hLayout->addLayout(vLayout);
	hLayout->addWidget(plot);

	setLayout(hLayout);
}

Window::~Window() {
	// Tells the thread to no longer run its endless loop
    // adcreader->quit();
	// Wait until the run method has terminated
    // adcreader->wait();
    // delete adcreader;
}

void Window::timerEvent( QTimerEvent * )
{   

    // These variables are the inputs to the graph. They need to be changed to be inputs from the PID controller
	double currentTemp = tempRead->readTemp();
    double setTemp = gain;
	++count;

	// Add the new input to the plot
	memmove( yDataCurrent, yDataCurrent+1, (plotDataSize-1) * sizeof(double) );
    memmove( yDataSet, yDataSet+1, (plotDataSize-1) * sizeof(double) );
	//yDataCurrent[plotDataSize-1] = currentTemp;
    //yDataSet[plotDataSize-1] = setTemp;
	curveCurrent->setSamples(xDataCurrent, yDataCurrent, plotDataSize);
	curveSet->setSamples(xDataSet, yDataSet, plotDataSize);
	plot->replot();
}


// This function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// For example purposes, just change the amplitude of the generated input
	this->gain = gain;
    //pidControl->update_required_value((float)gain);
}
