#ifndef SOZIOBOTS_H
#define SOZIOBOTS_H

#include <qwidget.h>
#include <QMainWindow>
#include "world.h"
#include "robot.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * Limbic system simulator
 **/

/**
 * GNU GENERAL PUBLIC LICENSE
 * Version 3, 29 June 2007
 * (C) 2017-2018, Bernd Porr, bernd.porr@glasgow.ac.uk
 **/

class LimbicMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	LimbicMainWindow( QWidget *parent = 0, 
			  const char *name = 0
		);

public:
	~LimbicMainWindow();


protected:
	void	paintEvent( QPaintEvent * );
    
protected:
	void	timerEvent( QTimerEvent * );

public:
	long int actualStep;

private:
	void doSimStep();
    
private:
	int argc;
	char** argv;

	// max x coordinate
	int maxx;

	// max y coordinate
	int maxy;

	int foodDelay;

	int isReversal;
    
	int numOfFoodContactsDuringReversal;

public:
	int numOfFoodContactsFromReversal;

public:
	// Creates the world
	World* world;
    
	// array of robots
	Robot** robot;

	// the X0 file
	FILE* fX0;

	int stepsBelow;

	void setX0filename(char* tmp);

private:
	void closeEvent(QCloseEvent *);

	int saveAsQuicktime = 0;

public:
	void writeQuicktime();
};


#endif
