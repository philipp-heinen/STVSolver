class FileHandling
{
	public:
		FileHandling(std::string input_filename);
		FileHandling(std::string input_filename, std::string output_filename);
		//void open_input()
		std::vector<std::vector<int>> read_input_file();
		
	private:
		std::string input_file;
		std::string output_file;
		std::fstream file_in;
		std::fstream file_out;
};


FileHandling::FileHandling(std::string input_filename)
{
	input_file = input_filename;
}

FileHandling::FileHandling(std::string input_filename, std::string output_filename)
{
	input_file = input_filename;
	output_file = output_filename;
}

std::vector<std::vector<int>> FileHandling::read_input_file()
{
	std::vector<std::vector<int>> votes;
    file_in.open(input_file, std::fstream::in);
    
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
    
    return votes;
}
