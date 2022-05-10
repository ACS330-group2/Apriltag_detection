/**
**  ACS330-Group 2 Database Node
**/
#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <actionlib/server/simple_action_server.h>
#include <actionlib/client/simple_action_client.h>
#include <low_level_controller/ll_client_serverAction.h>
#include <process_actions/processAction.h>
#include <assembly_actions/assemblyAction.h>
#include <mm_actions/mmAction.h>

#include <std_msgs/Int32.h>
#include <unistd.h>

using namespace std;

class Result
{
protected:

	ros::NodeHandle n;
	//actionlib::SimpleActionServer<process_actions::processAction> as_; 
	//std::string station_name_;
	// create messages that are used to published feedback/result
	//process_actions::processResult result_;
	ros::Subscriber processdetected;
	ros::Subscriber assemblydetected;
	ros::Subscriber mm1detected;
	ros::Subscriber mm2detected;
	ros::Subscriber inputdetected;
	ros::Subscriber outputdetected;
	ros::Subscriber storagedetected;
	std::vector<float> process_timeCompleted;
	std::vector<uint32_t> process_seq;
	std::vector<float> assembly_timeCompleted;
	std::vector<uint32_t> assembly_seq;
	std::vector<float> mm1_timeCompleted;
	std::vector<uint32_t> mm1_seq;
	std::vector<float> mm2_timeCompleted;
	std::vector<uint32_t> mm2_seq;
	std::vector<float> input_timeCompleted;
	std::vector<uint32_t> input_seq;
	std::vector<float> output_timeCompleted;
	std::vector<uint32_t> output_seq;

	std::string storage;

	

public:
Result(std::string station_id) {
processdetected = n.subscribe("process/result", 1000, &Result::processCallback,this);
assemblydetected = n.subscribe("assembly/result", 1000, &Result::assemblyCallback,this);
mm1detected = n.subscribe("mm1/result", 1000, &Result::mm1Callback,this);
mm2detected = n.subscribe("mm2/result", 1000, &Result::mm2Callback,this);
inputdetected = n.subscribe("input/result", 1000, &Result::inputCallback,this);
outputdetected = n.subscribe("output/result", 1000, &Result::outputCallback,this);
//storagedetected = n.subscribe("storage", 1000, &Result::storageCallback,this);

}

void writeToCSV(std::string process_seq,std::string process_timeCompleted);
void writeToCSV(std::string assembly_seq,std::string assembly_timeCompleted);
void writeToCSV(std::string mm1_seq,std::string mm1_timeCompleted);
void writeToCSV(std::string mm2_seq,std::string mm2_timeCompleted);
void writeToCSV(std::string input_seq,std::string input_timeCompleted);
void writeToCSV(std::string output_seq,std::string output_timeCompleted){
std::ofstream csvfile;

csvfile.open("example.csv");

csvfile<<"Process Task\n"<<","<<"Process Task Time Completed\n";
csvfile<<process_seq<<','<<process_timeCompleted<<",\n";
csvfile<<"Assembly Task\n"<<","<<"Assembly Task Time Completed\n";
csvfile<<assembly_seq<<','<<assembly_timeCompleted<<",\n";
csvfile<<"Mobile Manupilator 1 Task\n"<<","<<"Mobile Manupilator 1 Task Time Completed\n";
csvfile<<mm1_seq<<','<<mm1_timeCompleted<<",\n";
csvfile<<"Mobile Manupilator 2 Task\n"<<","<<"Mobile Manupilator 2 Task Time Completed\n";
csvfile<<mm2_seq<<','<<mm2_timeCompleted<<",\n";
csvfile<<"Input Station Task\n"<<","<<"Input Station Task Time Completed\n";
csvfile<<input_seq<<','<<input_timeCompleted<<",\n";
csvfile<<"Output Station Task\n"<<","<<"Output Station Task Time Completed\n";
csvfile<<output_seq<<','<<output_timeCompleted<<",\n";

/*fs.open(outputFile,filename);
	outputFile << "" << "," << "Column B" << "Column C" << "Column D" << "Column E" << "Column F" << "Column G" << std::endl;
	'A'= process_seq;
	'B'= process_timeCompleted;
	'C'= assembly_seq;
	'D'= assembly_timeCompleted;
	'E'= mm1_seq;
	'F'= mm1_timeCompleted;	
	'G'= mm2_seq;
	'H'= mm2_timeCompleted;
	'I'= input_seq;
	'J'= input_timeCompleted;
	'K'= output_seq;
	'L'= output_timeCompleted;
	'M'= storage_seq;		
	//'N'= process_timeCompleted;	
	outputFile << 'A' << "," << 'B' << "," << 'C' <<"," << 'D' <<"," << 'E' <<"," << 'F' <<"," << 'G' <<"," << 'H' <<"," << 'I' <<"," << 'J' <<"," << 'K' <<"," << 'L' <<"," << 'M' <<"," << 'N' << std::endl;*/
	// close the output file
	//outputFile.close();
	ros::spin();

}
void processCallback(const process_actions::processActionResult::ConstPtr &processdetected){
	if(processdetected->result.complete)
	{
	process_seq.push_back(processdetected->header.seq);
	process_timeCompleted.push_back(processdetected->header.stamp.sec);
	}
	}

void assemblyCallback(const assembly_actions::assemblyActionResult::ConstPtr &assemblydetected){
	if(assemblydetected->result.complete)
	{
	assembly_seq.push_back(assemblydetected->header.seq);
	assembly_timeCompleted.push_back(assemblydetected->header.stamp.sec);	
	}
	}

void mm1Callback(const mm_actions::mmActionResult::ConstPtr& mm1detected)
	
	{
	if(mm1detected->result.complete)
	{
	mm1_seq.push_back(mm1detected->header.seq);
	mm1_timeCompleted.push_back(mm1detected->header.stamp.sec);	
	}
	}

void mm2Callback(const mm_actions::mmActionResult::ConstPtr& mm2detected)
	
	{
	if(mm2detected->result.complete)
	{
	mm2_seq.push_back(mm2detected->header.seq);
	mm2_timeCompleted.push_back(mm2detected->header.stamp.sec);	
	}
	}

void inputCallback(const low_level_controller::ll_client_serverActionResult::ConstPtr& inputdetected)

	{
	if(inputdetected->result.complete)
	{
	input_seq.push_back(inputdetected->header.seq);
	input_timeCompleted.push_back(inputdetected->header.stamp.sec);
	}
	}

void outputCallback(const low_level_controller::ll_client_serverActionResult::ConstPtr& outputdetected)
	
	{
	if(outputdetected->result.complete)
	{
	output_seq.push_back(outputdetected->header.seq);
	output_timeCompleted.push_back(outputdetected->header.stamp.sec);
	}
	};

/*void storageCallback(const std_msgs::String::ConstPtr& storagedetected)
	
	{
	storage=storagedetected->data;
	}
	};*/




//example of .txt output
//ofstream myfile("example.txt", ios::out | ios::binary);
//myfile << "Writing this to a file.\n";
//myfile.close();



int main(int argc, char** argv)
{
  // This must be called before anything else ROS-related
  ros::init(argc, argv, "database_node");
  
  Result result("result");
		

  // Don't exit the program.
  ros::spin();
  return 0;
}


