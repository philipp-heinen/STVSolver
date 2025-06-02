class ComputeWinners
{
	public:
		ComputeWinners(Votes *election_result, unsigned int number_seats, std::string counting_method, std::string quota_type);
		~ComputeWinners();
	private:
		void iteration_step();
		void exclude_candidate();
		
		Votes *election_res;
		std::string method;
		std::string quottype;
		unsigned int seats;
		
		double quota;
		std::vector<double> weights;
		std::vector<double> cand_votes;
		
};

ComputeWinners::ComputeWinners(Votes *election_result, unsigned int number_seats, std::string counting_method, std::string quota_type)
{
	method = counting_method;
	quottype = quota_type;
	election_res = election_result;
	
	seats = election_res->get_n_cand();
}

void ComputeWinners::iteration_step()
{
	if(method=="gregory"||method=="wright")
	{
		cand_votes = election_res->count_candidate_votes_gregory(weights);
	}
	else if(method=="meek")
	{
		cand_votes = election_res->count_candidate_votes_meek(weights);
	}
	else if(method=="warren")
	{
		cand_votes = election_res->count_candidate_votes_warren(weights);
	}
	else
	{	
		std::cout<<method<<": No such method exists."; 
	}
	
	double total_vote = std::reduce(cand_votes.begin(), cand_votes.end());
	
	if(quottype=="hare")
	{
		quota = total_vote/double(seats);
	}
	else if(quottype=="droop")
	{
		quota = total_vote/double(seats+1);
	}
	else
	{
		std::cout<<quottype<<": No such quota type exists.";
	}
	
	for(unsigned int i=0; i < cand_votes.size(); i++)
	{
		if(cand_votes[i]<quota)
		{
			weights[i] = 1.;
		}
		else
		{
			weights[i] = quota/cand_votes[i];
		}
	}
}

void ComputeWinners::exclude_candidate()
{
	weights[std::distance(cand_votes.begin(), std::min_element(cand_votes.begin(),cand_votes.end()))] = 0.;
}
