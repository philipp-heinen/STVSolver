class Compute
{
	public:
		Compute(Votes *election_result, int number_seats, std::string counting_method, std::string quota_type);
		//~Compute();
		
		void count_votes();
		int vote_transfer();
		int exclude_candidate();		
		
		double get_quota();
		std::vector<double> get_weights();
		std::vector<double> get_cand_votes();
		std::vector<int> get_elected_cand();
	private:
		void transfer_step();
		
		Votes *election_res;
		
		std::string method;
		std::string quottype;
		int seats;
		int n_cand;
		
		double eps = 0.0001;
		int max_iter = 100;
		
		double quota;
		std::vector<double> weights;
		std::vector<double> cand_votes;
		std::vector<std::vector<bool>> vote_contributing;
		
};

Compute::Compute(Votes *election_result, int number_seats, std::string counting_method, std::string quota_type)
{
	method = counting_method;
	quottype = quota_type;
	election_res = election_result;
	seats = number_seats;
	n_cand = election_result->get_n_cand();
	
	weights.resize(n_cand, 1.);
	
	if(method=="gregory"||method=="wright")
	{
		vote_contributing.resize(election_result->get_n_votes(),std::vector<bool>(n_cand));
	}
}

double Compute::get_quota()
{
	return quota;
}

std::vector<double> Compute::get_weights()
{
	return weights;
}

std::vector<double> Compute::get_cand_votes()
{
	return cand_votes;
}

std::vector<int> Compute::get_elected_cand()
{
	std::vector<int> elected_cand;
	for(int i=0; i<n_cand; i++)
	{
		if(cand_votes[i]>quota)
		{
			elected_cand.push_back(i);
		}
	}
	return elected_cand;
}

void Compute::count_votes()
{
	if(method=="gregory"||method=="wright")
	{
		cand_votes = election_res->count_candidate_votes_gregory(weights, vote_contributing, &vote_contributing);
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
}	

void Compute::transfer_step()
{
	count_votes();
	
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

int Compute::vote_transfer()
{
	std::vector<double> weights_temp;	
	int needed_iter;
	for(int i=0; i<max_iter; i++)
	{
		needed_iter = i;
		
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
	return needed_iter+1;
}

int Compute::exclude_candidate()
{
	weights[std::distance(cand_votes.begin(), std::min_element(cand_votes.begin(),cand_votes.end()))] = 0.;
	return std::distance(cand_votes.begin(), std::min_element(cand_votes.begin(),cand_votes.end()));
}
