
#include "world.h"
#include "stdio.h"
#include "worldpoint.h"
#include <qpen.h>
#include <stdio.h>




World::World(int x,int y) {
	swapFlag=0;
	foodCounter = 0;
	nConsequFoodContacts=0;
	nContacts=0;
	fContacts=NULL;
    int g = 128;
    int b = 0;
    penPlacefield=new QPen*[MAXPLACEFIELD];
	for(int index=0;index<MAXPLACEFIELD;index++) {
        penPlacefield[index]=new QPen(QColor(0,g,b));
        int t = b;
        b = g;
        g = t;
    }
	penFood=new QPen*[MAXFOOD+1];
    g = 255;
    b = 0;
	for(int index=0;index<MAXFOOD;index++) {
        penFood[index]=new QPen(QColor(0,g,b));
        int t = b;
        b = g;
        g = t;
    }
	penFood[MAXFOOD] = new QPen(QColor(255,0,0));
	r_index=0;
	maxx=x;
	maxy=y;
	// array of x pointers to an array of pointers
	points=new WorldPoint**[maxx+1];
	for(int x=0;x<maxx;x++) {
		// points[x] points on the array of all y coordinates
		points[x]=new WorldPoint*[maxy+1];
		for(int y=0;y<maxy;y++) {
			points[x][y]=new WorldPoint();
		}
	}
	foodX=new int[MAXFOOD];
	foodY=new int[MAXFOOD];
	foodEaten=new int[MAXFOOD];

	placefieldX=new int[MAXPLACEFIELD];
	placefieldY=new int[MAXPLACEFIELD];
	placefieldEntered=new int[MAXPLACEFIELD];

	for(int i=0;i<MAXFOOD;i++) {	
	        foodX[i]=-1;
		foodY[i]=-1;
		foodEaten[i]=-1;
	}

	for(int i=0;i<MAXPLACEFIELD;i++) {
		placefieldX[i]=-1;
		placefieldY[i]=-1;
		placefieldEntered[i]=-1;
	}
	foodContacts=0;
	lastXfood=-100;
	lastYfood=-100;

	placefieldContacts=0;
	lastXplacefield=-100;
	lastYplacefield=-100;
}


void World::setContactsFilename(const char *fn) {
	fContacts=fopen(fn,"wt");
}


World::~World() {
	delete foodX;
	delete foodY;
	delete foodEaten;
	delete placefieldX;
	delete placefieldY;
	delete placefieldEntered;

	for(int x=0;x<maxx;x++) {
		for(int y=0;y<maxy;y++) {
			delete points[x][y];
		}
		delete points[x];
	}
	delete points;
	if (fContacts) {
		fclose(fContacts);
	}
}

void World::drawBorders() {
	// Borders
	for(int x=5;x<(maxx-5);x++) {
		getPoint(x,5)->setObstacle();
		getPoint(x,maxy-5)->setObstacle();
	}
	for(int y=5;y<(maxy-5);y++) {
		getPoint(5,y)->setObstacle();
		getPoint(maxx-5,y)->setObstacle();
	}
}


// gets the point with periodic boundary conditions
WorldPoint* World::getPoint(float x,float y) {
	int xtmp=(((long int)x)%maxx);
	if (xtmp<0) {
		xtmp=xtmp+maxx;
	}
	int ytmp=(((long int)y)%maxy);
	if (ytmp<0) {
		ytmp=ytmp+maxy;
	}
	return points[xtmp][ytmp];
}


int World::isObstacle(float x,float y) {
	return getPoint(x,y)->isObstacle();
}


int World::isReward(int,float x,float y) {
	int index=getPoint(x,y)->indexFood();
	if (index<0) {
		// no food
		return 0;
	}
	int d = (index==r_index);
	return d;
}

int World::isFood(int step,float x,float y) {
	int index=getPoint(x,y)->indexFood();

	if (index<0) {
		// no food
		return 0;
	} else {
		foodEaten[index]--;
		if (foodEaten[index]<0) {
			fprintf(stderr,"Food landmark encountered\n");
			if (isReward(step,x,y)) {
				nConsequFoodContacts++;
			} else {
				nConsequFoodContacts=0;	
			}
			nContacts++;
			if (fContacts) {
				if (nConsequFoodContacts>=THRES_FOOD_CONTACTS) {
					fprintf(fContacts,"%d %d\n",step,nContacts);
					fprintf(stderr,"step %d: %d contacts.\n",step,nContacts);
					nConsequFoodContacts=0;
					nContacts=0;
					fflush(fContacts);
					swapFlag=!swapFlag;
				}
			}
			foodCounter--;
			if (foodCounter<0) {
			  printf("BUG!\n");
			  exit(1);
			}
			removeFood(index);
			foodContacts++;
		}
	}
	return 1;
}

int World::isPlacefield(int index,float x,float y) {
	return getPoint(x,y)->isPlacefield(index);
}

int World::drawObstacle(int x, int y, int w, int h) {
	int nPoints=w*h*2;
	int* xBuff=new int[nPoints];
	int* yBuff=new int[nPoints];
	int n=0;
	int x1,x2,y1,y2;

	y1=y-h/2;
	y2=y+h/2;
	if ((y1<0)||(y1>=maxy)||
	    (y2<0)||(y2>=maxy)) {
		delete xBuff;
		delete yBuff;
		return 0;
	}
	for(x1=x-(w/2);x1<(x+w/2);x1++) {
		xBuff[n]=x1;
		yBuff[n]=y1;
		n++;


		xBuff[n]=x1;
		yBuff[n]=y2;
		n++;
	}
	x1=x-w/2;
	x2=x+w/2;
	if ((x1<0)||(x1>=maxx)||
	    (x2<0)||(x2>=maxx)) {
		delete xBuff;
		delete yBuff;
		return 0;
	}
	for(int y1=y-(h/2);y1<(y+h/2);y1++) {
		xBuff[n]=x1;
		yBuff[n]=y1;
		n++;
		xBuff[n]=x2;
		yBuff[n]=y1;
		n++;
	}
	for(int i=0;i<n;i++) {
		if (getPoint(xBuff[i],yBuff[i])->isObstacle()) {
			delete xBuff;
			delete yBuff;
			return 0;
		}
	}
	for(int i=0;i<n;i++) {
		getPoint(xBuff[i],yBuff[i])->setObstacle();
	}
	delete xBuff;
	delete yBuff;
	return 1;
}



void World::removeFood(int index) {
	//make it invalid
	foodX[index]=-1;
	foodY[index]=-1;
	foodEaten[index]=-1;
	// erase the old obstacle
	for(int xl=0;xl<maxx;xl++) {
		for(int yl=0;yl<maxy;yl++) {
			getPoint(xl,yl)->resetFood(index);
		}
	}
}



int World::drawFood(int step,int x, int y, int r, int index,int reward) {
	int nPoints=(int)(M_PI*r*r*2);
	int* xBuff=new int[nPoints];
	int* yBuff=new int[nPoints];
	int n=0;

	// fprintf(stderr,"drawing food\n");

	removeFood(index);

	if (step>NO_NEW_FOOD_AFTER) {
		// fake success
		delete xBuff;
		delete yBuff;
		// fprintf(stderr,"no new food\n");
		return 1;
	}

	// draw a circle
	// Create the coordinates in a buffer
	for(int ym=-r;ym<=r;ym=ym+1) {
		int x1=-(int)sqrt((float)r*(float)r-(float)ym*(float)ym);
		int x2=(int)sqrt((float)r*(float)r-(float)ym*(float)ym);
		for(int xm=x1;xm<=x2;xm++) {
			xBuff[n]=x+xm;
			yBuff[n]=y+ym;
			if ((xBuff[n]<0)||
			    (xBuff[n]>maxx)||
			    (yBuff[n]<0)||
			    (yBuff[n]>maxy)) {
				delete xBuff;
				delete yBuff;
				return 0;
			}
			n++;
		}
		if (n>=nPoints) {
			fprintf(stderr,"FATAL: n>nPoints in drawFood\n");
			exit(1);
		}
	}

	// Test, if the food collides with obstacles or the robot itself  
	for(int i=0;i<n;i++) {
		if (getPoint(xBuff[i],yBuff[i])->isObstacle()) {
			delete xBuff;
			delete yBuff;
			// fprintf(stderr,"Coll with obstacle\n");
			return 0;
		}
		if (getPoint(xBuff[i],yBuff[i])->isRobot()) {
			delete xBuff;
			delete yBuff;
			// fprintf(stderr,"Coll with robot\n");
			return 0;
		}
		// Test, if the food collides with another food-bit
		int iFood=getPoint(xBuff[i],yBuff[i])->indexFood();
		if (iFood>=0) {
			removeFood(iFood);
			foodCounter--;
		}
	}
	// draw the food in the playground
	for(int i=0;i<n;i++) {
		getPoint(xBuff[i],yBuff[i])->setFood(index);
	}
	// save the coordinates
	foodX[index]=x;
	foodY[index]=y;
	foodEaten[index]=MAX_FOOD_LIFE;
	// success
	delete[] xBuff;
	delete[] yBuff;
	foodCounter++;
	fprintf(stderr,"step#%d, Food drawn, # of food bits: %d\n",step,foodCounter);
	// This is our reward
	if (reward) {
		setRewardIndex(index);
	}
	return 1;
}


void World::removePlacefield(int index) {
	//make it invalid
	placefieldX[index]=-1;
	placefieldY[index]=-1;
	placefieldEntered[index]=-1;
	// erase the old obstacle
	for(int xl=0;xl<maxx;xl++) {
		for(int yl=0;yl<maxy;yl++) {
			getPoint(xl,yl)->resetPlacefield(index);
		}
	}

}



int World::drawPlacefield(int,int x, int y, int r, int index) {
	int nPoints=(int)(M_PI*r*r*2);
	int* xBuff=new int[nPoints];
	int* yBuff=new int[nPoints];
	int n=0;

	fprintf(stderr,"drawing place field\n");

	removePlacefield(index);

	// draw a circle
	// Create the coordinates in a buffer
	for(int ym=-r;ym<=r;ym=ym+1) {
		int x1=-(int)sqrt((float)r*(float)r-(float)ym*(float)ym);
		int x2=(int)sqrt((float)r*(float)r-(float)ym*(float)ym);
		for(int xm=x1;xm<=x2;xm++) {
			xBuff[n]=x+xm;
			yBuff[n]=y+ym;
			if ((xBuff[n]<0)||
			    (xBuff[n]>maxx)||
			    (yBuff[n]<0)||
			    (yBuff[n]>maxy)) {
				delete xBuff;
				delete yBuff;
				return 0;
			}
			n++;
		}
		if (n>=nPoints) {
			fprintf(stderr,"FATAL: n>nPoints in drawPlacefield\n");
			exit(1);
		}
	}

	// draw the place field in the playground
	for(int i=0;i<n;i++) {
		getPoint(xBuff[i],yBuff[i])->setPlacefield(index);
	}
	// save the coordinates
	placefieldX[index]=x;
	placefieldY[index]=y;
	// success
	delete[] xBuff;
	delete[] yBuff;
	fprintf(stderr,"place field drawn\n");
	return 1;
}


int World::getFoodValid(int i) {
	return ((foodEaten[i]>0)&&(foodX[i]>=0)&&(foodY[i]>=0)); 
}

int World::getFreeFoodIndex() {
	for(int i=0;i<MAXFOOD;i++) {
		if (!getFoodValid(i)) {
			return i;
		}
	}
	return -1;
}


int World::getFoodX(int i) {
	if (i>=MAXFOOD) {
		fprintf(stderr,"Fatal: getFoodX: index too large: %d\n",i);
		exit(1);
	}
	return foodX[i];
}


int World::getFoodY(int i) {
	if (i>=MAXFOOD) {
		fprintf(stderr,"Fatal: getFoodY: index too large\n");
		exit(1);
	}
	return foodY[i];
}

int World::getPlacefieldValid(int i) {
	return ((placefieldEntered[i]>0)&&(placefieldX[i]>=0)&&(placefieldY[i]>=0)); 
}


int World::getPlacefieldX(int i) {
	if (i>=MAXPLACEFIELD) {
		fprintf(stderr,"Fatal: getPlacefieldX: index too large: %d\n",i);
		exit(1);
	}
	return placefieldX[i];
}


int World::getPlacefieldY(int i) {
	if (i>=MAXPLACEFIELD) {
		fprintf(stderr,"Fatal: getPlacefieldY: index too large\n");
		exit(1);
	}
	return placefieldY[i];
}


void World::openQuicktime(const char* qtName) {
	merge2quicktime=new Merge2quicktime();
	int result=merge2quicktime->openQuicktime(qtName,
					      maxx,
					      maxy,
					      FRAME_RATE,
					      QT_CODEC);
	if (result) {
		fprintf(stderr,"Cannot open quicktime. Error code %d\n",result);
		merge2quicktime = NULL;
		exit(result);
	}
}


void World::docQuicktime(int step) {
	if (step%10) {
		return;
	}
	if (!merge2quicktime) return;

	QImage image = QPixmap(maxx, maxy).toImage();
	QPainter pixPaint(&image);
	fillPainter(pixPaint);
	
	for(int y=0;y<maxy;y++) {
		for(int x=0;x<maxx;x++) {
			QRgb pixel = image.pixel(x,y);
			merge2quicktime->setPoint(x,y,qRed(pixel),qGreen(pixel),qBlue(pixel));
		}
	}
	merge2quicktime->step();
}


void World::closeQuicktime() {
	merge2quicktime->close();
	delete merge2quicktime;
}


void World::fillPainter(QPainter &painter) {
	painter.eraseRect ( 0, 0, MAXX, MAXY );
	int r[]={0,255,000,000,255,000,255};
	int g[]={0,000,255,000,255,255,000};
	int b[]={0,000,000,255,000,255,255};
	QString t;
	QFont timeFont("helvetica",12);
	painter.setFont(timeFont);
	painter.setPen(QColor(0,0,0));
	painter.drawText(10,20,t.asprintf("%ld",step));
	QPen penObstacle(QColor(255,255,255));
	for(int y=0;y<maxy;y++) {
		for(int x=0;x<maxx;x++) {
			WorldPoint* p=getPoint(x,y);
			for(int index=0;index<MAXPLACEFIELD;index++) {
				if ((p->isPlacefield(index))) {
                    painter.setPen(*penPlacefield[index]);
                    painter.drawPoint(x,y);
				}
			}
			if (p->isObstacle()) {
				painter.setPen(penObstacle);
				painter.drawPoint(x,y);
			}
			int idx = p->indexFood();
			if (idx>=0) {
				if (idx == r_index) {
					switch (reward_visible) {
					case 1:
						if (y<(maxy/2)) {
							painter.setPen(*penFood[MAXFOOD]);
						} else {
							painter.setPen(*penFood[(p->indexFood())]);
						}
						break;
					case 2:
						if (y>(maxy/2)) {
							painter.setPen(*penFood[MAXFOOD]);
						} else {
							painter.setPen(*penFood[(p->indexFood())]);
						}
						break;
					default:
						painter.setPen(*penFood[(p->indexFood())]);
					}
				} else {
					painter.setPen(*penFood[(p->indexFood())]);
				}
				painter.drawPoint(x,y);
			}
			int ri=p->isRobot();
			if (ri) {
				ri--;
				QColor color(r[ri%7],g[ri%7],b[ri%7]);
				QPen pen(color);
				painter.setPen(pen);
				painter.drawPoint(x,y);
			}
		}
	}
}
