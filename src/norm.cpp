/*!
 \file norm.cpp
 \author adeline.bailly@uhb.fr
*/

#include "../inc/norm.hpp"

void NORM::norm_bow_ssr(
	std::vector< std::vector<double> > & histo
	)
{
	for(unsigned int ii=0u; ii<histo.size(); ++ii)
		for(auto iit = histo[ii].begin(); iit != histo[ii].end(); ++iit)
			*iit = std::sqrt(*iit);
}


void NORM::norm_bow_idf_train(
	std::vector< std::vector<double> > & histo,
	std::vector<unsigned int> & idf_nb_apparition
	)
{
	unsigned int i, j;
	unsigned int sz = histo[0].size();
	const unsigned int sz_train = histo.size();
	double sum;

	idf_nb_apparition.assign(sz, 0u);

	for(i=0; i<histo.size(); ++i)
	{
		for(j=0; j<sz; ++j)
			if(histo[i][j] > 0)
				++idf_nb_apparition[j];
	}

	for(i=0; i<histo.size(); ++i)
	{
		sum = std::accumulate(histo[i].begin(), histo[i].end(), 0.);
		
		for(j=0; j<sz; ++j)
			histo[i][j] = (histo[i][j] / sum) * log(sz_train / idf_nb_apparition[j]);
	}
}


void NORM::norm_bow_idf_test(
	std::vector< std::vector<double> > & histo,
	std::vector<unsigned int> & idf_nb_apparition,
	const unsigned int & sz_train
	)
{
	unsigned int i, j;
	unsigned int sz = histo[0].size();
	double sum;

	for(i=0; i<histo.size(); ++i)
	{
		sum = std::accumulate(histo[i].begin(), histo[i].end(), 0.);
		
		for(j=0; j<sz; ++j)
			histo[i][j] = (histo[i][j] / sum) * log(sz_train / idf_nb_apparition[j]);
	}
}


void NORM::norm_bow(
	std::vector< std::vector<double> > & histo
	)
{
	double m;
	for(unsigned int ii=0; ii<histo.size(); ++ii)
	{
		m = NORM::l2_norm<double>(histo[ii]);
		
		for(auto iit = histo[ii].begin(); iit != histo[ii].end(); ++iit)
			*iit /= m;
	}
}
