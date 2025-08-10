#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>


#include "Votes.cpp"
#include "Compute.cpp"
#include "FileHandling.cpp"

void print_statistics(std::vector<double> votecount, std::vector<double> weights, double quota)
{
	std::cout<<"**********"<<std::endl;
	std::cout<<"Vote distribution:"<<std::endl<<"Candidate      #Votes"<<std::endl;
	for(int i=0; i<int(votecount.size()); i++)
	{
		std::cout<<i+1<<std::string(15-size(std::to_string(i+1)),' ')<<votecount[i]<<std::endl;
	}

	std::cout<<"Weights of candidates:"<<std::endl<<"Candidate      Weight"<<std::endl;	
	for(int i=0; i<int(weights.size()); i++)
	{
		std::cout<<i+1<<std::string(15-size(std::to_string(i+1)),' ')<<weights[i]<<std::endl;
	}
	
	std::cout<<"Quota for obtaining a seat is: "<<quota<<std::endl;
	std::cout<<"**********"<<std::endl;
}


int main(int argc, char **argv){
	std::vector<std::string> args(argv + 1, argv + argc);
	
	std::string method = "meek";
	std::string quota = "hare";
	int seats = 1;
	int n_cand = 1;
	
	bool random = false;
	
	bool cand_number_provided = false;
	
	std::string input_file;
	
	std::string output_file;
	bool file_output = false;
	
	for(int i=0; i<int(args.size())-1; i++)
	{
		if(args[i]=="-m"||args[i]=="--method")
		{
			if(args[i+1]=="gregory"||args[i+1]=="meek"||args[i+1]=="warren")
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
			cand_number_provided = true;
			try
			{
				n_cand = std::stoi(args[i+1]);
			}
			catch(const std::invalid_argument &e)
			{
				cand_number_provided = false;
				std::cout<<"Invalid number of candidates "<< args[i+1]<<", infering number of candidates from input file instead.\n";
			}
		}
		if(args[i]=="-f"||args[i]=="--file")
		{
			file_output = true;
			output_file = args[i+1];
		}
		if(args[i]=="-r"||args[i]=="--random")
		{
			random = true;
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
	
	FileHandling *file;
	
	if(file_output)
	{
		file = new FileHandling(input_file, output_file);
		file->create_outputfiles();
	}
	else
	{
		file = new FileHandling(input_file);
	}
	
	if(not file->read_input_file())
	{
		std::cout<<"Input file "<<input_file<<" could not be opened.\n";
		return EXIT_FAILURE;
	}

	if(not cand_number_provided)
	{
		n_cand = file->get_number_of_candidates();
	}
	
	Votes votes(n_cand);
	votes.read_votes(file->get_votes());
	
	Compute compute(&votes, seats, method, quota, random);
	
	bool break_by_excluded_cand;
	
	compute.count_votes();
	if(method!="gregory")
	{
		compute.compute_quota();
	}
	while(true)
	{
		print_statistics(compute.get_cand_votes(),compute.get_weights(), compute.get_quota());
		if(file_output)
		{
			file->write_votes_weights(compute.get_cand_votes(),compute.get_weights());
		}
		
		std::cout<<std::endl<<"Transfer votes..."<<std::endl;
		int iterations = compute.vote_transfer();
		std::cout<<"Needed "<<iterations<<" iterations for convergence."<<std::endl<<std::endl;
		
		print_statistics(compute.get_cand_votes(),compute.get_weights(),compute.get_quota());
		if(file_output)
		{
			file->write_votes_weights(compute.get_cand_votes(),compute.get_weights());
		}
		
		if(int(compute.get_elected_cand().size())==seats)
		{
			break_by_excluded_cand = false;
			break;
		}
		else if(int(compute.get_not_excluded_cand().size())<=seats)
		{
			break_by_excluded_cand = true;
			break;
		}
		else
		{
			std::cout<<std::endl<<"Exclude candidate "<<compute.exclude_candidate()+1<<"."<<std::endl<<std::endl;
		}
	}
	
	std::cout<<std::endl<<std::endl<<"Elected candidates are:"<<std::endl;
	std::vector<int> el_cand;
	if(break_by_excluded_cand)
	{
		el_cand = compute.get_not_excluded_cand();
	}
	else
	{
		el_cand = compute.get_elected_cand();
	}
	for(int i=0; i<int(el_cand.size()); i++)
	{
		std::cout<<el_cand[i]+1<<std::endl;
	}
	
	if(file_output)
	{
		file->write_elected_candidates(el_cand);
		file->close_outputfiles();
	}		
	
}
