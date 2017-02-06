#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Float64.h> 


void scalerCallback(const std_msgs::Float64& weight) 
{ 
  ROS_INFO("received value is: %f",weight.data);
  weight_data = weight.data;
  //really could do something interesting here with the received data...but all we do is print it 
} 


int main(int argc, char **argv) {
	//Set up our node.
	ros::init(argc, argv, "playfile_jointspace");
	ros::NodeHandle nh;


trajectory_msgs::JointTrajectory default_position;
default_position.positions.resize(14);

default_position.positions[0] = -0.66709;
default_position.positions[1] = -0.0507692;
default_position.positions[2] = 0.201701;
default_position.positions[3] = 1.25122;
default_position.positions[4] = -0.0885405;
default_position.positions[5] = 0.0329113;
default_position.positions[6] = 0.15338;
default_position.positions[7] = 0.734138;
default_position.positions[8] = -0.253545;
default_position.positions[9] = 0.200111;
default_position.positions[10] = 0.540371;
default_position.positions[11] = 0.0721441;
default_position.positions[12] = -0.0711621;
default_position.positions[13] = -0.163507;


ros::Subscriber scaler_sub= nh.subscribe("scaler", 1, scalerCallback);

