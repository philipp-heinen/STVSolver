class Votes
{
	public:
		Votes(int number_candidates);
		
		void read_vote(std::vector<int> vote_vector);
		void read_votes(std::vector<std::vector<int>> votes_vector);
		bool check_correctness_vote(int i);
		bool check_correctness_votes();
		
		std::vector<double> count_candidate_votes_gregory(std::vector<double> weights, std::vector<std::vector<bool>> vote_contributing, std::vector<std::vector<bool>> *vote_contributing_out);
		std::vector<double> count_candidate_votes_meek(std::vector<double> weights);
		std::vector<double> count_candidate_votes_warren(std::vector<double> weights);
		
		int get_n_cand();
		int get_n_votes();
		
	private:
		int n_cand;
		std::vector<std::vector<int>> votes;
};

Votes::Votes(int number_candidates)
{
	n_cand = number_candidates;
}

void Votes::read_vote(std::vector<int>vote_vector)
{
	votes.push_back(vote_vector);
}

void Votes::read_votes(std::vector<std::vector<int>> votes_vector)
{
	votes = votes_vector;
}

std::vector<double> Votes::count_candidate_votes_gregory(std::vector<double> weights, std::vector<std::vector<bool>> eligible_votes, std::vector<std::vector<bool>> *contributing_votes)
{
	if(int(weights.size()) != n_cand)
	{
		std::cout<<"Number of weights does not match number of candidates.";
	}
	
	for(int i=0; i<int(votes.size()); i++)
	{
		for(int j=0; j<n_cand; j++)
		{
			(*contributing_votes)[i][j] = false;
		}
	}
	
	std::vector<double> cand_votes(n_cand, 0.);
	
	for(int i=0; i < int(votes.size()); i++)
	{
		double r = 1.;
		for(int j=0; j < int(votes[i].size()); j++)
		{
			if(votes[i][j]<n_cand)
			{
				if(eligible_votes[i][votes[i][j]])
				{
					cand_votes[votes[i][j]] += r*weights[votes[i][j]];
					if(r*weights[votes[i][j]]>0.)
					{
						(*contributing_votes)[i][votes[i][j]] = true;
					}
					r *= 1.-weights[votes[i][j]];
				}
			}
			else
			{
				std::cout<<"Invalid entry at position "<<j<<" of vote "<<i<<".\n";
			}
			if(r<1e-20)
			{
				break;
			}
		}
	} 
	return cand_votes;
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
				std::cout<<"Invalid entry at position "<<j<<" of vote "<<i<<".\n";
			}
			if(r<1e-20)
			{
				break;
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
				std::cout<<"Invalid entry at position "<<j<<" of vote "<<i<<".\n";
			}
			if(r<1e-20)
			{
				break;
			}
		}
	} 
	return cand_votes;
}

int Votes::get_n_cand()
{
	return n_cand;
}

int Votes::get_n_votes()
{
	return int(votes.size());
}
