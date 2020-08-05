// forward decl
class bandp;

#ifndef _bandp
#define _bandp

typedef bandp *Pbandp;

//#include <stdlib.h>

#include <complex>
#include <stdlib.h>



/**
 * Implements a bandpass with 2 poles and no zeros. The time step is T=1
 * so that the frequency range is from 0 to 0.5 (Nyquist frequency).
 ***/
class bandp {
 public:
  /**
   * Constructor
   **/
  bandp();

  /**
   * Filter
   **/
  float filter(float value);

  /**
   * Calculates the coefficients from the typical bandpass parameters q and f.
   * The frequency is the normalized frequency in the range [0..0.5].
   **/
  void calcCoeffBandp(float f,float q);

  /**
   * Calculates the coefficients from the two conjugate poles. The parameters
   * are the coordinates in the s-plane (!). The transformation from the
   * s-plane to the z-plane is just now the impulsinvariant transform.
   **/
  void calcCoeffPoles(float r,float i);

  /**
   * Generates an acsii file with the impulse response of the filter.
   **/
  void impulse(const char* name);

  /**
   * the transfer function
   **/
  std::complex<float> h(std::complex<float> s);

  /**
   * normalization
   **/
  float norm;

  void calc_norm();

  /**
   * Generates an ASCII file with the transfer function
   **/
  void transfer(char* name);

  /**
   * Gets the actual output of the filter. Same as the return value
   * of the function "filter()".
   **/
  float getActualOutput() {return actualOutput;};

 private:
  /**
   * Real part of the pole
   **/
  double realPart;

  /**
   * Imaginary part of the pole
   **/
  double imagPart;

  /**
   * The first pole
   **/
  std::complex<float> s1;

  /**
   * The second pole
   **/
  std::complex<float> s2;

  /**
   * constant pi
   **/
  double pi;

  /**
   * constant 2pi
   **/
  double pi2;

  /**
   * The coefficients of the denominator of H(z)
   **/
  double denominator[3];

  /**
   * The coefficients of the enumerator of H(z)
   **/
  double enumerator[3];

  /**
   * Delay lines for the IIR-Filter
   **/
  double buffer[3];

  /**
   * The actual output of the filter (the return value of the filter()
   * function).
   **/
  float actualOutput;
};

#endif
