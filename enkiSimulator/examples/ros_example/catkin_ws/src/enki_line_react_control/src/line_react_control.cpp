#include <unistd.h>

#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "geometry_msgs/Twist.h"

static int sensor_values[2]; 
static int count; 
static bool flag = false;


void callback(const sensor_msgs::Image::ConstPtr& msg, int index){
	sensor_values[index] = msg->data[0];
	//TODO have a better way to check for recpetion of all sensor values...
	count++;
	if(count==2){
		count=0;
		flag = true;
	}
}
void callback0(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 0);}
void callback1(const sensor_msgs::Image::ConstPtr& msg){callback(msg, 1);}

void calculateMotorSpeeds(geometry_msgs::Twist& msg){
	
	double error = (sensor_values[0]/255.0) - (sensor_values[1]/255.0);
	//error *= 50; //150

	//msg.angular.z = 30 + error;
	//msg.angular.y = 30 - error;	
	
	msg.angular.z = error;
	
	

}
/*===============================================================================================*/

int main(int argc, char **argv){

	ros::init(argc, argv, "line_react_control_node");
	ros::NodeHandle nh;
	ros::Subscriber sub[2];
	
	sub[0] = nh.subscribe("mybot/left_sensor4/image_raw", 1, callback0);
	sub[1] = nh.subscribe("mybot/right_sensor4/image_raw", 1, callback1);

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
