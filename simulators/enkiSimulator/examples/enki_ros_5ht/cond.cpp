#include "bandp.h"
#include "features.h"
#include "learner.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>



#define MAXSTEPS 1000


#define FREQUENCY 0.1
#define QUALITY 1


int main(int, char**) {
  bandp* testFilter=new bandp();
  
  testFilter->calcCoeffBandp(FREQUENCY,QUALITY);
  testFilter->impulse("impulse.dat");
  testFilter->transfer("transfer.dat");

  /**
	* Set up the differnt modules
	**/
  features* f=new features();
  learner* l=new learner();
  l->setIIR(FREQUENCY,QUALITY);

  /**
	* main-loop
	**/

  for(int i=0;i<MAXSTEPS;i++) {
	 f->classCondStim(i,5,30,100);
	 f->docu(i);

    l->prediction(i,f->feature);
    l->docu(i);
  }
  
  return 0;
}

