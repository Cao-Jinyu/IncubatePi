#include "window.h"
// #include "adcreader.h"

#include <cmath>  // for sine stuff


Window::Window() : gain(5), count(0)
{
	knob = new QwtKnob;
	// set up the gain knob
	knob->setValue(gain);

	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
	connect( knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );
    

	curveSet = new QwtPlotCurve;
    curveCurrent = new QwtPlotCurve;
	plot = new QwtPlot;
	// make a plot curve from the data and attach it to the plot
	curveCurrent->setSamples(xDataCurrent, yDataCurrent, plotDataSize);
	curveCurrent->attach(plot);
    curveSet->setSamples(xdataSet, yDataSet, plotDataSize);
    curveSet->attach(plot);

	plot->replot();
	plot->show();


	// set up the layout - knob above thermometer
	vLayout = new QVBoxLayout;
	vLayout->addWidget(knob);

	// plot to the left of knob and thermometer
	hLayout = new QHBoxLayout;
	hLayout->addLayout(vLayout);
	hLayout->addWidget(plot);

	setLayout(hLayout);
}

Window::~Window() {
	// tells the thread to no longer run its endless loop
//	adcreader->quit();
	// wait until the run method has terminated
//	adcreader->wait();
//	delete adcreader;
}

void Window::timerEvent( QTimerEvent * )
{   

    //These variables are the inputs to the graph. They need to be changed to be inputs from the PID controller
	double currentTemp = gain * sin( M_PI * count/50.0 );
    double setTemp = 0.8 * gain;
	++count;

	// add the new input to the plot
	memmove( yDataCurrent, yDataCurrent+1, (plotDataSize-1) * sizeof(double) );
    memmove( yDataSet, yDataSet+1, (plotDataSize-1) * sizeof(double) );
	yDataCurrent[plotDataSize-1] = currentTemp;
	curveCurrent->setSamples(xDataCurrent, yDataCurrent, plotDataSize);
	curveSet->setSamples(xDataSet, yDataSet, plotDataSize);
	plot->replot();
}


// this function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// for example purposes just change the amplitude of the generated input
	this->gain = gain;
}
