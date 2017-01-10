/*!
 \file sift.tpp
 \author adeline.bailly@uhb.fr
*/

#ifndef SIFT_TPP
#define SIFT_TPP

template<typename T>
std::vector<double> SIFT::l(const std::vector<T>& ts, const double& sigma)
{
	std::vector<double> r, vg;
	r.assign(ts.size(), 0.);

	int i, j, k, m;
	int dec;

	vg = SIFT::g(sigma);
	
	dec = (int) ((vg.size() + 1) * 0.5);

	for(i=0; i<ts.size(); ++i)
	{
		
		k = i-dec;
		m = 1;

		for(j=0; j<vg.size(); ++j)
		{
			if(fabs(++k)<ts.size())
			{
				r[i] += (vg[j] * ts[fabs(k)]);
			}
			else
			{
				if(m<ts.size())
				{
					r[i] += (vg[j] * ts[ts.size()-(++m)]);
				}
				
			}
		}
	}
	
	return r;
}


template<typename T>
SIFT::st_sift SIFT::difference_of_gaussians(
	const std::vector<T>& ts,
	const double& sigma,
	const double& k_sc,
	const unsigned int& n_sc
	)
{
	unsigned int size = ts.size();

	SIFT::st_sift res;

	res.l.resize(n_sc+3);

	res.dog.resize(n_sc+2);
	for(auto it = res.dog.begin(); it != res.dog.end(); ++it)
		(*it).resize(size);

	unsigned int i, j;

	res.l[0] = l(ts, sigma / k_sc);
	res.l[1] = l(ts, sigma);
	
	for(i=0; i<size; ++i)
		res.dog[0][i] = res.l[1][i] - res.l[0][i];

	// RANG NORMAUX
	for(j=1; j<res.dog.size(); ++j)
	{
		res.l[j+1] = l(ts, pow(k_sc, j) * sigma);
		
		for(i=0; i<size; ++i)
			res.dog[j][i] = res.l[j+1][i] - res.l[j][i];
	}

	return res;
}


template<typename T>
void SIFT::get_ip(
	const std::vector< TS::time_series_class<T> >& x,
	std::vector< std::vector<SIFT::ipoints> >& vec_ip,
	std::vector<SIFT::st_sift>& vec_st,
	const PARAM& p
)
{
	SIFT::st_sift st;

	if(!p.dense)
	{
		for(auto jt=x.begin(); jt!=x.end(); ++jt)
		{
			st = SIFT::difference_of_gaussians((*jt).ts, p.sigma, p.k_sc, p.n_sc);
			std::vector< SIFT::ipoints > ip = SIFT::interest_points(st.dog, p.threshold);
			
			vec_ip.push_back(ip);
			vec_st.push_back(st);
		}
	}
	else
	{
		for(auto jt=x.begin(); jt!=x.end(); ++jt)
		{
			st = SIFT::difference_of_gaussians((*jt).ts, p.sigma, p.k_sc, p.n_sc);
			std::vector< SIFT::ipoints > ip = SIFT::dense_points(st.dog, p.dense);
			
			vec_ip.push_back(ip);
			vec_st.push_back(st);
		}
	}
}

#endif // SIFT_TPP
