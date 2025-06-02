class ComputeWinners
{
	public:
		ComputeWinners(Votes *election_result, unsigned int number_seats, string counting_method, string quota_type);
		~ComputeWinners();
	private:
		void iteration_step();
		
		Votes *election;
		string method;
		string quota;
		unsigned int seats;
		
		std::vector<double> weights;
		
};

ComputeWinners::ComputeWinners(Votes *election_result, unsigned int number_seats,string counting_method, string quota_type)
{
	method = counting_method;
	quota = quota_type;
	election = election_result;
	
}

void ComputeWinners::iteration_step()
{
	
}
