class FileHandling
{
	public:
		FileHandling(std::string input_filename);
		FileHandling(std::string input_filename, std::string output_filename_base);
		bool read_input_file();
		std::vector<std::vector<int>> get_votes();
		int get_number_of_candidates();
		
		void create_outputfiles();
		void close_outputfiles();
		void write_elected_candidates(std::vector<int> el_cand);
		void write_votes_weights(std::vector<double> votecount, std::vector<double> weights);
		
	private:
		std::string input_file;
		std::string output_file_votes;
		std::string output_file_weights;
		std::string output_file_elected_candidates;
		
		std::fstream file_in;
		std::fstream file_out_votes;
		std::fstream file_out_weights;
		std::fstream file_out_elected_candidates;
		
		std::vector<std::vector<int>> votes;
};


FileHandling::FileHandling(std::string input_filename)
{
	input_file = input_filename;
}

FileHandling::FileHandling(std::string input_filename, std::string output_filename_base)
{
	input_file = input_filename;
	output_file_votes = output_filename_base+"_votes.csv";
	output_file_weights = output_filename_base+"_weights.csv";
	output_file_elected_candidates = output_filename_base+"_elected_candidates.csv";
}

bool FileHandling::read_input_file()
{
	votes.clear();
	
	file_in.open(input_file, std::fstream::in);
	
	if(file_in.fail())
	{
		return false;
	}
    
    std::string line;
    
    int i=1;
    while(std::getline(file_in, line))
    {
		std::stringstream line_stream(line);
		std::string temp;
		std::vector<int> vote;
		while(std::getline(line_stream, temp, ','))
		{
			try
			{
				vote.push_back(std::stoi(temp)-1);
			}
			catch(const std::invalid_argument &e)
			{
				std::cout<<"Invalid entry in vote "<<i<<".\n";
			}
		}
		votes.push_back(vote);
		i++;
	} 
    
    file_in.close();
    
    return true;
}

std::vector<std::vector<int>> FileHandling::get_votes()
{
	return votes;
}

void FileHandling::create_outputfiles()
{
	file_out_votes.open(output_file_votes, std::fstream::out);
	file_out_weights.open(output_file_weights, std::fstream::out);
	file_out_elected_candidates.open(output_file_elected_candidates, std::fstream::out);
}

void FileHandling::close_outputfiles()
{
	file_out_votes.close();
	file_out_weights.close();
	file_out_elected_candidates.close();
}

void FileHandling::write_votes_weights(std::vector<double> votecount, std::vector<double> weights)
{
	for(int i=0; i<int(votecount.size()); i++)
	{
		file_out_votes<<votecount[i];
		if(i!=int(votecount.size())-1)
		{
			file_out_votes<<",";
		}
	}
	file_out_votes<<"\n";
	
	for(int i=0; i<int(weights.size()); i++)
	{
		file_out_weights<<weights[i];
		if(i!=int(weights.size())-1)
		{
			file_out_weights<<",";
		}
	}
	file_out_weights<<"\n";
}

void FileHandling::write_elected_candidates(std::vector<int> el_cand)
{
	for(int i=0; i<int(el_cand.size()); i++)
	{
		file_out_elected_candidates<<el_cand[i]+1;
		if(i!=int(el_cand.size())-1)
		{
			file_out_elected_candidates<<",";
		}
	}
}

int FileHandling::get_number_of_candidates()
{
	std::vector<int> max_per_vote;
	for(int i=0; i<int(votes.size()); i++)
	{
		max_per_vote.push_back(*std::max_element(votes[i].begin(), votes[i].end()));
	}
	return *std::max_element(max_per_vote.begin(), max_per_vote.end())+1;
}
