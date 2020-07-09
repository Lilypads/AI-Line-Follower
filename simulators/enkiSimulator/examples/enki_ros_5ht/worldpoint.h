#ifndef _point
#define _point
#include<stdio.h>
#include "defs.h"

class Point;
typedef Point* PPpoint;

/*******************************************************************
 * This is one point in my toy-world: it consists of different
 * layers. One layer for obstacles and MAXFOOD layers of food
 * and MAXROBOT layers of robots.
 *******************************************************************/
class WorldPoint {

 public:
  /**
   * Sets the empty point
   **/
  WorldPoint();

  ~WorldPoint();

 private:
  /**
   * If there is an obstacle then it gets a unique number
   **/
  int obstacle;

 private:
  /**
   * If there is food: the different food items are indexed since
   * they have to be removed separately
   **/
  int* food;
  
 private:
  /**
   * If there is robot 1 then it is set to true.
   **/
  int* robot;

 private:
  /**
   * If there is place field: diff place fields are indexed since
   * they have to be removed separately
   **/
  int* placefield;

 public:
  /**
   * Tests if there is an obstacle at that point (either another robot
   * or an rigid obstacle). Loops through all robots and the obstacle
   * layer.
   **/
  int isObstacle();

 public:
  /**
   * Tests if there is food in any layer
   **/
  int indexFood();


 public:
  /**
   * Tests if there is a robot
   **/
  int isRobot();

 public:
  /**
   * Tests if there is a place field with index number index
   **/
  int isPlacefield(int index);

 public:
  /**
   * Sets that there is a robot: i is the index of the layer
   **/
  void setRobot(int i);

 public:
  /**
   * Sets that there is food: i is the index of the layer
   **/
  void setFood(int i);

 public:
  /**
   * Sets that there is an obstacle
   **/
  void setObstacle();

 public:
  /**
   * Sets that there is place field: i is index of layer
   **/
  void setPlacefield(int i);

 public:
  /**
   * Sets that there is a robot: i is the index of the layer
   **/
  void resetRobot(int i);

 public:
  /**
   * Sets that there is food: i is the index of the layer
   **/
  void resetFood(int i);

 public:
  /**
   * Sets that there is an obstacle
   **/
  void resetObstacle();

 public:
  /**
   * Sets that there is place field: i is index of the layer
   **/
  void resetPlacefield(int i);
};

#endif
