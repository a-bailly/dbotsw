/*!
 \file sift.cpp
 \author adeline.bailly@uhb.fr
*/

#include "../inc/sift.hpp"

std::vector<double> SIFT::g(const double & sigma)
{
	std::vector<double> vg;
	int qs = (int) (4* sigma);
	vg.resize(1 + 2*qs);

	int x, y = -1;
	for(x=-qs; x<=qs; ++x)
		vg[++y] = exp(-1. * x * x / (2.*sigma*sigma) ) / ( sqrt(2. * M_PI) * sigma);

	return vg;
}


std::vector<double> SIFT::g(const double & sigma, const int& length)
{
	std::vector<double> vg;
	if(length % 2)
		vg.resize(length);
	else
		vg.resize(length+1);

	int x, l = vg.size()/2;
	for(x=1; x<=l; ++x)
	{
		vg[l-x] = exp(-1. * x * x / (2.*sigma*sigma) ) / ( sqrt(2. * M_PI) * sigma);
		vg[l+x] = exp(-1. * x * x / (2.*sigma*sigma) ) / ( sqrt(2. * M_PI) * sigma);
	}

	vg[l] = 1. / ( sqrt(2. * M_PI) * sigma);

	double e = *std::max_element(vg.begin(), vg.end());
	
	for(unsigned int i=0; i<vg.size(); ++i)
		vg[i] /= e;

	return vg;
}


std::vector<double> SIFT::get_neighboors(const std::vector< std::vector<double> >& dog, const int& tm, const int& sc)
{
	int tmin = std::max(tm-1, 0);
	int tmax = std::min(tm+1, (int)dog.size()-1);

	int scmin = std::max(sc-1, 0);
	int scmax = std::min(sc+1, (int)dog[0].size()-1);

	std::vector<double> n;
	int i, j;

	for(i=tmin; i<=tmax; ++i)
		for(j=scmin; j<=scmax; ++j)
			if(j!=sc || i != tm)
				n.push_back(dog[i][j]);

	return n;
}


std::vector< SIFT::ipoints > SIFT::interest_points(const std::vector< std::vector<double> >& dog, const double& threshold)
{
	int i, j;
	std::vector<double> n;
	std::vector< SIFT::ipoints > vip;
	ipoints ip;

	int sz = (int) dog.size() -1; // Last "Dog[]"
	int szz;

	for(i=1; i<sz; ++i)
	{
		ip.sc = i;
		szz = (int) dog[i].size();

		for(j=0; j<szz; ++j)
		{
			n.clear();
			n = get_neighboors(dog, i, j);

			if( dog[i][j] < *std::min_element(n.begin(), n.end()) || dog[i][j] > *std::max_element(n.begin(), n.end()) )
			{
				ip.tm = j;
				if(dog[i][j] > threshold || dog[i][j] < -threshold)
					vip.push_back(ip);
			}
		}
	}

	return vip;
}


std::vector< SIFT::ipoints > SIFT::dense_points(const std::vector< std::vector<double> >& dog, const unsigned int& dense)
{
	int i, j;
	std::vector< SIFT::ipoints > vip;
	ipoints ip;

	int sz = (int) dog.size() -1; // Last "Dog[]"
	int szz;

	int step_sc = 1;
	for(i=1; i<sz; i+= step_sc)
	{
		ip.sc = i;
		szz = (int) dog[i].size();

		for(j=0; j<szz; j+=dense)
		{
			ip.tm = j;
			vip.push_back(ip);
		}
	}

	return vip;
}


std::vector< std::vector<float> > SIFT::get_feature_vectors(
	const std::vector< std::vector<double> >& l,
	const std::vector< SIFT::ipoints >& vip,
	const unsigned int& n_b,
	const unsigned int& a
)
{

	std::vector< std::vector<float> > vdiff, vn;
	unsigned int i, j, mx;

	if(n_b*a > l[0].size())
		return vn;

	int nb_on2 = .5*n_b;

	vdiff.resize(vip.size());
	
	for(auto it=vdiff.begin(); it!=vdiff.end(); ++it)
		(*it).assign(a*n_b+1, 0.);

	std::vector<double> gaussian_filter;
	gaussian_filter = SIFT::g(nb_on2*a, n_b*a);

	std::vector< std::vector<double> > gradient_l;
	gradient_l.resize(l.size());

	for(i=0; i<l.size(); ++i)
	{
		gradient_l[i].resize(l[i].size());

		gradient_l[i][0] = l[i][1] - l[i][0];
		for(j=1; j<l[i].size()-1; ++j)
			gradient_l[i][j] = (l[i][j+1] - l[i][j-1]) * 0.5;
		gradient_l[i][j] = l[i][j] - l[i][j-1];
	}

	int tm, sc;
	
	for(i=0; i<vdiff.size(); ++i)
	{
		sc = vip[i].sc;
		tm = vip[i].tm - a*nb_on2;
		mx = vip[i].tm + a*nb_on2 - 1;

		if(tm > 0 && mx < l[0].size())
		{
			for(j=0; j<=a*n_b; ++j)
				vdiff[i][j] = (float) gaussian_filter[j] * gradient_l[sc][tm+j];
		}
		else // If near extrema
		{
			for(j=0; j<=a*n_b; ++j)
				if( (tm+j) < l[0].size() && (tm+j) > 0 )
					vdiff[i][j] = (float) gaussian_filter[j] * gradient_l[sc][tm+j];
		}
		vdiff[i].erase(vdiff[i].begin()+(int)(vdiff[i].size()/2));
	}

	vn.resize(vip.size());

	for(auto it=vn.begin(); it!=vn.end(); ++it)
		(*it).resize(2*n_b);

	for(i=0; i<vn.size(); ++i)
	{
		for(j=0; j<n_b; ++j)
		{
			for(mx=0; mx<a; ++mx)
			{
				if(vdiff[i][j*a+mx] < 0)
					vn[i][2*j] -= vdiff[i][j*a+mx];
				else
					vn[i][2*j+1] += vdiff[i][j*a+mx];
			}
		}
	}

	vdiff.clear();

	return vn;
}

std::ostream& operator << (std::ostream &os, const SIFT::st_sift &s)
{
	return os << "dog\n" << s.dog << "l\n" << s.l;
}

std::ostream& operator << (std::ostream &os, const SIFT::ipoints &p)
{
	return os << p.tm << " " << p.sc << " ";
}
