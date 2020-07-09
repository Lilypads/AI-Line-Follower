#ifndef FILTER_H
#define FILTER_H

#include <cmath>
#include <complex>

// #include "mat.h"

class Filter
{
public:
	virtual const float &filter(const float &input)=0;
	virtual void reset()=0;
	virtual const float &output() const { return output_; }

	const float &operator()(const float &input) { return filter(input); }
	operator float() const { return output_; }
	const float *operator&() { return &output_; }

protected:
	Filter(const float &baseline=0) : output_(0), baseline_(baseline) {}
	float output_;
	float baseline_;
  const float & baseline() { return baseline_; }

	static inline void fill_float_array(float *array, const unsigned &size, const float &val)
	{
		float *end = array + size;
		while( array < end )
			*(array++) = val;
	}
};

class SimpleDecayLowpassFilter : public Filter
{
public:
	SimpleDecayLowpassFilter(unsigned d) : hold(0), a1(exp(-1.0 / d)) {}
	const float &filter(const float &input) { return hold = input + hold*a1; }
	void reset() { hold = 0; }

private:
	float hold;
	float a1;

};

class SecondOrderFilter : public Filter
{
public:
	SecondOrderFilter(float a0, float a1, float a2, float b1, float b2) : a0(a0), a1(a1), a2(a2), b1(b1), b2(b2), hold1(0), hold2(0) {}
	SecondOrderFilter(float gain, std::complex<float> zero, std::complex<float> pole);
	const float &filter(const float &input);
	void reset() { hold1 = 0; hold2 = 0; }

protected:
	SecondOrderFilter() : hold1(0), hold2(0) {}
	float a0, a1, a2, b1, b2;
	float hold1, hold2;
};

class SecondOrderLowpassFilter : public SecondOrderFilter
{
public:
	SecondOrderLowpassFilter(float frequency=0.01, float qFactor=0.49);
};

class SecondOrderLowpassFilterNormalisedImpulse : public SecondOrderFilter
{
public:
	SecondOrderLowpassFilterNormalisedImpulse(float frequency=0.01, float qFactor=0.49);
};

class SecondOrderHighpassFilter : public SecondOrderFilter
{
public:
	SecondOrderHighpassFilter(float frequency=0.01, float qFactor=0.71);
};

class FIRFilter : public Filter
{
public:
	FIRFilter(const float *coefficients, const unsigned &taps, const float &baseline=0);
	FIRFilter(const char *matFileName, const float &baseline=0, const char *variableName="impulse");
	~FIRFilter();

	const float &filter(const float &input);
	void reset() { fill_float_array(buffer, taps, baseline_); offset = 0; }

private:
	const float *coefficients;
	float       *buffer;
	unsigned      taps, offset;

	// mxArray  *matArray; // only used when loading coefficients from a .mat file
};

template <unsigned N>
class DelayFilter : public Filter
{
public:
	DelayFilter(const float baseline=0) : Filter(baseline), offset(0) { fill_float_array(buffer, N, baseline_); }

	const float &filter(const float &input)
	{
		buffer[offset] = input;

		if( ++offset >= N )
			offset = 0;

		return output_ = buffer[offset];
	}

	void reset() { fill_float_array(buffer, N, baseline_); offset = 0; }

private:
	float buffer[N];
	unsigned offset;
};

#endif
