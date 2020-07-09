#ifndef _MERGE2QUICKTIME
#define _MERGE2QUICKTIME

#include<quicktime.h>
#include<colormodels.h>
#include<string.h>
#include<stdio.h>
#include<math.h>


class Merge2quicktime {
 public:
/**
 * Initialisation: Constructor
 **/
    Merge2quicktime();


/**
 * Destructor
 **/
    ~Merge2quicktime();


/**
 * Opens the output-file
 **/
    int openQuicktime(const char* movFilename,
		      const int w,
		      const int h,
		      const int rate=10,
		      const char *codec=QUICKTIME_RAW);


/**
 * Sets a point in the image
 **/
    void setPoint(int x,int y,char r,char g,char b);


/**
 * Fills the image in the color rgb.
 **/
    void fill(char r,char g,char b);


/**
 * Draws a ring from radius r1 to radius r2
 **/
void drawFcircle(int xm,int ym,
		 int r2,
		 char r, char g, char b);
 


/**
 * Writes the video data out in the quicktime file.
 **/    
    int step();

/**
 * Closes the quicktime-file
 **/
    int close();

 private:
/**
 * Quicktime file
 **/
    quicktime_t *file;

/**
 * The pointer to an array which contains silence
 **/
    char* silence;

/**
 * Width
 **/
    int width;

/**
 * Height
 **/
    int height;


/**
 * The sound-samples per video.frame
 **/
    int samplesPerFrame;

/**
 * Is it verbose
 **/
    int verbose;

/**
 * Frame rate
 **/
    float frame_rate;

/**
 * Filename of the quicktime file
 **/
    char* movFilename;

/**
 * Gives the number of samples which are left of a sound effect
 **/
    int sampleCredit;

/**
 * Video-Buffer which is trasferred to the quicktime lib
 **/
    unsigned char* videoBuffer;

/**
 * 2D array for convenience: points to videoBuffer so that the
 * rows can directly be addressed.
 **/
    unsigned char** row_pointers;

void addPoint(int x,int y,
	      unsigned char r,
	      unsigned char g,
	      unsigned char b);

 private:
 int pcmbitwidth(unsigned int format);
 

};


#endif









