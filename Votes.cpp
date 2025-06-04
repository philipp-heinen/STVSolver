class Votes
{
	public:
		Votes(int number_candidates);
		~Votes();
		
		void read_vote(std::vector<int> vote);
		
		std::vector<double> count_candidate_votes_gregory(std::vector<double> weights);
		std::vector<double> count_candidate_votes_meek(std::vector<double> weights);
		std::vector<double> count_candidate_votes_warren(std::vector<double> weights);
		
		int get_n_cand();
		
	private:
		int n_cand;
		std::vector<std::vector<int>> votes;
};

Votes::Votes(int number_candidates)
{
	n_cand = number_candidates;
}

void Votes::read_vote(std::vector<int>vote)
{
	votes.push_back(vote);
}

std::vector<double> Votes::count_candidate_votes_gregory(std::vector<double> weights)
{
}

std::vector<double> Votes::count_candidate_votes_meek(std::vector<double> weights)
{
	if(int(weights.size()) != n_cand)
	{
		std::cout<<"Number of weights does not match number of candidates.";
	}
	
	std::vector<double> cand_votes(n_cand, 0.);
	
	for(int i=0; i < int(votes.size()); i++)
	{
		double r = 1.;
		for(int j=0; j < int(votes[i].size()); j++)
		{
			if(votes[i][j]<n_cand)
			{
				cand_votes[votes[i][j]] += r*weights[votes[i][j]];
				r *= 1.-weights[votes[i][j]];
			}
			else
			{
				std::cout<<"Invalid entry at position "<<j<<" of vote "<<i;
			}
		}
	} 
	return cand_votes;
}

std::vector<double> Votes::count_candidate_votes_warren(std::vector<double> weights)
{
	if(int(weights.size()) != n_cand)
	{
		std::cout<<"Number of weights does not match number of candidates.";
	}
	
	std::vector<double> cand_votes(n_cand, 0.);
	
	for(int i=0; i < int(votes.size()); i++)
	{
		double r = 1.;
		for(int j=0; j < int(votes[i].size()); j++)
		{
			if(votes[i][j]<n_cand)
			{
				cand_votes[votes[i][j]] += std::min(r, weights[votes[i][j]]);
				r -= weights[votes[i][j]];
			}
			else
			{
				std::cout<<"Invalid entry at position "<<j<<" of vote "<<i;
			}
		}
	} 
	return cand_votes;
}

int Votes::get_n_cand()
{
	return n_cand;
}
