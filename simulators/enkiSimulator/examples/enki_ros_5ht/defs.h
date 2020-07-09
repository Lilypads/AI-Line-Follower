#ifndef _defs_h
#define _defs_h



// constant parameters

/**
 * Experiment
 **/
#define EXPERIMENT 100

/**
 * Number of input channels for the learner
 **/
#define MAXFEATURES 3

/**
 * Number of iterations of the simulation before its displayed
 **/
#define ITERATIONS_BEFORE_DISPLAY 21

/**
 * Move only some robots below the given step number
 **/
#define PARTIAL_MOVE_BELOW_STEP 1

/**
 * Max Number of Robots
 **/
#define MAXROBOT 1

/**
 * Number of robots which should be moved
 **/
#define ROBOTS_WHICH_MOVE 1

/**
 * X-playground
 **/
#define MAXX 600

/**
 * Y-playground
 **/
#define MAXY 400

/**
 * Starting angle
 **/
#define START_ANGLE 2.5

/**
 * ROBOT_SPEED of the robot
 **/
#define ROBOT_SPEED 5

#define DAMPING 50

// threshold to switch on a behaviour
#define MOTOR_THRES 0.1

// slight random behaviour of the robot
#define RANDWALK

// interval bewtween two steps
#define TIMER_INTERVAL 20

/**
 * Frame-rate for the quicktime-film
 **/
#define FRAME_RATE 25

#define QT_CODEC QUICKTIME_PNG

#define DOC_EVERY_N_FRAMES  1000000000
#define DOC_FOR_N_FRAMES    1000000000

/**
 * stops if an x0 file is there
 **/
// #define CHECK_FOR_X0_FILE 1



//////////////////////////////////////////////////////////////////////
// BUMP

/**
 * revese ROBOT_SPEED to let the robot turn
 **/
#define BUMP_REVERSE_GAIN 0.15

/**
 * gain of the steering wheel when the robot turns
 **/
#define BUMP_STEER_GAIN -0.005


#define THREE_FACTOR_RULE 1

/**
 * Learning rate for the bump: dphi
 **/
#define MU_PHI_BUMP 0.00000


/**
 * Learning rate for the bump: ds
 **/
#define MU_DS_BUMP  0.00000

  /**
   * Frequency for the standard reaction of the robot
   **/
#define BUMP_F_REFLEX 0.01

/**
 * Quality for the standard recation fo the robot
 **/
#define BUMP_Q_REFLEX 0.6

/**
 * Number of filters for the learned behaviour
 **/
#define BUMP_N_FILTERS 10

/**
 * Highest frequency for the learned reaction of the robot
 **/
#define BUMP_F_LEARN 0.1

/**
 * Factor for the development of the frequencies
 **/
#define BUMP_LEARN_FACT 1

/**
 * Q for the learned reaction of the robot
 **/
#define BUMP_Q_LEARN 1.0

/**
 * RF_DEPTH of the light sensors
 **/
#define RF_DEPTH 50
//#define RF_DEPTH 100

/**
 * time
 **/
#define STICK_LIMIT 100


//////////////////////////////////////////////////////////////////////
// FOOD

// the rest of the parameters are defined in "main" of soziobots

#define FOOD_DIAMETER 20

// what the robot can see
#define EYES_VIEWING_ANGLE M_PI/2

/**
 * Number of FOOD bits
 **/
#define MAXFOOD 2

/**
 * No new food after step no
 **/
#define NO_NEW_FOOD_AFTER 900000

/**
 * Draw the food
 **/
#define DRAW_FOOD 

/**
 * The gain for the food
 **/
#define FOOD_GAIN 0.002

/**
 * Lifetime of the food
 **/
#define MAX_FOOD_LIFE 100

/**
 * number of the filters
 **/
#define N_FILTERS_FOOD 5

// number of steps before a new food blob is placed
#define FOOD_DELAY 50

///////////////////////////////////////////////////////////////////////
// PLACE FIELDS

// number of place fields

#define MAXPLACEFIELD 2

#define PLACEFIELD_DIAMETER 90

#define PLACE_X1 450
#define PLACE_X2 450
#define PLACE_Y1 100
#define PLACE_Y2 300

///////////////////////////////////////////////////////////////////////
// Statistics
// Max simulation steps
#define MAXSTEP 300000

///////////////////////////////////////////////////////////////////////
// Reversal learning

// defines how many successful contacts we need before reversal learning
#define THRES_FOOD_CONTACTS 100

#endif

