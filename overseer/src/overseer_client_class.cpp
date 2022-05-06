#include <ros/ros.h>
#include <iostream>
#include <string>
#include <actionlib/client/simple_action_client.h>
#include <low_level_controller/ll_client_serverAction.h>
#include <low_level_controller/ll_client_server_outputAction.h>
#include <low_level_controller/ll_client_server_inputAction.h>
#include <actionlib/client/terminal_state.h>
#include <boost/thread.hpp>
#include <unistd.h>
#include <col_to_tasks.h>
#include <assembly_tasks.h>
#include <std_msgs/Char.h>

void spinThread()
{
	ros::AsyncSpinner spinner(4); // Use 4 threads
	spinner.start();
	ros::waitForShutdown();
	

}


class Overseer{
private:
	
	ros::NodeHandle n;
	bool stop;
	char colour;
	bool finished_before_timeout_I;
	bool finished_before_timeout_O;
	char storageStr[16];
	char combination[3];
	bool full;
	std::string location;
	int dot_pos;
	int block_pos;
	unsigned int microsecond;
	ros::Subscriber detected;
	

public:
	Overseer(std::string overseer_id)
	{
	actionlib::SimpleActionClient<low_level_controller::ll_client_serverAction> acI("input", true);
	actionlib::SimpleActionClient<low_level_controller::ll_client_serverAction> acO("output", true);
	ROS_INFO("%s: Activated", overseer_id.c_str());
	detected = n.subscribe("color_detected", 1000, &Overseer::detectionCallback,this);
		

	char storageStr[16]={'.','.','.','r','.','.','.','.','b','.','.','.','.','.','g','.'};
	std::string location;
	int dot_pos;
	int block_pos;
	
while(ros::ok()){

	boost::thread spin_thread(&spinThread);
	low_level_controller::ll_client_serverGoal goal;

	bool finished_before_timeout_I;
	bool finished_before_timeout_O;
	char colour;
	bool full = true;
	std::string XY;
	char combination[3] = {'r','b','g'};
	



ROS_INFO("Sending goals.");

	unsigned int microsecond = 1000000;
	usleep(3*microsecond); // sleeps for 3 seconds
	actionlib::SimpleClientGoalState state_I = acI.getState();
	actionlib::SimpleClientGoalState state_O = acO.getState();

	//goal combined
	while( colour == NULL&&ros::ok())
{
	ROS_INFO("no colour detetcted");
	ros::spinOnce();
	usleep(1*microsecond); // sleeps for 3 seconds
}

	if(state_I.toString() != "Active"){
	ROS_INFO("Input sending goals");
	for(int i = 0; i <=15;i++){
	if(storageStr[i] == '.')
	{
	dot_pos = i;
	full = false;
	i = 15;
	}
	else
	ROS_INFO("space not found checking position %d", i+1);
	}
	if(full ==true)
	{
		std::string location = {"Full"};
		ROS_INFO("Storgae full");
		break;
	}
	storageStr[dot_pos]=colour;
	int temp = dot_pos*5;
	XY=std::to_string(temp) += ".14";
	location = {XY};

	std::vector<std::string> tasksI = col_to_tasks(colour,location);

	goal.task = tasksI; //{"Heating","Cleaning","Cutting","MM1","MM2","Assembly"};
	acI.sendGoal(goal);
	finished_before_timeout_I = acI.waitForResult(ros::Duration(0.0));
		if (finished_before_timeout_I)
		{
		state_I = acI.getState();
		ROS_INFO("Action finished: %s",state_I.toString().c_str());
		}
		else
		ROS_INFO("Actions did not finish before the time out.");

	
	}
	//unsigned int microsecond = 1000000;
	usleep(3*microsecond); // sleeps for 3 seconds
	if(state_O.toString() != "Active")
{
	ROS_INFO("Output sending goals");
	for(int n = 0;n<=2;n++){
		for(int i = 0; i <=15;i++){
		if(storageStr[i] == combination[n])
		{
		block_pos = i;
		full = false;
		i = 15;
		}
		else
		ROS_INFO("checking position %d", i+1);
		}
		if(full ==true)
		{
			
			ROS_INFO("Block_NOT_FOUND");
			n = 3;
			break;
		}
		storageStr[block_pos]='.';
		int temp = block_pos*5;
		XY=std::to_string(temp) += ".19";
		

		std::vector<std::string> tasksO = assembly_tasks(XY);
		goal.task = tasksO; //{"Heating","Cleaning","Cutting","MM1","MM2","Assembly"};
		acO.sendGoal(goal);
		finished_before_timeout_O = acO.waitForResult(ros::Duration(0.0));
		if (finished_before_timeout_O)
			{
			
			state_O = acO.getState();
			ROS_INFO("Action finished: %s",state_O.toString().c_str());
			}
			else
			ROS_INFO("Actions did not finish before the time out.");
		}
		
	}


		
		
	}
	}
void detectionCallback(const std_msgs::Char::ConstPtr& detected)
{
	if(detected->data =='r'||detected->data =='b'||detected->data =='g')
	{
	colour = detected->data;
	}
	else{
	colour = NULL;
	}
}

};

int main (int argc, char **argv)
{
	ros::init(argc, argv, "Overseer");
	Overseer overseer("overseer");
	ros::AsyncSpinner spinner(4); // Use 2 threads
	spinner.start();

	
	
	//ros::waitForShutdown();
	
	return 0;
}


