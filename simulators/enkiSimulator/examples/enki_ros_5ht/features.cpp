#include"features.h"


features::features() {
  fprintf(stderr,"Init features\n");
  fDocu=NULL;
  for(int i=0;i<MAXFEATURES;i++) {
	 feature[i]=0;
  }
}


features::~features() {
  if (fDocu) {
	 fclose(fDocu);
  }
}


void features::classCondStim(int step, int tLight, int tFood, int period) {
  if ((step%period)==tLight) {
	 feature[1]=1.0F;
  } else {
	 feature[1]=0.0F;
  }
  if ((step%period)==tFood) {
	 feature[0]=1.0F;
  } else {
	 feature[0]=0.0F;
  }
}







void features::docu(int step) {
  char* fileName="features.dat";
  if (!fDocu) {
	 fDocu=fopen(fileName,"wt");
	 if (!fDocu) {
		fprintf(stderr,"Could not open %s\n",fileName);
		exit(1);
	 }
  }
  fprintf(fDocu,"%d ",step);
  for(int i=0;i<MAXFEATURES;i++) {
	 fprintf(fDocu,"%f ",feature[i]);
  }
  fprintf(fDocu,"\n");
}



