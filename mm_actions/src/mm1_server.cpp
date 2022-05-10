
// version 2 oop

#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <mm_actions/mmAction.h>
#include <unistd.h>
#include <std_msgs/Int32.h>



class MMAction
{
protected:

	ros::NodeHandle nh_;
	actionlib::SimpleActionServer<mm_actions::mmAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
	std::string mm_name_;
	// create messages that are used to published feedback/result
	mm_actions::mmFeedback feedback_;
	mm_actions::mmResult result_;
	ros::Subscriber feedback;
	bool completed;
	unsigned int microsecond;
public:
MMAction(std::string mm_id) :
as_(nh_, mm_id, boost::bind(&MMAction::executeCB, this, _1), false), mm_name_(mm_id)
	{
		as_.start();
		ROS_INFO("%s: Activated", mm_id.c_str());
		feedback = nh_.subscribe("feedback", 1000, &MMAction::feedbackCallback,this);
		bool completed = false;
		
	}

~MMAction(void)
{
}
void executeCB(const mm_actions::mmGoalConstPtr &goal)
{
	ros::Rate r(1);
bool success = true;
feedback_.percent_complete = 0;

ROS_INFO("%s: Executing, Moving to %d,%d", mm_name_.c_str(), goal->location[0],goal->location[1]);
 // start executing the action
ros::Publisher location_pub = nh_.advertise<std_msgs::Int32>("location", 1000);


std_msgs::Int32 locationX;
	unsigned int microsecond = 1000000;
       while(completed != true && ros::ok())
	{
		
	locationX.data = goal->location[0];
	location_pub.publish(locationX);
	
	usleep(1*microsecond); // sleeps for seconds
	
	}
	ros::spinOnce();
	locationX.data = 0;
for(int i=0;i>=4;i++){
	location_pub.publish(locationX);
	usleep(0.5*microsecond);
}
	
	result_.complete = true;
	ROS_INFO("%s task completed", mm_name_.c_str());
	as_.setSucceeded(result_);	
	

   	
}
void feedbackCallback(const std_msgs::Int32::ConstPtr& feedback)
{
	if(feedback->data ==1)
	{
	completed = true;
	ROS_INFO("Completed = true");
	}
	else 
	completed= false;
	ROS_INFO("Completed = false");
}


};
 int main(int argc, char** argv)
 {
     ros::init(argc, argv, "mm1");
	

	MMAction mm1("mm1");
	
	ros::spin();
   
     return 0;
   }
