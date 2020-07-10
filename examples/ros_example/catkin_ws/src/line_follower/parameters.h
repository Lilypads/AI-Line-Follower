#ifndef PARAMETERS_H
#define PARAMETERS_H

#endif // PARAMETERS_H

#define STEPSCOUNT 10000
#define RUNSCOUNT 10

#define LEARNINGRATECOUNT 5

// Predictor Array
#define ROW1P 12
#define ROW1N 24
#define ROW1S 2

#define ROW2P 14
#define ROW2N 18
#define ROW2S 2

#define ROW3P 16
#define ROW3N 12
#define ROW3S 2

// Error Arrays
#define SENSORLEFT 8
#define SENSORRIGHT -8

// Filter Specifications
#define NFILTERS 5
#define MINT 3 //3
#define MAXT 10 //10
#define DAMPINGCOEFF 0.51

// Network Structure
#define BCOEFF 0.0
#define MCOEFF 0.0
#define FCOEFF 0.0
#define GCOEFF 0.0
#define LCOEFF 0.0
#define ECOEFF 0.0
#define StartLEARNINGRATE 0.1
#define DESIREDLEARNINGRATE 0.01

#define NLAYERS 11
    #define N1 2
    #define N2 11
    #define N3 11
    #define N4 11
    #define N5 11
    #define N6 11
    #define N7 11
    #define N8 11
    #define N9 11
    #define N10 2
    #define N11 6

// Robot speed
#define SPEED 30 // for reflex it is 60
#define ERRORGAIN 150 // for reflex it is 200

#define NETWORKGAIN 10
#define PREDGAIN 100


