#include "worldpoint.h"
#include "stdio.h"


// initialise one coordinate point in the playground
WorldPoint::WorldPoint() {
   // it's no obstacle
  obstacle=0;
  // it's not food
  food=new int[MAXFOOD];
  for(int i=0;i<MAXFOOD;i++) {
    food[i]=0;
  }
  // it's not a place field
  placefield=new int[MAXPLACEFIELD];
  for(int i=0;i<MAXPLACEFIELD;i++) {
    food[i]=0;
  }
  robot=new int[MAXROBOT];
  // and it's no robot
  for(int i=0;i<MAXROBOT;i++) {
    robot[i]=0;
  }
}


WorldPoint::~WorldPoint() {
    delete food;
    delete robot;
    delete placefield;
}


// Tests if there is an obstacle or a robot
int WorldPoint::isRobot() {
  // loop through all robot layers
  for(int i=0;i<MAXROBOT;i++) {
    if (robot[i]) {
      return i+1;
    }
  }
  return 0;
}



// Tests if there is an obstacle or a robot
int WorldPoint::isObstacle() {
  // first the obstacle
  if (obstacle) {
    return 1;
  }
  // a robot is also an obstacle
  return isRobot();
}




// Tests if there is food
int WorldPoint::indexFood() {
  // loop through all food layers
  for(int i=0;i<MAXFOOD;i++) {
    if (food[i]) {
      return i;
    }
  }
  return -1;
}

// Tests if there is placefield
int WorldPoint::isPlacefield(int index) {
  if (placefield[index]) {
      return 1;
      }
  return 0;
}
	
void WorldPoint::setObstacle() {
  obstacle=1;
}


void WorldPoint::resetObstacle() {
  obstacle=0;
}


void WorldPoint::setFood(int i) {
  food[i]=1;
}


void WorldPoint::resetFood(int i) {
  food[i]=0;
}


void WorldPoint::setRobot(int i) {
  robot[i]=1;
}


void WorldPoint::resetRobot(int i) {
  robot[i]=0;
}

void WorldPoint::setPlacefield(int i) {
  placefield[i]=1;
}

void WorldPoint::resetPlacefield(int i) {
  placefield [i]=0;
}


