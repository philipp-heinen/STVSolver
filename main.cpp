#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <fstream>


#include "Votes.cpp"
#include "Compute.cpp"
#include "FileHandling.cpp"


int main(int argc, char **argv){
	std::vector<std::string> args(argv + 1, argv + argc);
	
	std::string method = "meek";
	std::string quota = "hare";
	int seats = 1;
	int n_cand = 0;
	
	std::string input_file;
	
	bool file_output = false;
	
	for(int i=0; i<int(args.size())-2; i++)
	{
		if(args[i]=="-m"||args[i]=="--method")
		{
			if(args[i+1]=="gregory"||args[i+1]=="wright"||args[i+1]=="meek"||args[i+1]=="warren")
			{
				method = args[i+1];
			}
			else
			{
				std::cout<<"No method named "<< args[i+1]<<", using "<<method<<" instead.\n";
			}
		}
		if(args[i]=="-q"||args[i]=="--quota")
		{
			if(args[i+1]=="hare"||args[i+1]=="droop")
			{
				quota = args[i+1];
			}
			else
			{
				std::cout<<"No quota named "<< args[i+1]<<", using "<<quota<<" instead.\n";
			}
		}
		if(args[i]=="-s"||args[i]=="--seats")
		{
			try
			{
				seats = std::stoi(args[i+1]);
			}
			catch(const std::invalid_argument &e)
			{
				std::cout<<"Invalid number of seats "<< args[i+1]<<", using "<<seats<<" for number of seats instead.\n";
			}
		}
		if(args[i]=="-c"||args[i]=="--candidates")
		{
			try
			{
				n_cand = std::stoi(args[i+1]);
			}
			catch(const std::invalid_argument &e)
			{
				std::cout<<"Invalid number of candidates "<< args[i+1]<<", infering number of candidates from input file instead.\n";
			}
		}
		if(args[i]=="-f"||args[i]=="--file")
		{
			file_output = true;
		}
	}
	
	if(args.size()>0)
	{
		input_file = args[args.size()-1];
	}
	else
	{
		std::cout<<"No input file provided.";
		return EXIT_FAILURE;
	}
	
	
	
	
	FileHandling file(input_file);
	
	Votes votes(n_cand);
	votes.read_votes(file.read_input_file());
	
	Compute compute(&votes, seats, method, quota);
	
	if(method=="gregory"||method=="meek"||method=="warren")
	{
		compute.count_votes();
		while(true)
		{
			compute.vote_transfer();
			
			if(int(compute.get_elected_cand().size())==seats)
			{
				break;
			}
			else
			{
				compute.exclude_candidate();
			}
		}
	}
	else
	{
		
	}
	
	
	std::cout<<quota<<"\n"<<method<<"\n"<<seats<<"\n";
}
