


std::vector<std::string>  col_to_tasks(char colour, std::string location )
{

	std::vector<std::string> tasks; // Empty on creation
    
	 
	
	if(colour == 'r')
			{
			//tasks.push_back("test0"); // Adds an element
			    
			    tasks.push_back("MM1");
			    tasks.push_back("0.14");
			    tasks.push_back("end");
			    tasks.push_back("wait");
			    tasks.push_back("MM1");
			    tasks.push_back("12.14");
			    tasks.push_back("end");
			    tasks.push_back("Heating");
			    tasks.push_back("MM1");
			    tasks.push_back(location);
			    tasks.push_back("end");
			}
	if(colour == 'b')
			{
			//tasks.push_back("test0"); // Adds an element
			    tasks.push_back("MM1");
			    tasks.push_back("14.14");
			    tasks.push_back("end");
			    tasks.push_back("Cleaning");
			    tasks.push_back("MM1");
			    tasks.push_back(location);
			    tasks.push_back("end");
			}
		
	if(colour == 'g')
			{
			//tasks.push_back("test0"); // Adds an element
			    tasks.push_back("MM1");
			    tasks.push_back("16.14");
			    tasks.push_back("end");
			    tasks.push_back("Cutting");
			    tasks.push_back("MM1");
			    tasks.push_back(location);
			    tasks.push_back("end");
			}


	return tasks;
}
