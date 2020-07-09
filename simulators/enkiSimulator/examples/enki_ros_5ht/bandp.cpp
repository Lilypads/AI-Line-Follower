#include "bandp.h"
#include <math.h>
#include <complex>
#include <stdio.h>


// the filter


bandp::bandp() {
  pi=acos(-1);
  pi2=pi*(float)2.0;           
  buffer[0]=0;
  buffer[1]=0;
  buffer[2]=0;
  actualOutput=0.0F;
  norm=1;
}



void bandp::impulse(const char* name) {
  int steps=100;
#ifdef DEBUG_BP
  fprintf(stderr,"Impulse resp: %s, %d steps\n",name,steps);
#endif
  for(int i=0;i<steps;i++) {
    filter(0);
  }
  float input=0.0;
  FILE* ff=fopen(name,"wt");
  if (!ff) {
    fprintf(stderr,"Couldn't open %s \n",name);
    return;
  }
  for(int i=0;i<steps;i++) {
    if (i==10) {
      input=1.0F;
    } else {
      input=0.0F;
    }
    fprintf(ff,"%d %f\n",i,filter(input));
  }
  fclose(ff);
}


void bandp::calc_norm() {
  int steps=200;
  for(int i=0;i<steps;i++) {
    filter(0);
  }
  float input=0.0;
  float norm_tmp=0;
  norm=1.0;
  for(int i=0;i<steps;i++) {
    if (i==10) {
      input=1.0F;
    } else {
      input=0.0F;
    }
    float f=filter(input);
    if (f>norm_tmp) {
      norm_tmp=f;
    }
  }
  if (norm_tmp>0) {
    norm=norm_tmp;
  }
}



std::complex<float> bandp::h(std::complex<float> s) {
  return std::complex<float>(1,0)/((s+s1)*(s+s2));
}



void bandp::transfer(char* name) {
  int steps=100;
#ifdef DEBUG_BP
  fprintf(stderr,"Transfer function: %s, %d steps\n",name,steps);
#endif
  FILE* ff=fopen(name,"wt");
  if (!ff) {
    fprintf(stderr,"Couldn't open %s \n",name);
    return;
  }
  for(int i=0;i<steps;i++) {
    float f=((float)i)/((float)steps)*0.5;
    std::complex<float> iw=std::complex<float>(0,f*pi2);
    fprintf(ff,"%f %f\n",f,(float)sqrt(real(h(iw)*h(-iw))));
  }
  fclose(ff);
}






float bandp::filter(float value) {
  float input=0.0;
  float output=0.0;
  // a little bit cryptic but a little bit optimized for speed
  input=value;
  output=(enumerator[1]*buffer[1]);
  input=input-(denominator[1]*buffer[1]);
  output=output+(enumerator[2]*buffer[2]);
  input=input-(denominator[2]*buffer[2]);
  output=output+input*enumerator[0];
  buffer[2]=buffer[1];
  buffer[1]=input;
  output=output/norm;
  actualOutput=output;
  return output;
}




void bandp::calcCoeffPoles(float r,float i) {
  realPart=r;
  imagPart=i;
  s1=std::complex<float>(r,i);
  s2=std::complex<float>(r,-i);
  enumerator[0]=0;
  enumerator[1]=0; //1;
  enumerator[2]=1; //-1;
  denominator[0]=1;
  denominator[1]=real(-exp(s2)-exp(s1));
  denominator[2]=real(exp(s1+s2));
  calc_norm();
#ifdef DEBUG_BP
  fprintf(stderr,"real=%f,imag=%f, coeff.: 1=%f, 2=%f, n=%f\n",
          realPart,imagPart,denominator[1],denominator[2],norm);
#endif
}


void bandp::calcCoeffBandp(float f,float q) {
#ifdef DEBUG_BP
  fprintf(stderr,"f=%f, Q=%f, ",f,q);
#endif
  float fTimesPi;
  if (q>0) { // quality is ok
    fTimesPi=f*pi2;
    float e=fTimesPi/(2.0*q);
    if ((fTimesPi*fTimesPi-e*e)>0) { // root ok
      float w=sqrt(fTimesPi*fTimesPi-e*e);
      calcCoeffPoles(-e,w);
    } else { // root
      fprintf(stderr,"bandpass: The quality is too low\n");
      exit(-1);
    } // 
  } else { // Q bad
    fprintf(stderr,"bandpass: The quality is rubbish!\n");
    exit(-1);
  } // Q bad
}












