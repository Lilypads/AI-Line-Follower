#include <QTimer>
#include <QTimerEvent>
#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <QLayout> 
#include <QPixmap>

#include <sys/stat.h>
#include <sys/types.h>

#include "world.h"
#include "robot.h"
#include "worldpoint.h"
#include "defs.h"
#include "patience-5ht.h"


/***********************************************
 * GNU GENERAL PUBLIC LICENSE
 * Version 3
 * (C) 2017-2019, Bernd Porr, bernd.porr@glasgow.ac.uk
 ***********************************************/

/**
 * shows the sim in a window
 **/
int SHOW_SIM = 1;

/**
 * Number of max landmark contacts containing the food
 **/
int MAXFOODCONTACTS = 50;

/**
 * Path to the results
 **/
const char prefix[]="results_patience_for_reward";

#define BORDERS 1



/** Sets the parameters of the simulation
 * These are all globally defiend in the
 * respecitve c++ codes and then made external
 * so that they can be set here from the main
 * program.
 **/
const char* setParameters(int scenario) {
	
	// default parameters
	LHB_BIAS = 0;
	DRN_SUPPRESSION = 0;
	OFC_5HTR1_OFFSET = 0;
	OFC_5HTR2_OFFSET = 0;
	DRN_OFFSET = 0;
	REWARD_DELAY = 150;

	switch (scenario) {
	case 0:
		fprintf(stderr,"Normal condition");
		return "normal";
	case 1:
		fprintf(stderr,"Reward shows up earlier");
		REWARD_DELAY = 100;
		return "normal_less_wait";
	case 2:
		fprintf(stderr,"DRN is suppressed");
		DRN_SUPPRESSION = 4;
		return "drn_suppress";
	case 3:
		fprintf(stderr,"DRN is suppressed and less wait");
		DRN_SUPPRESSION = 4;
		REWARD_DELAY = 100;
		return "drn_suppress_less_wait";
	case 4:
		fprintf(stderr,"DRN is suppressed and SSRI");
		DRN_SUPPRESSION = 4;
		DRN_OFFSET = 0.15;
		return "drn_suppress_ssri";
	case 5:
		fprintf(stderr,"DRN is suppressed, SSRI and less wait");
		DRN_SUPPRESSION = 4;
		DRN_OFFSET = 0.15;
		REWARD_DELAY = 100;
		return "drn_suppress_ssri_less_wait";
	case 6:
		fprintf(stderr,"DRN is suppressed and 5HTR2 up");
		DRN_SUPPRESSION = 4;
		OFC_5HTR2_OFFSET = 0.5;
		return "drn_suppress_5ht2up";
	case 7:
		fprintf(stderr,"DRN is suppressed, 5HTR2 up and less wait");
		DRN_SUPPRESSION = 4;
		OFC_5HTR2_OFFSET = 0.5;
		REWARD_DELAY = 100;
		return "drn_suppress_5ht2up_less_wait";
	}
	return "";
}








LimbicMainWindow::LimbicMainWindow( QWidget *parent, 
				    const char *) : QMainWindow( parent ) {
	actualStep=0;
	maxx=MAXX;
	maxy=MAXY;
	stepsBelow=0;
	fX0=NULL;
	foodDelay=FOOD_DELAY;
	numOfFoodContactsDuringReversal=0;
	numOfFoodContactsFromReversal=0;
	isReversal=0;

	// Creates the world
	world=new World(maxx,maxy);
	world->setContactsFilename("contacts.dat");

	// array of robots
	robot=NULL;

	robot=new Robot*[MAXROBOT];
	for(int i=0; i<MAXROBOT;i++) {
		robot[i]=new Robot(world,i,0,maxx/2,maxy/2-i*30,10,20);
		robot[i]->setPhi(START_ANGLE);
	}

	//Borders, food!

#ifdef BORDERS
	fprintf(stderr,"Borders\n");
	world->drawBorders();
#endif
	fprintf(stderr,"Food\n");

#ifdef DRAW_FOOD

	world->drawFood(0,PLACE_X1-FOOD_DIAMETER/2,
			PLACE_Y1-FOOD_DIAMETER/2,
			FOOD_DIAMETER,0,1); // real food at index 0
	world->drawFood(0,
			PLACE_X2+FOOD_DIAMETER/2,
			PLACE_Y2+FOOD_DIAMETER/2,
			FOOD_DIAMETER,1,0); // fake food at index 1
#endif	
	// place field

	fprintf(stderr,"Place field\n");
	
	int index = 0;
	
	world->drawPlacefield(0,PLACE_X1,PLACE_Y1,PLACEFIELD_DIAMETER,index);
	index++;
	world->drawPlacefield(0,PLACE_X2,PLACE_Y2,PLACEFIELD_DIAMETER,index);
	index++;

   
	startTimer( TIMER_INTERVAL );
}


LimbicMainWindow::~LimbicMainWindow() {
	if (saveAsQuicktime) {
		saveAsQuicktime = 0;
	        world->closeQuicktime();
        }
	for(int i=0;i<MAXROBOT;i++) {
		delete robot[i];
	}
	delete robot;

}


void LimbicMainWindow::writeQuicktime() {
	saveAsQuicktime = 1;
	world->openQuicktime("/tmp/limbic.mov");
}



//
// Handles paint events for the LimbicMainWindow widget.
//

void LimbicMainWindow::paintEvent( QPaintEvent * ) {
	// will paint in this object
	int w = width();
	int h = height();

	QPixmap pm( w, h );

	QPainter *paint = new QPainter( &pm );

	if(w <= 0 || h <= 0) 
		return;

	world->fillPainter(*paint);

	delete paint;

	QPainter paint2(this);
	paint2.drawPixmap(0,0,pm);
}

//
// Handles timer events for the LimbicMainWindow widget.
//

void LimbicMainWindow::timerEvent( QTimerEvent * )
{
	for ( int i=0; i<ITERATIONS_BEFORE_DISPLAY; i++ ) {
		doSimStep();
	}
	repaint();
}


void LimbicMainWindow::doSimStep() {

	world->setStep(actualStep);

	if (actualStep>PARTIAL_MOVE_BELOW_STEP) { // 
		for(int j=0;j<ROBOTS_WHICH_MOVE;j++) {
			robot[j]->move(actualStep);
			robot[j]->docCoord(actualStep);
		}
	} else { // all robots one step
		for(int j=0;j<MAXROBOT;j++) {
			robot[j]->move(actualStep);
			robot[j]->docCoord(actualStep);
		}
	}
	//plot new food...

#ifdef DRAW_FOOD
	
	if (world->getNewFood()){
		if (foodDelay>0) {
			foodDelay--;
		} else {
		        robot[0]->rewardFlag = 0;
		        robot[0]->eatenFlag = 0;
			
			// fprintf(stderr,"step #%d: drawing new food\n",actualStep);
			int newIndex = world->getFreeFoodIndex();

			robot[0]->xCoord = 50;
			robot[0]->yCoord = world->maxy/2 + rand()%(maxy/3)-maxy/6;
			
			int cx1 = PLACE_X1;
			int cy1 = PLACE_Y1;
			
			int cx2 = PLACE_X2;
			int cy2 = PLACE_Y2;
			int dx,dy;
			int sub = PLACEFIELD_DIAMETER/6;
			
			// let's find out if there's still real food out there
			// this is the index number of the food bit which is actually rewarding
			int r=world->getRewardIndex();
			// get info if that has been eaten
			int reward_exists=world->getFoodValid(r);
			if (!(world->getSwapFlag())) {
				// real food is on the left
				if (isReversal) {
					fprintf(stderr,"Final reversal contacts: %d\n",
						numOfFoodContactsDuringReversal);
				}
			} else {
				// real food is on the right (reversal)
				if (!isReversal) {
					// first step
					isReversal=1;
					numOfFoodContactsFromReversal=
						world->getNumberOfFoodContacts();
					fprintf(stderr,"########### REVERSAL STARTED ###########\n");
				}
				numOfFoodContactsDuringReversal=
					world->getNumberOfFoodContacts()-
					numOfFoodContactsFromReversal;
				fprintf(stderr,"Reversal contacts so far: %d\n",
					numOfFoodContactsDuringReversal);
			}


			    
			if (reward_exists) {
				// reward is still out there
				// we have to place fake food
				if (world->getSwapFlag()) {
					dx=rand()%(PLACEFIELD_DIAMETER/3);
					//fprintf(stderr,"%d\n",dx);
					dy=rand()%(PLACEFIELD_DIAMETER/3);
					world->drawFood(actualStep, 
							cx1+dx-sub, 
							cy1+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							0);
				} else {
					// we plae the fake food on the right
					dx=rand()%(PLACEFIELD_DIAMETER/3);
					//fprintf(stderr,"%d\n",dx);
					dy=rand()%(PLACEFIELD_DIAMETER/3);
					world->drawFood(actualStep, 
							cx2+dx-sub, 
							cy2+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							0);
				}
			} else {
				if (!(world->getSwapFlag())) {
					dx=rand()%(PLACEFIELD_DIAMETER/3);
					dy=rand()%(PLACEFIELD_DIAMETER/3);
					//fprintf(stderr,"%d\n",dx);
					world->drawFood(actualStep, 
							cx1+dx-sub, 
							cy1+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							1);
				} else {
					dx=rand()%(PLACEFIELD_DIAMETER/3);
					dy=rand()%(PLACEFIELD_DIAMETER/3);
					//fprintf(stderr,"%d\n",dx);
					world->drawFood(actualStep, 
							cx2+dx-sub, 
							cy2+dy-sub, 
							FOOD_DIAMETER,
							newIndex,
							1); 
				}
			}
		}
	} else {
		foodDelay=FOOD_DELAY+(rand()%FOOD_DELAY);
	}
#endif	

	if (saveAsQuicktime) {
		world->docQuicktime(actualStep);
	}
	
        actualStep++;
	if (actualStep==MAXSTEP) {
		close();
	}
	if (robot[0]->nEaten >= MAXFOODCONTACTS) {
		close();
	}
}


void LimbicMainWindow::closeEvent(QCloseEvent *) {
        if (saveAsQuicktime) {
		saveAsQuicktime = 0;
	        world->closeQuicktime();
        }
}





void single_food_run(int argc, char **argv,int quicktime = 0) {
	QApplication a( argc, argv );
	LimbicMainWindow* limbicbots=new LimbicMainWindow();	// create widget
	if (quicktime) limbicbots->writeQuicktime();
	limbicbots->resize( MAXX, MAXY );	// start up with size 400x250
	if (SHOW_SIM)
		limbicbots->show();		// show widget
	a.exec();				// run event loop
	printf("nRewards = %d, nEaten = %d\n",limbicbots->robot[0]->nReward,limbicbots->robot[0]->nEaten);
}






void statistics_food_run(int argc, char **argv) {
	fprintf(stderr,"----------STATS------------\n");
	FILE* f=fopen("perf.dat","wt");
	QApplication* a=NULL;
	LimbicMainWindow* limbicbots=NULL;
	a=new QApplication( argc, argv ); // create application object

	// loop through different learning rates.
       	for(float phi=0.0001;phi<2*M_PI;phi=phi+0.1) {
		fprintf(stderr,"phi=%e\n",phi);
		limbicbots=new LimbicMainWindow();
		if (!limbicbots) {
			fprintf(stderr,"Cound not create limbicbots class\n");
			exit(1);
		}
		limbicbots->robot[0]->setPhi(phi);
		limbicbots->resize( MAXX, MAXY );
		if(SHOW_SIM)
			limbicbots->show();
		a->exec();
		printf("nRewards = %d, nEaten = %d\n",limbicbots->robot[0]->nReward,limbicbots->robot[0]->nEaten);
		fprintf(f,"%ld\t%d\t%d\n",
			limbicbots->actualStep,
			limbicbots->robot[0]->nReward,
			limbicbots->robot[0]->nEaten);
		fflush(f);
		srandom((unsigned int)(limbicbots->actualStep%65536));
		delete limbicbots;
	}
	delete a;
	fclose(f);
}


void createSubdirs(const char* selectedPath) {
	mkdir(prefix,0770);
	chdir(prefix);
	mkdir(selectedPath,0770);
	chdir(selectedPath);
}



int main( int argc, char **argv ) {
        int c;
	int qt = 0;
	int stats = 0;
	
	while (-1 != (c = getopt(argc, argv, "p:f:abqhn"))) {
		switch (c) {
		case 'a':
			stats = 0;
			break;
                case 'b':
			stats = 1;
			break;
                case 'q':
			qt = 1;
			break;
		case 'n':
			SHOW_SIM = 0;
			break;
		case 'f':
			MAXFOODCONTACTS = atoi(optarg);
			break;
		case 'p':
			createSubdirs(setParameters(atoi(optarg)));
			break;
		case 'h':
			fprintf(stderr,"%s: command line options\n",argv[0]);
			fprintf(stderr," -a:            single food run\n");
			fprintf(stderr," -b:            statistics\n");
			fprintf(stderr," -f max:        max contacts to the landmark with food for stats\n");
			fprintf(stderr," -q:            quicktime export\n");
			fprintf(stderr," -n:            no graphical output\n");
			fprintf(stderr," -p scenario #: experimental scenario\n");
			fprintf(stderr,"Possible scenarios:\n");
			for(int i=0;i<8;i++) {
				fprintf(stderr,"%d : ",i);
				const char* s = setParameters(i);
				fprintf(stderr," (/%s/)\n",s);
			}
			exit(0);
                }
        }
	// default behaviour without any valid arguments
	if (!stats) {
		single_food_run(argc,argv,qt);
	} else {
		statistics_food_run(argc,argv);
	}
	return 0;	
}
