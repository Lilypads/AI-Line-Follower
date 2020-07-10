class Bandpass;

#ifndef _Bandpass
#define _Bandpass

#include <assert.h>

#include "globals.h"


/**
 * Creates memory traces at specified length. It's a 2nd order IIR filter.
 **/
class Bandpass {
public:
	/**
	 * Constructor
	 **/
	Bandpass();
	
	/**
	 * Filter
	 **/
	double filter(double v);

	/**
	 * Calculates the coefficients
	 * The frequency is the normalized frequency in the range [0..0.5].
	 **/
	void calcPolesZeros(double f,double r);

	/**
	 * sets the filter parameters
	 **/
	void setParameters(double frequency, double Qfactor);

	/**
	 * Generates an acsii file with the impulse response of the filter.
	 **/
	void impulse(char* name);

	/**
         * Normalises the output with f
         **/
	void calcNorm(double f);

	/**
	 * Generates an ASCII file with the transfer function
	 **/
	void transfer(char* name);

	/**
	 * Gets the output of the filter. Same as the return value
	 * of the function "filter()".
	 **/
    inline double getOutput() {return actualOutput;}

	/**
	 * Sets the output to zero again
	 **/
	void reset();

private:

	/**
	 * normalization
	 **/
    double norm=0;

	/**
	 * The coefficients of the denominator of H(z)
	 **/
    double denominator0=0;
    double denominator1=0;
    double denominator2=0;

	/**
	 * The coefficients of the enumerator of H(z)
	 **/
    double enumerator0=0;
    double enumerator1=0;
    double enumerator2=0;

	/**
	 * Delay lines for the IIR-Filter
	 **/
    double buffer0=0;
    double buffer1=0;
    double buffer2=0;

	/**
	 * The actual output of the filter (the return value of the filter()
	 * function).
	 * Normalised
	 **/
    double actualOutput=0;

    double output=0;
};

#endif
