#include "merge2quicktime.h"


Merge2quicktime::Merge2quicktime() {
	silence=NULL;
	verbose=0;
	sampleCredit=0;
}



Merge2quicktime::~Merge2quicktime() {
}



int Merge2quicktime::openQuicktime(const char* movFilename,
		const int w,
		const int h,
		const int rate,
		const char *codec) {
	file = quicktime_open(movFilename, 0, 1);
	frame_rate=rate;
	width=w;
	height=h;
	char tmp[256];
	strcpy(tmp,codec);
	quicktime_set_video(file,1,w,h,rate,tmp);
	quicktime_set_cmodel(file,BC_RGB888);
	quicktime_set_depth(file,24,0);

	int supported=quicktime_supported_video(file,0);
	if (!supported) {
		fprintf(stderr,"The video-codec is not supported\n");
		return 1;
	}

	// init the 2D-array from a 1D array since the data *must* be
	// in one row.
	videoBuffer=new unsigned char[(height+1)*(width+1)*3];
	row_pointers=new unsigned char*[height+1];
	for(int j=0;j<height;j++) {
		row_pointers[j]=&videoBuffer[j*width*3];
	}
	return 0;
}


void Merge2quicktime::setPoint(int x,int y,char r,char g,char b) {
	row_pointers[y][x*3+0]=r; // R
	row_pointers[y][x*3+1]=g; // G
	row_pointers[y][x*3+2]=b; // B
}



void Merge2quicktime::addPoint(int x,int y,
		unsigned char r,
		unsigned char g,
		unsigned char b) {
	int c;
	c=(unsigned char)(row_pointers[y][x*3+0])+r; // R
	if (c>255) {
		c=255;
	}
	row_pointers[y][x*3+0]=(char)c;
	c=(unsigned char)(row_pointers[y][x*3+1])+g; // G
	if (c>255) {
		c=255;
	}
	row_pointers[y][x*3+1]=(char)c;
	c=(unsigned char)(row_pointers[y][x*3+2])+b; // B
	if (c>255) {
		c=255;
	}
	row_pointers[y][x*3+2]=(char)c;
}



void Merge2quicktime::fill(char r,char g,char b) {
	for(int y=0;y<height;y++) {
		for(int x=0;x<width;x++) {
			row_pointers[y][x*3+0]=r;
			row_pointers[y][x*3+1]=g; // G
			row_pointers[y][x*3+2]=b; // B
		}
	}
}



void Merge2quicktime::drawFcircle(int x,int y,
		int r2,
		char r, char g, char b) {
	for(float phi=0;phi<=(M_PI*2);phi=phi+0.02) {
		int xa=(int)(x+r2*cos(phi));
		int ya=(int)(y+r2*sin(phi));
		if ((xa>=0)&&(xa<width)&&
				(ya>=0)&&(ya<height)) {
			addPoint(xa,ya,r,g,b);
		}
	}
}



int Merge2quicktime::step() {
	// write the video
	if (!file) return 0;
	quicktime_encode_video(file,row_pointers,0);
	fill(0,0,0);
	return 0;
}





int Merge2quicktime::close() {
	if (verbose) {
		fprintf(stderr,"All input files processed. Closing QuickTime file.\n");
	}
	if (!file) return 0;
	quicktime_close(file);
	file = NULL;
	return 0;
}







