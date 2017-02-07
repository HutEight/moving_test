#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/Float64.h>
#include <cwru_davinci_traj_streamer/trajAction.h>
#include <actionlib/client/simple_action_client.h>


// void scalerCallback(const std_msgs::Float64& weight)
// {
//   ROS_INFO("received value is: %f",weight.data);
//   weight_data = weight.data;
//   //really could do something interesting here with the received data...but all we do is print it
// }


int main(int argc, char **argv) {
	//Set up our node.
	ros::init(argc, argv, "moving_test");
	ros::NodeHandle nh;

	std::string fname;


	trajectory_msgs::JointTrajectory track;

	trajectory_msgs::JointTrajectoryPoint default_position;
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
	default_position.time_from_start = ros::Duration(0.0);

	track.points.push_back(default_position);

	trajectory_msgs::JointTrajectoryPoint temp;
	temp.positions.resize(14);

	double wait_time = 2.0;

	for (int i=0; i<20; i++){
	  temp.positions[0] = default_position.positions[0];
	  temp.positions[1] = default_position.positions[1];
	  temp.positions[2] = default_position.positions[2] + (i+1)*0.2;
	  temp.positions[3] = default_position.positions[3];
	  temp.positions[4] = default_position.positions[4];
	  temp.positions[5] = default_position.positions[5];
	  temp.positions[6] = default_position.positions[6];
	  temp.positions[7] = default_position.positions[7];
	  temp.positions[8] = default_position.positions[8];
	  temp.positions[9] = default_position.positions[9];
	  temp.positions[10] = default_position.positions[10];
	  temp.positions[11] = default_position.positions[11];
	  temp.positions[12] = default_position.positions[12];
	  temp.positions[13] = default_position.positions[13];


	  temp.time_from_start = ros::Duration( 2.0+ i*2.0);

	  track.points.push_back(temp);
		wait_time += 2.0;
	}

	// ros::Subscriber scaler_sub= nh.subscribe("scaler", 1, scalerCallback);
	track.header.stamp = ros::Time::now();

	//Add an ID.
	cwru_davinci_traj_streamer::trajGoal tgoal;
	tgoal.trajectory = track;
	srand(time(NULL));
	tgoal.traj_id = rand();

	//Locate and lock the action server
	actionlib::SimpleActionClient<
	  cwru_davinci_traj_streamer::trajAction
	> action_client("davinci_gazebo", true);
	bool server_exists = action_client.waitForServer(ros::Duration(5.0));
	// something odd in above: does not seem to wait for 5 seconds, but returns rapidly if server not running
	ROS_INFO("Waiting for server: ");
	while (!server_exists && ros::ok()) {
	  server_exists = action_client.waitForServer(ros::Duration(5.0));
	  ROS_WARN("Could not connect to server; retrying...");
	}
	ROS_INFO("SERVER LINK LATCHED");

	//Send our message:
	ROS_INFO("Sending trajectory with ID %u", tgoal.traj_id);
	action_client.sendGoal(tgoal);

	//action_client.getState();

	//Wait for it to finish.
	while(!action_client.waitForResult(ros::Duration(wait_time + 2.0)) && ros::ok()){
	  ROS_WARN("CLIENT TIMED OUT- LET'S TRY AGAIN...");
	  //Could add logic here to resend the request or take other actions if we conclude that
	  //the original is NOT going to get served.
	}
	//Report back what happened.
	ROS_INFO(
	  "Server state is %s, goal state for trajectory %u is %i",
	  action_client.getState().toString().c_str(),
	  action_client.getResult()->traj_id,
	  action_client.getResult()->return_val
	);

	//This has to do with the intermittent "Total Recall" bug that breaks the trajectory interpolator
	//If you see this appear in your execution, you are running the program too soon after starting Gazebo.
	if(action_client.getState() ==  actionlib::SimpleClientGoalState::RECALLED){
	  ROS_WARN("Server glitch. You may panic now.");
	}

	return 0;
}
