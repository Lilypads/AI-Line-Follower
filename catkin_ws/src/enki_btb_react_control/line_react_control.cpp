#include <unistd.h>

#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "geometry_msgs/Twist.h"

static int sensor_values[9];
static int sensor_values_stuck;
static int count; 
static bool flag = false;


void callback(const sensor_msgs::Image::ConstPtr& msg, int index){
	sensor_values[index] = msg->data[(81*1)+index*9+5];
		//81*1 shift to red data (from rgba)
		//index*9 to get desired row
		//+5 to get to the middle of the row
	count++;
	if(count==9){
		count=0;
		flag = true;
	}
}
void callback0(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 0);}
void callback1(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 1);}
void callback2(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 2);}
void callback3(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 3);}
void callback4(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 4);}
void callback5(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 5);}
void callback6(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 6);}
void callback7(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 7);}
void callback8(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 8);}

void callbackstuck(const sensor_msgs::Image::ConstPtr& msg){
	sensor_values_stuck = msg->data[18];
		//18 is one of the first data points that change from gray to white on rqt when near walls, so we use it to check if the robot stuck at the walls
	count++;
	if(count==1){
		count=0;
		flag = true;
	}
}


void calculateMotorSpeeds(geometry_msgs::Twist& msg){
	
	if((sensor_values[8]/255.0) - (sensor_values[2]/255.0) != 0||(sensor_values[7]/255.0) -(sensor_values[3]/255.0) != 0||(sensor_values[6]/255.0) - (sensor_values[4]/255.0) != 0){
		double error = (sensor_values[6]/255.0) + 2*(sensor_values[7]/255.0) + 3*(sensor_values[8]/255.0) - 3*(sensor_values[2]/255.0) - 2*(sensor_values[3]/255.0) - (sensor_values[4]/255.0);
		msg.angular.z = -error*0.01;
	}

	else if(sensor_values_stuck/255.0 > 0.9) { // if it is almost white (0.0-1.0 is black-white)
		msg.angular.z = 0.8;	//positive is clockwise
	}

}
/*===============================================================================================*/

int main(int argc, char **argv){

	ros::init(argc, argv, "enki_btb_react_control_node");
	ros::NodeHandle nh;
	ros::Subscriber middleRowR[9];
	ros::Subscriber stuck;
	
	middleRowR[0] = nh.subscribe("mybot/camera2/image_raw", 1, callback0);
	middleRowR[1] = nh.subscribe("mybot/camera2/image_raw", 1, callback1);
	middleRowR[2] = nh.subscribe("mybot/camera2/image_raw", 1, callback2);
	middleRowR[3] = nh.subscribe("mybot/camera2/image_raw", 1, callback3);
	middleRowR[4] = nh.subscribe("mybot/camera2/image_raw", 1, callback4);
	middleRowR[5] = nh.subscribe("mybot/camera2/image_raw", 1, callback5);
	middleRowR[6] = nh.subscribe("mybot/camera2/image_raw", 1, callback6);
	middleRowR[7] = nh.subscribe("mybot/camera2/image_raw", 1, callback7);
	middleRowR[8] = nh.subscribe("mybot/camera2/image_raw", 1, callback8);
	stuck = nh.subscribe("mybot/camera2/image_raw", 1, callbackstuck);

	ros::Publisher motors_pub = nh.advertise<geometry_msgs::Twist>("mybot/cmd_vel", 1);

	ros::Rate loop_rate(60);

	while(ros::ok()){
	
		if(flag){
			flag = false;
			geometry_msgs::Twist motors_msg;
			calculateMotorSpeeds(motors_msg);
			motors_pub.publish(motors_msg);
		}
	
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;

}
