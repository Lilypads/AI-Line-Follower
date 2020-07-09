#ifndef _robot
#define _robot
#include <stdio.h>
#include "world.h"
#include "bandp.h"
#include "limbic-system-model.h"
#include "direction.h"

class robot;
typedef robot* Probot;

// delay in timesteps (150 is a good starting point for a short delay)
extern int REWARD_DELAY;

enum {BIG_T_NOTHING,BIG_T_VISION,BIG_T_BUMP,BIG_T_AFTER_BUMP};

struct ioData_t {
	short int collFrontLeft;
	short int collFrontRight;
	short int collRearRight;
	short int collRearLeft;
	short int seeLeft;
	short int seeRight;
	float phi;
	float dPhi;
	float dStep;
};


/**
 * The robot itself
 **/
class Robot {

 public:
	Robot(World* ww,int index,float angle,float x,float y,float w,float h);

 public:
	~Robot();

	/**
	 * The index of the robot
	 **/
 private:
	int roboIndex;

 public:
	/**
	 * x-Coordinate of the robot
	 **/
	float xCoord;
	float oldXcoord;

 public:
	/**
	 * y-Coordinate of the robot
	 **/
	float yCoord;
	float oldYcoord;

 public:
	int stickCounter;

 public:
	int getSensors();

 protected:
	/**
	 * angle of the robot in absolute coordinates
	 **/
	float phi;

 public:
	void setPhi(float phiTmp) {phi=phiTmp;};

 protected:
	/**
	 * last dPhi
	 **/
	float dPhi_old;
  
 protected:
	/**
	 * angle of the steering wheel of the robot
	 **/
	float dPhi;

 public:
	/**
	 * maximum angle of the steering wheel
	 **/
	float maxDphi;

 public:
	/**
	 * minimum angle of the steering wheel
	 **/
	float minDphi;

 protected:
	/**
	 * Length of the step of the robot
	 **/
	float dStep;

 public:
	/**
	 * last dStep
	 **/
	float dStep_old;

 protected:
	/**
	 * Draw the robot into the buffer
	 **/
	void draw();

 protected:
	/**
	 * Buffer saves the x-coordinates of the robot
	 **/
	float* xBuffer;

 protected:
	/**
	 * Buffer saves the y-coordinates of the robot
	 **/
	float* yBuffer;

 protected:
	/**
	 * Number of points in the buffer
	 **/
	int nBuffer;
  
 public:
	/**
	 * Move the robot
	 **/
	void move(int step);

 protected:
	/**
	 * Rotate the x-coordinate
	 **/
	float rotx(float f, float x,float y);

 protected:
	/**
	 * Rotate the y-coordinate
	 **/
	float roty(float f, float x,float y);

 protected:
	/**
	 * Width of the robot
	 **/
	float width;

 protected:
	/**
	 * lenghth of the robot
	 **/
	float length;

 public:
	/**
	 * documents the coord
	 **/
	void docCoord(long int step);

 public:
	/**
	 * collision right front
	 **/
	int collFrontLeft;

 public:
	/**
	 * collision left front
	 **/
	int collFrontRight;

 public:
	/**
	 * collision right rear
	 **/
	int collRearRight;

 public:
	/**
	 * collision left rear
	 **/
	int collRearLeft;

 public:
	/**
	 * sees an object in the left dir
	 **/
	int seeLeft;

 public:
	/**
	 * sees an onject in the right dir
	 **/
	int seeRight;

 private:
	bandp* bandp_da_dphi;
	bandp* bandp_da_ds;

	/**
	 * LP front left
	 **/
	bandp* bandpFront;

	/**
	 * LP front right
	 **/
	bandp* bandpRear;

	/**
	 * LP rear left
	 **/
	bandp* bandpLeft;

	/**
	 * LP rear right
	 **/
	bandp* bandpRight;


	/**
	 * BP left eye bandpasses
	 **/
	bandp** bandpLeftEye;

	/**
	 * BP right eye bandpasses
	 **/
	bandp** bandpRightEye;

	/**
	 * BP for the output dPhi
	 **/
	bandp** bandpDphi;

	/**
	 * BP for the output dStep;
	 **/
	bandp** bandpDstep;

	/**
	 * weight left eye to dstep
	 **/
	float* left2dStep;

	/**
	 * weight right eye to dstep
	 **/
	float* right2dStep;

	/**
	 * weight left eye to dPhi
	 **/
	float* left2dPhi;

	/**
	 * weight right eye to dPhi
	 **/
	float* right2dPhi;

	/**
	 * weight dStep to dPhi
	 **/
	float* dStep2dPhi;

	/**
	 * weight dPhi to dStep
	 **/
	float* dPhi2dStep;

	/*****************************************************************
	 * Attraction behaviour
	 *****************************************************************/

	Limbic_system* limbic_system;
       	Direction* Greendirection;
       	Direction* Bluedirection;

	/*****************************
	 *****************************/

 private:
	World* world;


 protected:
	/**
	 * reacts in the environment
	 **/
	void react(int step,int collision);

 private:
	/**
	 * hysteresis 
	 **/
	void hyst(int &counter,int detect);

 protected:
	/**
	 * docu the direction
	 **/
	FILE* fdir;

	/**
	 * docu the weights
	 **/
	FILE* fWeights;

	/**
	 * file for the coordinates file
	 **/
	FILE* fCoord;

	/**
	 * sensorEvent
	 **/
	int sensorOut;  

	/**
	 * file for the input- and output data
	 **/
	FILE* fIO;

	/**
	 * time to record after a vision-event
	 **/
	int ioEventDuration;

	/**
	 * counter: writes the io-data out if its above 0
	 **/
	int ioDataCount;

	/**
	 * structure which keeps all io-data
	 **/
	struct ioData_t ioData;

 private:
	int foodFl;
	int foodFr;
	int placefieldPl;
	int placefieldPr;

 public:
	int getFoodContact() {return foodFl||foodFr;};

 public:
        float getX0foodAbs() {return 1;};

 public:
        float getX0foodValue() {return 1;};

 public:
	int getPlacefieldContact() {return placefieldPl||placefieldPr;};

 public:
	int isPlacefield(int index);

 private:
	// time before learning starts. Basically to settle the resonators
	int time2settle;

 public:
	// sets the time to settle in units of frequency
	void setTime2settle(float f);

 public:
	// changes position
	void setRoboCoord(int x,int y) {xCoord=x;yCoord=y;};
	
 private:
	//the left and right direction switches.
	float Greensw;
	float Bluesw;


 private:
	int rewardDelayGreen = REWARD_DELAY;
	int rewardDelayBlue = REWARD_DELAY;

public:
	int eatenFlag = 0;
	int rewardFlag = 0;

	int nReward = 0;
	int nEaten = 0;

};




#endif
