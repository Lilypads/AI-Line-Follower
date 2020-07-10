/*
    Enki - a fast 2D robot simulator
    Copyright (C) 2017 Bernd Porr <mail@berndporr.me.uk>
    Copyright (C) 1999-2016 Stephane Magnenat <stephane at magnenat dot net>
    Copyright (C) 2004-2005 Markus Waibel <markus dot waibel at epfl dot ch>
    Copyright (c) 2004-2005 Antoine Beyeler <abeyeler at ab-ware dot com>
    Copyright (C) 2005-2006 Laboratory of Intelligent Systems, EPFL, Lausanne
    Copyright (C) 2006-2008 Laboratory of Robotics Systems, EPFL, Lausanne
    See AUTHORS for details

    This program is free software; the authors of any publication 
    arising from research using this software are asked to add the 
    following reference:
    Enki - a fast 2D robot simulator
    http://home.gna.org/enki
    Stephane Magnenat <stephane at magnenat dot net>,
    Markus Waibel <markus dot waibel at epfl dot ch>
    Laboratory of Intelligent Systems, EPFL, Lausanne.

    You can redistribute this program and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
Custom Robot example which has ground sensors and follows a line
It has also a camera which looks to the front and IR sensors
*/

//#include <../../enki/Enki.h>
#include "ros/ros.h" //these have to go first for some reason...
#include "sensor_msgs/Image.h"
#include "geometry_msgs/Twist.h"

#include <Enki.h>
#include <QApplication>
#include <QtGui>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Racer.h"

#include "bandpass.h"
#include "parameters.h"
#include <chrono>

#define reflex

using namespace Enki;
using namespace std;
using namespace std::chrono;

double	maxx = 250;
double	maxy = 250;

int countSteps=0;
int countRuns=0;
int learningRateCount =0;
int firstStep =1; //so that it does propInputs once and then back/forth in that order

int nInputs= ROW1N+ROW2N+ROW3N; //this cannot be an odd number for icoLearner


class EnkiPlayground : public EnkiWidget
{
private:

    //callback function for neural network output to set motor speeds
    void motors_callback(const geometry_msgs::Twist::ConstPtr& msg){
        //random mapping I have assigned
        //need to find a way of doing this properly
        //racer->leftSpeed = msg->angular.z;
        //racer->rightSpeed = msg->angular.y;

        double angular_speed = msg->angular.z;

        //both will be 30 speed //used to multiply by 18
        racer->leftSpeed = (angular_speed*50.0) + 30.0;
        racer->rightSpeed = (angular_speed*(-50.0)) + 30.0;
        /**********************
         * 
         * MIGHT NEED TO CHANGE MULTIPLICATION HERE DEPENDING ON NEURAL NET OUTPUT VALUE....
         * 
         * **********************/
    }

protected:
	Racer* racer;
    double speed = SPEED;
    double prevX;
    double prevY;
    double prevA;

    FILE* errorlog = NULL;
    FILE* fcoord = NULL;
    FILE* fspeed = NULL;


    //ros stuff
    ros::NodeHandle nh;
    //publisher to output the 8 line sensor values
    ros::Publisher line_sensor_pub[8];
    //publisher to output the 9x9 'front camera'
    ros::Publisher camera_pub;
    //subscriber for the motor control messages
    ros::Subscriber sub;

public:
    EnkiPlayground(World *world, QWidget *parent = 0):
		EnkiWidget(world, parent)
	{

        //N.B. all these use same topic names as gazebo
        line_sensor_pub[0] = nh.advertise<sensor_msgs::Image>("mybot/left_sensor4/image_raw",1);
        line_sensor_pub[1] = nh.advertise<sensor_msgs::Image>("mybot/left_sensor3/image_raw",1);
        line_sensor_pub[2] = nh.advertise<sensor_msgs::Image>("mybot/left_sensor2/image_raw",1);
        line_sensor_pub[3] = nh.advertise<sensor_msgs::Image>("mybot/left_sensor1/image_raw",1);
        line_sensor_pub[4] = nh.advertise<sensor_msgs::Image>("mybot/right_sensor1/image_raw",1);
        line_sensor_pub[5] = nh.advertise<sensor_msgs::Image>("mybot/right_sensor2/image_raw",1);
        line_sensor_pub[6] = nh.advertise<sensor_msgs::Image>("mybot/right_sensor3/image_raw",1);
        line_sensor_pub[7] = nh.advertise<sensor_msgs::Image>("mybot/right_sensor4/image_raw",1);

        camera_pub = nh.advertise<sensor_msgs::Image>("mybot/camera1/image_raw", 1);

        sub = nh.subscribe("mybot/cmd_vel", 1, &EnkiPlayground::motors_callback, this);

#ifdef reflex
        errorlog = fopen("errorReflex.tsv","wt");
        fcoord = fopen("coordReflex.tsv","wt");
#endif
        racer = new Racer(nInputs);
        racer->pos = Point(maxx/2 +30, maxy/2 +5); // x and y of the start point
		racer->leftSpeed = 0;
		racer->rightSpeed = 0;
        world->addObject(racer);

        //set camera sensor pixels -> 9x9 array of ground sensors infront of robot
        //args, y distance from centre of robot, number of sensor in row, spacing between sensors
        racer->setPreds(26, 9, 2);
        racer->setPreds(24, 9, 2);
        racer->setPreds(22, 9, 2);
        racer->setPreds(20, 9, 2);
        racer->setPreds(18, 9, 2);
        racer->setPreds(16, 9, 2);
        racer->setPreds(14, 9, 2);
        racer->setPreds(12, 9, 2);
        racer->setPreds(10, 9, 2); //10 is front of robot, since robot is 20long and 10wide

    }

    ~EnkiPlayground(){
        fclose(fcoord);
        fclose(errorlog);
        for(int i=0; i<8; i++){
            line_sensor_pub[i].shutdown();
        }
        camera_pub.shutdown();
    }
	// here we do all the behavioural computations
	// as an example: line following and obstacle avoidance


//this is called every 30Hz
virtual void sceneCompletedHook()
	{
        /*
        int errorGain = ERRORGAIN;

		double leftGround = racer->groundSensorLeft.getValue();
		double rightGround = racer->groundSensorRight.getValue();
        double error = (leftGround - rightGround);
        */

        //setup the ros message
        sensor_msgs::Image msg;
        msg.header.seq = countRuns;
        msg.header.frame_id = "camera_link";
        msg.height = 1;
        msg.width = 1;
        msg.encoding = "mono8";
        msg.is_bigendian = 0;
        msg.step = 1;
        msg.data.push_back(uint8_t(racer->lineSensors[0]->getValue()*255.0));

        line_sensor_pub[0].publish(msg);

        for(int i=1; i<8; i++){

            msg.data.pop_back();
            msg.data.push_back(uint8_t(racer->lineSensors[i]->getValue()*255.0));
            line_sensor_pub[i].publish(msg);
        }

        //publish the camera stuff
        msg.height = 9;
        msg.width = 9;
        msg.step = 9; //i think the step for one row?

        msg.data.pop_back();
        for(int i=0; i<9; i++){
            //do it row by row since rows are flipped for some weird reason
            for(int j=0; j<9; j++){
                msg.data.push_back(uint8_t(racer->groundSensorArray[(((i+1)*9)-1)-j]->getValue()*255.0));
            }
        }

        camera_pub.publish(msg);

        ros::spinOnce();

#ifdef reflex
        if (countRuns==0){
            fprintf(fcoord,"%e\t%e\n",racer->pos.x,racer->pos.y);
        }
#endif
        //fprintf(errorlog, "%e\t", error);
        //error = error * errorGain;

#ifdef reflex
        //racer->leftSpeed  = speed + error;
        //racer->rightSpeed = speed - error;
#endif

        countSteps ++;
        if (countSteps == STEPSCOUNT){
            qApp->quit();
        }

	}
};

int main(int argc, char *argv[])
{
    srand(5);
    QApplication app(argc, argv);
    QImage gt;
    gt = QGLWidget::convertToGLFormat(QImage("src/line_follower/cc.png")); //path from catkin_ws
    if (gt.isNull()) {
        fprintf(stderr,"Texture file not found\n");
        exit(1);
    }
    const uint32_t *bits = (const uint32_t*)gt.constBits();
    World world(maxx, maxy,
                Color(1000, 1000, 1000), World::GroundTexture(gt.width(), gt.height(), bits));
    cout<<gt.width()<<" "<<gt.height()<<endl;

    ros::init(argc,argv,"enki_node");

    EnkiPlayground viewer(&world);
    viewer.show();

    return app.exec();
}

