class ComputeWinners
{
	public:
		ComputeWinners(Votes *election_result, int number_seats, std::string counting_method, std::string quota_type);
		~ComputeWinners();
		
		double get_quota();
		std::vector<double> get_weights();
		std::vector<double> get_cand_votes();
	private:
		void transfer_step();
		int vote_transfer();
		int exclude_candidate();
		
		Votes *election_res;
		std::string method;
		std::string quottype;
		int seats;
		
		double eps = 0.0001;
		int max_iter = 100;
		
		double quota;
		std::vector<double> weights;
		std::vector<double> cand_votes;
		
};

ComputeWinners::ComputeWinners(Votes *election_result, int number_seats, std::string counting_method, std::string quota_type)
{
	method = counting_method;
	quottype = quota_type;
	election_res = election_result;
	
	seats = election_res->get_n_cand();
}

double ComputeWinners::get_quota()
{
	return quota;
}

std::vector<double> ComputeWinners::get_weights()
{
	return weights;
}

std::vector<double> ComputeWinners::get_cand_votes()
{
	return cand_votes;
}

void ComputeWinners::transfer_step()
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
	
	for(int i=0; i < int(cand_votes.size()); i++)
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

int ComputeWinners::vote_transfer()
{
	std::vector<double> weights_temp;	
	int needed_iter;
	for(int i=0; i<max_iter; i++)
	{
		weights_temp = weights;
		
		transfer_step();
		
		bool converged = false;
		for(int j=0; j<int(weights.size()); j++)
		{
			if(std::abs(weights[j]-weights_temp[j])/weights_temp[j]<eps)
			{
				converged = true;
			}
		}
		if(converged)
		{
			break;
		}
		if(i==max_iter-1)
		{
			std::cout<<"Did not converge within maximum number of iterations ("<<max_iter<<")";
		}
	}
}

int ComputeWinners::exclude_candidate()
{
	weights[std::distance(cand_votes.begin(), std::min_element(cand_votes.begin(),cand_votes.end()))] = 0.;
	return std::distance(cand_votes.begin(), std::min_element(cand_votes.begin(),cand_votes.end()));
}
