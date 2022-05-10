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
#include <std_msgs/String.h>

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
	ros::Subscriber order;
	

public:
	Overseer(std::string overseer_id)
	{
	actionlib::SimpleActionClient<low_level_controller::ll_client_serverAction> acI("input", true);
	actionlib::SimpleActionClient<low_level_controller::ll_client_serverAction> acO("output", true);
	ROS_INFO("%s: Activated", overseer_id.c_str());
	detected = n.subscribe("Color_Detected", 1000, &Overseer::detectionCallback,this);
	detected = n.subscribe("order", 1000, &Overseer::orderCallback,this);
		

	char storageStr[16]={'.','.','.','r','.','.','.','.','y','.','.','.','.','.','g','.'};
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
	char combination[3] = {'o','o','o'};
	colour = 'o';
	





	unsigned int microsecond = 1000000;
	usleep(3*microsecond); // sleeps for 3 seconds
	actionlib::SimpleClientGoalState state_I = acI.getState();
	actionlib::SimpleClientGoalState state_O = acO.getState();

	//goal combined
	if( colour == 'o'&&ros::ok()&&combination[1] == 'o')
{
	ROS_INFO("no input detetcted");
	ros::spinOnce();
	usleep(1*microsecond); // sleeps for 3 seconds
}

	if(state_I.toString() != "Active"&&colour != 'o'){
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
	if(state_O.toString() != "Active"&&combination[1] != 'o')
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


		
	ros::spinOnce();	
	}
	}
void detectionCallback(const std_msgs::String::ConstPtr& detected)
{
	if(detected->data[0] =='r'||detected->data[0] =='y'||detected->data[0] =='g'||detected->data[0] =='o')
	{
	colour = detected->data[0];
	ROS_INFO("colour recieved");
	}
	
}

void orderCallback(const std_msgs::String::ConstPtr& order)
{
	if(order->data[0] != 'o'){
	combination[0] = order->data[0];
	combination[1] = order->data[1];
	combination[2] = order->data[2];
	ROS_INFO("order recieved");
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


