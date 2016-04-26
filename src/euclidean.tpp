/*!
 \file euclidean.tpp
 \author adeline.bailly@uhb.fr
*/

#ifndef EUCLIDEAN_TPP
#define EUCLIDEAN_TPP

template<typename T>
double ED::euclDistance(const std::vector<T>& v, const std::vector<T>& w)
{
	if( (v.size() == 0 || w.size() == 0) || (v.size() != w.size()) )
		UTILS::_exit("Impossible to compute ED, check vectors' sizes.");

	double eucl = 0.;

	for(unsigned i=0; i<v.size(); i++)
		eucl += ((v[i] - w[i]) * (v[i] - w[i]));

	eucl = sqrt(eucl);

	if(isnan(eucl))
		eucl = DBL_MAX;

	return eucl;
}

template<typename T>
unsigned int ED::euclDistance_vec(const std::vector<T>& v, const std::vector< std::vector<T> >& m)
{
	unsigned int idx = 0;
	unsigned int j;
	
	double dist;
	double best = DBL_MAX;

	for(unsigned int i=0; i<m.size(); ++i)
	{
		dist = 0.;
		j = -1;
		
		while( (dist<best) && (++j<v.size()) )
			dist += ((v[j] - m[i][j]) * (v[j] - m[i][j]));

		if(dist < best)
		{
			best = dist;
			idx = i;
		}
	}

	return idx;
}

#endif // EUCLIDEAN_TPP
