#ifndef _world
#define _world



#include "merge2quicktime.h"



#include <stdio.h>
#include "defs.h"
#include "bandp.h"
#include "worldpoint.h"
#include <qpainter.h>

class world;
typedef world* Pworld;



/**
 * This is the grid of my toy-world
 **/
class World {
 public:
	/**
	 * Constructor: maximum size of the grid
	 **/
	World(int maxX,int maxY);

	/**
	 * Destructor
	 **/ 
 public:
	~World();

 public:
	void fillPainter(QPainter &painter);

 public:
	int maxx;

 public:
	int maxy;
	
 public:
	int foodCounter;

 
 private:
	/**
	 * The array of my toy world
	 **/
	WorldPoint*** points;

 public:
	/**
	 * draws the borders
	 **/
	void drawBorders();

 private:
	/**
	 * The array of the food: X
	 **/
	int* foodX;

 private:
	/**
	 * The array of the food: Y
	 **/
	int* foodY;

	/**
	 * Is the food already eaten? Positive value: food there, zero
	 * or negative its no longer there
	 **/
	int* foodEaten;

 public:
	/**
	 * asks if the layer is valid
	 **/
	int getFoodValid(int i);

 public:
	/** 
	 * gets the x-coordinate of the i-th food piece
	 **/
	int getFoodX(int i);

 public:
	/** 
	 * gets the y-coordinate of the i-th food piece
	 **/
	int getFoodY(int i);


 private:
	/**
	 * The array of the placefield: X
	 **/
	int* placefieldX;

 private:
	/**
	 * The array of the placefield: Y
	 **/
	int* placefieldY;


	/**
	 * Is the placefield already entered? Positive value: placefield there, zero
	 * or negative its no longer there
	 **/
	int* placefieldEntered;

 public:
	/**
	 * asks if the layer is valid
	 **/
	int getPlacefieldValid(int i);


 public:
	/** 
	 * gets the x-coordinate of the i-th place field
	 **/
	int getPlacefieldX(int i);

 public:
	/** 
	 * gets the y-coordinate of the i-th place field
	 **/
	int getPlacefieldY(int i);


 public:
	/**
	 * gets a pointer to one point
	 **/
	WorldPoint* getPoint(float x,float y);

 private:
	/**
	 * points to the object which writes the quicktime files
	 **/
	Merge2quicktime* merge2quicktime = NULL;

 public:
	/**
	 * documents the entire time course as quicktime-raw
	 **/
	void openQuicktime(const char* qtName);

 public:
	/**
	 * Writes one frame
	 **/
	void docQuicktime(int step);

 public:
	/**
	 * Closes qt
	 **/
	void closeQuicktime();

 public:
	/**
	 * Detects if there is an obstacle (or other robot)
	 **/
	int isObstacle(float x,float y);

 public:
	/**
	 * Detects if there is food
	 **/
	int isFood(int step,float x,float y);

 public:
	/**
	 * Detects reward signal
	 **/
	int isReward(int step,float x,float y);

 public:
	/**
	 * Detects if there is place field
	 **/
	int isPlacefield(int index,float x,float y);

 public:
	/**
	 * Draws the obstacle only if there is no robot or other obstacle
	 **/
	int drawObstacle(int x, int y, int w, int h);

 public:
	/**
	 * Draws food
	 **/
	int drawFood(int step,int x, int y, int r, int index,int reward=1);

 public:
	/**
	 * Draws place field
	 **/
	int drawPlacefield(int step,int x, int y, int r, int index);

 private:
	/**
	 * Number of robots in the playground
	 **/
	int nRobots = 0;

 public:
	/**
	 * Remove food
	 **/
	void removeFood(int foodIndex);

 public:
	/**
	 * Remove place field
	 **/
	void removePlacefield(int placefieldIndex);

 private:
	int foodContacts = 0;

 public:
	int getNumberOfFoodContacts() {return foodContacts;};

	int lastXfood = 0;
	int lastYfood = 0;

 private:
	int placefieldContacts = 0;

 public:
	int getNumberOfPlacefieldContacts() {return placefieldContacts;};

	int lastXplacefield = 0;
	int lastYplacefield = 0;

	//"reward" index no...
 private:
       	int r_index;

 public:
	int getRewardIndex() {
		return r_index;
	}

 private:
	void setRewardIndex(int r) {
		r_index=r;
	}

 private:
	int reward_visible = 1;

 public:
	void setRewardVisible(int _reward_visible) {reward_visible = _reward_visible;}

 public:
	int getNewFood(){
		return (foodCounter<MAXFOOD);
	}

 public:
	int getFreeFoodIndex();

 private:
	QPen** penPlacefield;
	QPen** penFood;

 private:
	// number of consequtive food contacts with real food
	int nConsequFoodContacts;

	// number of contacts with food in total (fake or real)
	int nContacts;

	long int step = 0;

 public:
	inline void setStep(long int _step) {step=_step;}

 public:
	int getConsequFoodContacts() {
		return nConsequFoodContacts;
	}

 public:
	int getContacts() {
		return nContacts;
	}

 private:
	FILE* fContacts = NULL;

 public:
	void setContactsFilename(const char *fn);

 private:
	int swapFlag;

 public:
	int getSwapFlag() {
		return swapFlag;
	}

};
#endif


