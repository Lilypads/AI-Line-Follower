  /*****************************************************************
        THE DIRECTION 
  ******************************************************************/
#include"direction.h"

Direction::Direction() 
{
  char tmp[]="direction.dat";
  f=fopen(tmp,"wt");
  if (!f) 
    {
      fprintf(stderr,"Could not open %s\n",tmp);
      exit(1);
    }
  step=0; 
  dirleft1=0.0;
  dirright1=0.0;
  doDocu=0;
  bandpLeft1=new bandp();
  bandpLeft1->calcCoeffBandp(0.01,0.6);
  bandpRight1=new bandp();
  bandpRight1->calcCoeffBandp(0.01,0.6);
}


void Direction::doDirection(float l1,float r1,float sw) 
{	
//	if (sw>thres) {
//		sw = 1;
		speed = sw;
//	} else {
//		speed = thres;
//	}
	dirleft1=bandpLeft1->filter(l1);
	dirright1=bandpRight1->filter(r1);
	Motorout=(dirleft1-dirright1)*sw;   
	if (f) 
	{
		fprintf(f,"%ld %f %f %f\n",step,Motorout,dirleft1,dirright1);
	}
	step++;
}	

/**
 * Write gnuplot friendly output
 **/
void Direction::writeDocu(int step) 
{
	if (!doDocu) return;

	fprintf(f,"%d %f %f %f\n",step,Motorout,dirleft1,dirright1);
	step++;

}
	
Direction::~Direction()
 {
	if (doDocu) 
	  {
	    fclose(f);
	  }
	if (f) fclose(f);
	delete bandpLeft1;
	delete bandpRight1;
 }
