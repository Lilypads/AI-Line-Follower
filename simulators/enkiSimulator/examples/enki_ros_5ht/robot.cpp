
#include "robot.h"
#include "world.h"

#include <sys/types.h>
#include <sys/stat.h>

// delay in timesteps (150 is a good starting point for a short delay)
int REWARD_DELAY = 150;

Robot::Robot(World* ww,
	     int index,
	     float angle,
	     float xPos,
	     float yPos,
	     float w,
	     float l) {
	world=ww;
	fprintf(stderr,
		"start init Robot(%d,%f,%f,%f,%f,%f)\n",
		index,angle,xPos,yPos,w,l);
	time2settle=0;

	/**
	 * Index of the robot
	 **/
	roboIndex=index;

	/**
	 * min change of the angle
	 **/
	minDphi=-0.7;

	/**
	 * max change of the angle
	 **/
	maxDphi=0.7;

	// init the absolute angle
	phi=angle;

	// init the relative angle
	dPhi=0;

	// init the old angle for the calc of the derivative
	dPhi_old=0;

	// At the beginning there is no sensor-event
	sensorOut=-1;

	/**
	 * collision right front
	 **/
	collFrontLeft=0;

	/**
	 * collision left front
	 **/
	collFrontRight=0;

	/**
	 * collision right rear
	 **/
	collRearRight=0;

	/**
	 * collision left rear
	 **/
	collRearLeft=0;

	/**
	 * sees an object in the left dir
	 **/
	seeLeft=0;

	/**
	 * sees an onject in the right dir
	 **/
	seeRight=0;
	
	//left and right direction switches
	Greensw=0;
	Bluesw=0;

	xCoord=xPos;
	yCoord=yPos;
	width=w;
	length=l;
	dStep=1;
	dStep_old=1;
	stickCounter=0;
	ioDataCount=0;

	left2dStep=new float[BUMP_N_FILTERS];
	right2dStep=new float[BUMP_N_FILTERS];
	left2dPhi=new float[BUMP_N_FILTERS];
	right2dPhi=new float[BUMP_N_FILTERS];
	dStep2dPhi=new float[BUMP_N_FILTERS];
	dPhi2dStep=new float[BUMP_N_FILTERS];
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		left2dStep[i]=0;
		right2dStep[i]=0;
		left2dPhi[i]=0;
		right2dPhi[i]=0;
		dStep2dPhi[i]=0;
		dPhi2dStep[i]=0;
	}

	int numPoints=(int)(w*l)+1;
	xBuffer=new float[numPoints];
	yBuffer=new float[numPoints];
	nBuffer=0;

	/**
	 * docu
	 **/
	char tmp[128];
	mkdir("temp",0700);
	sprintf(tmp,"temp/coordinates%02d.dat",roboIndex);
	fCoord=fopen(tmp,"wt");
	if (!fCoord) {
		fprintf(stderr,"Error with %s\n",tmp);
		exit(1);
	}
	  
	sprintf(tmp,"temp/weights%02d.dat",roboIndex);
	fWeights=fopen(tmp,"wb");
	if (!fWeights) {
		fprintf(stderr,"Error with %s\n",tmp);
		exit(1);
	}


	sprintf(tmp,"temp/iodata%02d.dat",roboIndex);
	fIO=fopen(tmp,"wb");
	if (!fIO) {
		fprintf(stderr,"Error with %s\n",tmp);
		exit(1);
	}	
	char tmpD[256];	
	sprintf(tmpD,"Direction.dat");
	fdir=fopen(tmpD,"wt");
	if (!fdir) 
	{
		fprintf(stderr,"Could not open %s\n",tmpD);
		exit(1);
	}


	bandpFront=new bandp();
	setTime2settle(BUMP_F_REFLEX);
	bandpFront->calcCoeffBandp(BUMP_F_REFLEX,
				   BUMP_Q_REFLEX);
	bandpFront->impulse("front_impulse.dat");

	int reactTimeBumpReflex=(int)(10.0/BUMP_F_REFLEX);
		
	bandpRear=new bandp();
	bandpRear->calcCoeffBandp(BUMP_F_REFLEX,
				  BUMP_Q_REFLEX);
		
	bandpLeft=new bandp();
	bandpLeft->calcCoeffBandp(BUMP_F_REFLEX,
				  BUMP_Q_REFLEX);
		
	bandpRight=new bandp();
	bandpRight->calcCoeffBandp(BUMP_F_REFLEX,
				   BUMP_Q_REFLEX);
	

	// max response time for the resonator with the lowest frequency
	int reactTimeVision=(int)(10.0/(BUMP_F_LEARN/BUMP_N_FILTERS));

	bandpLeftEye=new bandp*[BUMP_N_FILTERS];
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		bandpLeftEye[i]=new bandp();
		float f=BUMP_F_LEARN/((float)((BUMP_LEARN_FACT)*i+1));
		bandpLeftEye[i]->calcCoeffBandp(f,BUMP_Q_LEARN);
		setTime2settle(f);
	}
		
	bandpRightEye=new bandp*[BUMP_N_FILTERS];
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		bandpRightEye[i]=new bandp();
		bandpRightEye[i]->
			calcCoeffBandp(BUMP_F_LEARN/
				       ((float)((BUMP_LEARN_FACT)*i+1)),
				       BUMP_Q_LEARN);
	}

	bandpDphi=new bandp*[BUMP_N_FILTERS];
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		bandpDphi[i]=new bandp();
		bandpDphi[i]->calcCoeffBandp(BUMP_F_LEARN/
					     ((float)((BUMP_LEARN_FACT*i)+1)),
					     BUMP_Q_LEARN);
	}
		
	bandpDstep=new bandp*[BUMP_N_FILTERS];
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		bandpDstep[i]=new bandp();
		bandpDstep[i]->calcCoeffBandp(BUMP_F_LEARN/
					      ((float)((BUMP_LEARN_FACT*i)+1)),
					      BUMP_Q_LEARN);
	}

	bandp_da_dphi=new bandp();
	bandp_da_dphi->calcCoeffBandp(BUMP_F_LEARN*0.1/BUMP_N_FILTERS,
				      BUMP_Q_LEARN);

	bandp_da_ds=new bandp();
	bandp_da_ds->calcCoeffBandp(BUMP_F_LEARN*0.1/BUMP_N_FILTERS,
				    BUMP_Q_LEARN);


	// this deals with all the learning
	limbic_system=new Limbic_system();
	Greendirection=new Direction();
	Bluedirection=new Direction();

	ioEventDuration=reactTimeBumpReflex;
	if (reactTimeVision>ioEventDuration) {
		ioEventDuration=reactTimeVision;
	}
	setTime2settle(10);
	fprintf(stderr,"Learning starts after %d learning steps\n",time2settle);
	fprintf(stderr,"finished init Robot\n");
}



void Robot::setTime2settle(float f) {
	int t=20*(int)(1.0/f);
	if (t>time2settle) {
		time2settle=t;
	}
}



Robot::~Robot() {
	fclose(fWeights);
	fclose(fCoord);
	fclose(fIO);
	fclose(fdir);
	// tidy up
	delete[] left2dStep;
	delete[] right2dStep;
	delete[] left2dPhi;
	delete[] right2dPhi;
	delete[] dStep2dPhi;
	delete[] dPhi2dStep;
	delete[] xBuffer;
	delete[] yBuffer;
	delete bandpFront;
	delete bandpRear;
	delete bandpLeft;
	delete bandpRight;

	for(int i=0;i<BUMP_N_FILTERS;i++) {
		delete bandpLeftEye[i];
	}
	delete bandpLeftEye;
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		delete bandpRightEye[i];
	}
	delete bandpRightEye;
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		delete bandpDphi[i];
	}
	delete bandpDphi;
	for(int i=0;i<BUMP_N_FILTERS;i++) {
		delete bandpDstep[i];
	}
	delete bandpDstep;
	delete bandp_da_dphi;
	delete bandp_da_ds;
	//
	delete limbic_system;
	delete Greendirection;
	delete Bluedirection;
}



void Robot::docCoord(long int step) {
	fprintf(fCoord,"%ld %d %d %f\n",step,(int)xCoord,(int)yCoord,phi);
	//fprintf(stderr,"%ld %d %d %f\n",step,(int)xCoord,(int)yCoord,phi);
	fflush(fCoord);
}




float Robot::rotx(float phi,float x,float y) {
	return ((float)x*cos(phi)+(float)y*sin(phi));
}



float Robot::roty(float phi,float x,float y) {
	return (-(float)x*sin(phi)+(float)y*cos(phi));
}




void Robot::draw() {
	// Draw the new robot position
	nBuffer=0;
	for(float i=-(length/2);i<=(length/2);i=i+1.0) {
		float xp=xCoord+rotx(phi,width/2,i);
		float yp=yCoord+roty(phi,width/2,i);
		xBuffer[nBuffer]=(int)xp;
		yBuffer[nBuffer]=(int)yp;
		nBuffer++;
		xp=xCoord+rotx(phi,-width/2,i);
		yp=yCoord+roty(phi,-width/2,i);
		xBuffer[nBuffer]=(int)xp;
		yBuffer[nBuffer]=(int)yp;
		nBuffer++;
	}
	for(float i=-(width/2);i<=(width/2);i=i+1.0) {
		float xp=xCoord+rotx(phi,i,length/2);
		float yp=yCoord+roty(phi,i,length/2);
		xBuffer[nBuffer]=(int)xp;
		yBuffer[nBuffer]=(int)yp;
		nBuffer++;  
		xp=xCoord+rotx(phi,i,-length/2);
		yp=yCoord+roty(phi,i,-length/2);
		xBuffer[nBuffer]=(int)xp;
		yBuffer[nBuffer]=(int)yp;
		nBuffer++;  
	}
	if (roboIndex>6) {
		for(float i=-(width/2);i<=(width/2);i=i+1.0) {
			float xp=xCoord+rotx(phi,i,0);
			float yp=yCoord+roty(phi,i,0);
			xBuffer[nBuffer]=(int)xp;
			yBuffer[nBuffer]=(int)yp;
			nBuffer++;  
		}
		for(float i=-(length/2);i<=(length/2);i=i+1.0) {
			float xp=xCoord+rotx(phi,0,i);
			float yp=yCoord+roty(phi,0,i);
			xBuffer[nBuffer]=(int)xp;
			yBuffer[nBuffer]=(int)yp;
			nBuffer++;
		}
	}
}


void Robot::move(int step) {
	// Erase the old robot position
	for(int i=0;i<nBuffer;i++) {
		world->getPoint(xBuffer[i],yBuffer[i])->resetRobot(roboIndex);
	}
	if (dPhi>maxDphi) {
		dPhi=maxDphi;
	}
	if (dPhi<minDphi) {
		dPhi=minDphi;
	}
	if (dStep>1.0) {
		dStep=1.0;
	}
	if (dStep<-1.0) {
		dStep=-1.0;
	}
	// see in  the world
	float dx=width/length;
	int tmpSeeLeft=0;
	int tmpSeeRight=0;
	float l2=length/2-1;
	float dxstep=dx/10;
	for(float dxrun=dxstep;dxrun<dx;dxrun=dxrun+dxstep) {
		for(float y=0;y<RF_DEPTH;y=y+1) {
			float xp1=xCoord+rotx(phi,-(y+l2)*dxrun,(y+l2));
			float yp1=yCoord+roty(phi,-(y+l2)*dxrun,(y+l2));
			if (world->isObstacle(xp1,yp1)) {
				tmpSeeRight++;
			}
			float xp2=xCoord+rotx(phi,+(y+l2)*dxrun,(y+l2));
			float yp2=yCoord+roty(phi,+(y+l2)*dxrun,(y+l2));
			if (world->isObstacle(xp2,yp2)) {
				tmpSeeLeft++;
			}
		}
	}
	hyst(seeLeft,tmpSeeLeft);
	hyst(seeRight,tmpSeeRight);

	// collide in the world :-)

	float phiBak=phi;
	float xBak=xCoord;
	float yBak=yCoord;
	phi=phi+dPhi;
	xCoord=xCoord+sin(phi)*dStep;
	yCoord=yCoord+cos(phi)*dStep;
	draw();
	// detect collitions
	int collision=0;
	for(int i=0;i<nBuffer;i++) {
		if (world->getPoint(xBuffer[i],yBuffer[i])->isObstacle()) {
			collision=1;
		}
	}
	react(step,collision);
	if (collision) {
		xCoord=xBak;
		yCoord=yBak;
		phi=phiBak;
		if (stickCounter>STICK_LIMIT) {
			if (((stickCounter-STICK_LIMIT)%100)<50) {
				phi=phi+M_PI/2;
			} else {
				phi=phi-M_PI/2;
			}	
			xCoord=xBak+sin(phi)*dStep;
			yCoord=yBak+cos(phi)*dStep;
		}
		draw();
	}
	for(int i=0;i<nBuffer;i++) {
		world->getPoint(xBuffer[i],yBuffer[i])->setRobot(roboIndex);
	}
}



/**
 * A sort of hysteresis to prevent very short pulses in the sensors.
 * With detect=true the counter is set to 3. With detect=false the
 * counter is decreased. Thus for an if clause it remains true for
 * 3 time steps.
 **/
void Robot::hyst(int &counter,int detect) {
	if (detect) {
		counter=3;
	} else {
		if (counter>0) {
			counter--;
		}
	}
}


void Robot::react(int step,int collision) {
#ifdef RANDWALK
	/**
	 * Strength of the random effect
	 **/
	float randEff=0.19;
#endif

	/********************************************************************
	 * Collision
	 ********************************************************************/
	// now detect the collision
	if (collision) {
		stickCounter++;
		if (stickCounter>STICK_LIMIT) {
			fprintf(stderr,"%d: Stick. \n",stickCounter);
		}

		int fr=0;
		float xp;
		float yp;
		for(float w=0;w<=width/2;w=w+0.5) {
			xp=xCoord+rotx(phi,-w,length/2);
			yp=yCoord+roty(phi,-w,length/2);
			if (world->isObstacle(xp,yp)) {
				fr++;
			}
		}
		for(float l=0;l<=length/2;l=l+0.5) {
			xp=xCoord+rotx(phi,-width/2,l);
			yp=yCoord+roty(phi,-width/2,l);
			if (world->isObstacle(xp,yp)) {
				fr++;
			}
		}
		hyst(collFrontRight,fr);
    
		int fl=0;
		for(float w=0;w<=width/2;w=w+0.5) {    
			xp=xCoord+rotx(phi,w,length/2);
			yp=yCoord+roty(phi,w,length/2);
			if (world->isObstacle(xp,yp)) {
				fl++;
			}
		}
		for(float l=0;l<=length/2;l=l+0.5) {
			xp=xCoord+rotx(phi,width/2,l);
			yp=yCoord+roty(phi,width/2,l);
			if (world->isObstacle(xp,yp)) {
				fl++;
			}
		}
		hyst(collFrontLeft,fl);
    
		xp=xCoord+rotx(phi,width/2,-length/2);
		yp=yCoord+roty(phi,width/2,-length/2);
		hyst(collRearRight,world->isObstacle(xp,yp));
    
		xp=xCoord+rotx(phi,-width/2,-length/2);
		yp=yCoord+roty(phi,-width/2,-length/2);
		hyst(collRearLeft,world->isObstacle(xp,yp));
	} else {

		stickCounter=0;
		hyst(collFrontRight,0);
		hyst(collFrontLeft,0);
		hyst(collRearRight,0);
		hyst(collRearLeft,0);
	}
  
/**
**************************************************************************
* CENTRAL BEHAVIOR SECTION: AVOIDANCE
**/

	// learning rates
	float mu=     MU_DS_BUMP;
	float muPhi=  MU_PHI_BUMP;
	float muCross=0;

	// derivative
	float derivStep=dStep-dStep_old;
	float derivPhi=dPhi-dPhi_old;
	dStep_old=dStep;
	dPhi_old=dPhi;

	// different resonator results
	float lEye[BUMP_N_FILTERS];
	float rEye[BUMP_N_FILTERS];
	float resDstep[BUMP_N_FILTERS];
	float resDphi[BUMP_N_FILTERS];

	// sum of the different resonators
	float sumStep=0;
	float sumPhi=0;

	float sl=(float)(seeLeft>0);
	float sr=(float)(seeRight>0);

	float cf=(float)((collFrontRight>0)||
			 (collFrontLeft>0));
	float cb=(float)((collRearRight>0)||(collRearLeft>0));

	float cl=(float)(collFrontLeft>0);
	float cr=(float)(collFrontRight>0);


	// collision detector front/back
	float f=bandpFront->filter(cf);
	float b=bandpRear->filter(cb);


	// collision detector left/right
	float l=bandpLeft->filter(cl);
	float r=bandpRight->filter(cr);

	float multds=bandp_da_ds->filter(f-b)/DAMPING;
	multds=fabs(multds);
	
	float multphi=bandp_da_dphi->filter(l-r)/DAMPING;
	multphi=fabs(multphi);

	//fprintf(stderr,"%f %f\n",multds,multphi);

	for(int i=0;i<BUMP_N_FILTERS;i++) {

		lEye[i]=bandpLeftEye[i]->filter(sl);
		rEye[i]=bandpRightEye[i]->filter(sr);

		resDstep[i]=bandpDstep[i]->filter(derivStep);
		resDphi[i]=bandpDphi[i]->filter(dPhi);
		
		// let the resonators settle before learning
		if (step>time2settle) {
			left2dStep[i]=left2dStep[i]+mu*lEye[i]*derivStep*multds;
			right2dStep[i]=right2dStep[i]+mu*rEye[i]*derivStep*multds;
			
			left2dPhi[i]=left2dPhi[i]+muPhi*lEye[i]*derivPhi*multphi;
			right2dPhi[i]=right2dPhi[i]+muPhi*rEye[i]*derivPhi*multphi;

			// obsolete
			dPhi2dStep[i]=dPhi2dStep[i]+muCross*resDphi[i]*derivStep;
			dStep2dPhi[i]=dStep2dPhi[i]+muCross*resDstep[i]*derivPhi;
		}

		sumStep=sumStep+
			left2dStep[i]*lEye[i]+
			right2dStep[i]*rEye[i]+
			dPhi2dStep[i]*resDphi[i];

		sumPhi=sumPhi+
			left2dPhi[i]*lEye[i]+
			right2dPhi[i]*rEye[i]+
			dStep2dPhi[i]*resDstep[i];

	}


	
	/*********************************************************
	 * FOOD
	 *********************************************************/
	
	// REFLEX

	float reward=0;
	float xp;
	float yp;

	foodFr=0;
	
       	for(float w=width/2-1;w<=width/2;w=w+0.5) {
		xp=xCoord+rotx(phi,-w,length/2);
		yp=yCoord+roty(phi,-w,length/2);
		if (world->isFood(step,xp,yp)) {
			foodFr++;
		}
		if (world->isReward(step,xp,yp)) {

			reward=1;
			sensorOut = 1;

			if (!eatenFlag)
				nEaten++;
			if (reward>0) eatenFlag = 1;
		}
	}
	for(float l=length/2-1;l<=length/2;l=l+0.5) {
		xp=xCoord+rotx(phi,-width/2,l);
		yp=yCoord+roty(phi,-width/2,l);
		if (world->isFood(step,xp,yp)) {
			foodFr++;
		}
		if (world->isReward(step,xp,yp)) {
		
			reward=1;
			sensorOut = 1;
			
			if (!eatenFlag)
				nEaten++;
			if (reward>0) eatenFlag = 1;
		}
	}
    
	foodFl=0;
	for(float w=width/2-1;w<=width/2;w=w+0.5) {    
		xp=xCoord+rotx(phi,w,length/2);
		yp=yCoord+roty(phi,w,length/2);
		if (world->isFood(step,xp,yp)) {
			foodFl++;
		}
		if (world->isReward(step,xp,yp)) {

			reward=1;
			sensorOut = 1;

			if (!eatenFlag)
				nEaten++;
			if (reward>0) eatenFlag = 1;
		}
	}
	for(float l=length/2-1;l<=length/2;l=l+0.5) {
		xp=xCoord+rotx(phi,width/2,l);
		yp=yCoord+roty(phi,width/2,l);
		if (world->isFood(step,xp,yp)) {
			foodFl++;
		}
		if (world->isReward(step,xp,yp)) {

			reward=1;
			sensorOut = 1;

			if (!eatenFlag)
				nEaten++;
			if (reward>0) eatenFlag = 1;
		}
	}

	if (collision) {
		sensorOut=0;
	}

	/**
	float lf=0;
	if (foodFl) lf=1;
	float rf=0;
	if (foodFr) rf=1;

	float prox_explore=(lf-rf);
	**/

	// LEARNED
	// now: the robot "hears" the food

	// sum up the sound-distances to the robot
	float diffSound1=0;
	float diffSound2=0;
	// adds sound information
	float sumSound1=0;
	float sumSound2=0;
	
	/** These values are sent to direction.cpp to control the direction of the robot**/

	// the left and right direction for the food the sound distance
	float leftGreen=0;
	float leftBlue=0;
	float rightGreen=0;
	float rightBlue=0;
		
	float on_contact_direction_Green=0;
	float on_contact_direction_Blue=0;

	// loop through all food sources
	for(int i=0;i<2;i++) {
		if (world->getFoodValid(i)) {
			// get the coordinates of a food-bit
			int xf=world->getFoodX(i);
			int yf=world->getFoodY(i);
			// get the coordinate of the right ear
			float xr=xCoord+rotx(phi,-width/2,length/2);
			float yr=yCoord+roty(phi,-width/2,length/2);
			// get the coordinate of the left ear
			float xl=xCoord+rotx(phi,width/2,length/2);
			float yl=yCoord+roty(phi,width/2,length/2);
			// calculate the distance from the right ear
			// to the food-bit
			float dr=sqrt(pow(xf-xr,2)+pow(yf-yr,2));
			// calculate the distance from the left ear
			// to the food-bit
			float dl=sqrt(pow(xf-xl,2)+pow(yf-yl,2));
			// calc the field of view
			float nphi = fmodf(phi,M_PI*2);
			if (nphi>M_PI) {
				nphi -= M_PI*2;
			}
			if (nphi<-M_PI) {
				nphi += M_PI*2;
			}
			float viewing_angle = atan2f(xf-xCoord,yf-yCoord) - nphi;
			//printf("%d=%f,%f,(%d,%d),(%f,%f)\n",i,viewing_angle,nphi,xf,yf,xCoord,yCoord);
			if (yf>(MAXY/2))
			{
				if ((dl>1)&&(dr>1)&&(fabs(viewing_angle)<(EYES_VIEWING_ANGLE))) 
				{
					diffSound2=diffSound2+(dl-dr)/sqrt(sqrt(dl*dr));
					sumSound2=sumSound2+FOOD_DIAMETER/5/sqrt(sqrt(dl*dr));
					leftBlue=leftBlue+(dr)/sqrt(sqrt(dr*dr));
					rightBlue=rightBlue+(dl)/sqrt(sqrt(dl*dl));

				}

			}
			else
			{	
				if ((dl>1)&&(dr>1)&&(fabs(viewing_angle)<(EYES_VIEWING_ANGLE)))
				{
					diffSound1=diffSound1+(dl-dr)/sqrt(sqrt(dl*dr));
					sumSound1=sumSound1+FOOD_DIAMETER/5/sqrt(sqrt(dl*dr));
					leftGreen=leftGreen+(dr)/sqrt(sqrt(dr*dr));
					rightGreen=rightGreen+(dl)/sqrt(sqrt(dl*dl));
				}
				
			}
		}
	}
	
	if (sumSound1<0) sumSound1=0;
	if (sumSound2<0) sumSound2=0;

	float placefield1=isPlacefield(0);
	float placefield2=isPlacefield(1);
	
	if ((foodFl)||(foodFr))	{	
		on_contact_direction_Green = 0.0;
		on_contact_direction_Blue = 0.0;
		
		if ((placefield2>placefield1)) {	
			on_contact_direction_Green = 0.0;    		
			on_contact_direction_Blue = 1.0;
		}
		
		if ((placefield1>placefield2)) {
			on_contact_direction_Blue = 0.0;
			on_contact_direction_Green = 1.0;		
		}
	}  
	
	float visual_landmark_Green=sumSound1;
	float visual_landmark_Blue=sumSound2;

	// when the reward is visible
	float visual_reward_Green = 0;
	float visual_reward_Blue = 0;

	world->setRewardVisible(0);

	if ((fabs(placefield1)>0)&&(!(world->getSwapFlag()))) {
		if (rewardDelayGreen>0) {
			visual_reward_Green = 0;
			rewardDelayGreen--;
			reward = 0;
		}
	} else {
		rewardDelayGreen = REWARD_DELAY;
	}
	if (rewardDelayGreen==0) {
			world->setRewardVisible(1);
			visual_reward_Green = 1;
	}

	if ((fabs(placefield2)>0)&&(world->getSwapFlag())) {
		if (rewardDelayBlue>0) {
			visual_reward_Blue = 0;
			rewardDelayBlue--;
			reward = 0;
		}
	} else {
		rewardDelayBlue = REWARD_DELAY;
	}
	if (rewardDelayBlue==0) {
			world->setRewardVisible(2);
			visual_reward_Blue = 1;
	}

	if ((reward>0)&&(!rewardFlag)) {
		rewardFlag = 1;
		nReward++;
	}

	limbic_system->doStep(reward,
			      placefield1,
			      placefield2,
			      on_contact_direction_Green,
			      on_contact_direction_Blue,
			      visual_landmark_Green,
			      visual_landmark_Blue,
			      visual_reward_Green,
			      visual_reward_Blue);
	int stp=0;	

	// The switches for the direction controller to the dark or light green place fields.
	//the ditection triggers by the corelg and coredg
	Greensw=limbic_system->getGreenOutput();
	Bluesw=limbic_system->getBlueOutput();
	float exploreLeft=limbic_system->getExploreLeft();
	float exploreRight=limbic_system->getExploreRight();

	if (Greensw > 1) {
		Greensw=1;
	}
	if (Bluesw > 1) {
		Bluesw=0;
	}

	Greendirection->doDirection(leftGreen,rightGreen,Greensw);
	Bluedirection->doDirection(leftBlue,rightBlue,Bluesw);

//	fprintf(stderr,"%f %f\n",Greensw,Bluesw);

	/** document the direction**/
	int writeDir=1;
	if (writeDir) 
	{
		fprintf(fdir,"%d %f %f %f %f %f %f %f %f\n",
			stp,                           //01
			Greendirection->getOutput(),      //02
			leftGreen,                        //03
			rightGreen,                       //04
			Bluedirection->getOutput(),      //05
			leftBlue,                        //06
			rightBlue,                       //07
			Greensw,                          //08
			Bluesw                           //09
			);	
		stp++;
	}	    
	/*******************************************************
   END FOOD
	********************************************************/

	float Greenspeed = Greendirection->getSpeed();
	float Bluespeed = Bluedirection->getSpeed();

	// summation of the front collision
	dStep=ROBOT_SPEED*(Greenspeed+Bluespeed+exploreLeft+exploreRight)-BUMP_REVERSE_GAIN*f+BUMP_REVERSE_GAIN*b+sumStep;

	float foodPhi = Greendirection->getOutput()+Bluedirection->getOutput()+(exploreLeft-exploreRight)*20;

	float randPhi=0;
#ifdef RANDWALK
	// random, not yet used
	if ((!(step%100))&&(fabs(sumPhi)<0.001)&&(foodPhi<0.001)) {
		//fprintf(stderr,"%d rand \n",step);
		randPhi=(((float)random())/((float)RAND_MAX)-0.5)*randEff;
	}
#endif

	// summation left/right
	dPhi=BUMP_STEER_GAIN*(l-r)+
		sumPhi+
		randPhi+
		FOOD_GAIN*foodPhi;
	
	/***************************
	 * Docu
	 ***************************/
	// write out the weights of the vision-sensors as raw data
	fwrite(left2dStep,sizeof(left2dStep[0]),BUMP_N_FILTERS,fWeights);
	fwrite(right2dStep,sizeof(right2dStep[0]),BUMP_N_FILTERS,fWeights);
	fwrite(left2dPhi,sizeof(left2dPhi[0]),BUMP_N_FILTERS,fWeights);
	fwrite(right2dPhi,sizeof(right2dPhi[0]),BUMP_N_FILTERS,fWeights);

#ifdef DISPLAY_PROGRESS
	if ((seeLeft||seeRight)&&((step%10)==0)) {
		fprintf(stderr,"%d ",step);
		if (seeLeft) {
			fprintf(stderr,"seeLeft ");
		}
		if (seeRight) {
			fprintf(stderr,"seeRight ");
		}
		fprintf(stderr,"\n");
		fflush(stderr);
	}

	if ((collFrontRight)||
	    (collFrontLeft)||
	    (collRearRight)||
	    (collRearLeft)) {
		fprintf(stderr,"%d ",step);
		if (collFrontRight)
			fprintf(stderr,"collFrontRight ");
		if (collFrontLeft)
			fprintf(stderr,"collFrontLeft  ");
		if (collRearRight)
			fprintf(stderr,"collRearRight  ");
		if (collRearLeft)
			fprintf(stderr,"collRearLeft   ");
		fprintf(stderr,"\n");
	}
#endif

	if (seeLeft||seeRight) { // vision-signal
		ioDataCount=ioDataCount+ioEventDuration;
	}

	if (ioDataCount>0) {
		ioDataCount--;
		ioData.collFrontLeft=collFrontLeft;
		ioData.collFrontRight=collFrontRight;
		ioData.collRearRight=collRearLeft;
		ioData.seeLeft=seeLeft;
		ioData.seeRight=seeRight;
		ioData.phi=phi;
		ioData.dPhi=dPhi;
		ioData.dStep=dStep;
		fwrite(&ioData,sizeof(ioData_t),1,fIO);
	}
};

int Robot::getSensors() {
	int temp=sensorOut;
	sensorOut=-1;
	return temp;
}

int Robot::isPlacefield(int index) {
	return world->isPlacefield(index,xCoord,yCoord);
}

