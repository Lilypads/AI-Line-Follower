#include <string.h>

#include "filter.h"

const float &SecondOrderFilter::filter(const float &input)
{
	float hold = input - b1*hold1 - b2*hold2;

	output_ = a0*hold + a1*hold1 + a2*hold2;

	hold2 = hold1;
	hold1 = hold;

	return output_;
}

// valid second order filters have poles and zeros which are complex conjugates of each other - only need one zero and one pole as parameters
SecondOrderFilter::SecondOrderFilter(float gain, std::complex<float> zero, std::complex<float> pole) :
  a0(gain),
  a1( -2 * gain * zero.real() ),
  a2( gain * ( zero.real()*zero.real() + zero.imag()*zero.imag() ) ),
  b1( -2 * pole.real() ),
  b2( (pole.real()*pole.real() + pole.imag()*pole.imag()) ),
  hold1(0),
  hold2(0)
{}

SecondOrderLowpassFilter::SecondOrderLowpassFilter(float frequency, float qFactor)
{
	float Omega = 2 * tan( M_PI * frequency );
	float Omega2 = Omega * Omega;

	float c0 = 2 * Omega / qFactor;
	float c1 = 4 + c0 + Omega2;

	a0 = Omega2 / c1;
	a1 = 2 * a0;
	a2 = a0;

	b1 = 2 * (Omega2 - 4) / c1;
	b2 = (4 - c0 + Omega2) / c1;
}

SecondOrderLowpassFilterNormalisedImpulse::SecondOrderLowpassFilterNormalisedImpulse(float frequency, float qFactor)
{
	float Omega = 2 * tan( M_PI * frequency );
	float Omega2 = Omega * Omega;

	float c0 = 2 * Omega / qFactor;
	float c1 = 4 + c0 + Omega2;

	a0 = 1;
	a1 = 2;
	a2 = 1;

	b1 = 2 * (Omega2 - 4) / c1;
	b2 = (4 - c0 + Omega2) / c1;

	float max = filter(1);

	while(filter(0) >= max)
		max = output_;

	a0 /= max;
	a1 /= max;
	a2 /= max;

	reset();
}

SecondOrderHighpassFilter::SecondOrderHighpassFilter(float frequency, float qFactor)
{
	float Omega = 2 * tan( M_PI * frequency );
	float Omega2 = Omega * Omega;

	float c0 = 2 * Omega / qFactor;
	float c1 = 4 + c0 + Omega2;

	a0 = 4 / c1;
	a1 = -2 * a0;
	a2 = a0;

	b1 = 2 * (Omega2 - 4) / c1;
	b2 = (4 - c0 + Omega2) / c1;
}

FIRFilter::FIRFilter(const float *coefficients, const unsigned &taps, const float &baseline) :
  Filter(baseline),
  coefficients(coefficients),
  buffer(new float[taps]),
  taps(taps),
  offset(0)
{
}

FIRFilter::FIRFilter(const char *, const float &baseline, const char *) :
  Filter(baseline),
  offset(0)
{
}

FIRFilter::~FIRFilter()
{
	delete[] buffer;

}

const float &FIRFilter::filter(const float &input)
{
	const float *coeff     = coefficients;
	const float *coeff_end = coefficients + taps;

	float *buf_val = buffer + offset;

	*buf_val = input;
	output_ = 0;

	while(buf_val >= buffer)
		output_ += *buf_val-- * *coeff++;

	buf_val = buffer + taps-1;

	while(coeff < coeff_end)
		output_ += *buf_val-- * *coeff++;

	if(++offset >= taps)
		offset = 0;


	return output_;
}
