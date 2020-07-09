#ifndef _Direction
#define _Direction

#include "bandp.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Implements a direction circuit.
 **/

class Direction 
{
 public:
  //construct
  Direction();

 public:
  //destruct
  ~Direction();

 public:
  //filters the left and right input for the motor direction

  void doDirection(float left1,float right1,float sw); 

 public:
  //document direction circuit.

  void writeDocu(int step);


public:
  /**
   * The output of the direction circuit
   **/
  float getOutput() 
  {
    return Motorout;
  };

  float getSpeed() {
	  return speed;
  }
	
private:
  /**
   * Docu of the signals and output
   **/
  FILE* f;	

 private:
  /**
   * The actual motor output direction circuit
   **/
  float Motorout;

  float thres = 0.05;

 private:
  // the step time.
  long int step = 0;

 private:
	float dirleft1 = 0; 
 private:
	float dirright1 = 0;

	float speed = 0;
	
 /**
  * LP for the left direction
  **/
	bandp* bandpLeft1 = NULL;

 /**
  * LP for the right direction
  **/
	bandp* bandpRight1 = NULL;

 private:
  int doDocu;

};

#endif
