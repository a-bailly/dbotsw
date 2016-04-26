/*!
 \file cross_validation.cpp
 \author adeline.bailly@uhb.fr
*/

#include "../inc/cross_validation.hpp"

void CROSS_V::update_best_param(
	const std::vector<double> & acc,
	double & best,
	std::vector<cv_param> & best_param,
	unsigned int n_blocks,
	unsigned int sz_block,
	unsigned int k_means,
	std::vector<double> v_c_svm
	)
{
	for(unsigned int k=0; k<acc.size(); ++k)
	{
		if(best < acc[k])
		{
			best_param.resize(1);
			
			best_param[0].n_blocks = n_blocks;
			best_param[0].sz_block = sz_block;
			best_param[0].k_means = k_means;
			best_param[0].c_svm = v_c_svm[k];
			
			best = acc[k];
		}
		else if(best == acc[k])
		{
			best_param.resize(best_param.size()+1);
			
			int z = best_param.size() -1;
			
			best_param[z].n_blocks = n_blocks;
			best_param[z].sz_block = sz_block;
			best_param[z].k_means = k_means;
			best_param[z].c_svm = v_c_svm[k];
		}
	}
}

void CROSS_V::print_cv_svm(
	std::ostream& os,
	const std::vector<cv_param>& bp
	)
{
	unsigned int i;
	os << "n_blocks ";
	for(i=0; i<bp.size(); ++i)
		os << bp[i].n_blocks << " ";
	os << std::endl;
	os << "sz_block ";
	for(i=0; i<bp.size(); ++i)
		os << bp[i].sz_block << " ";
	os << std::endl;
	os << "k ";
	for(i=0; i<bp.size(); ++i)
		os << bp[i].k_means << " ";
	os << std::endl;
	os << "c_svm ";
	for(i=0; i<bp.size(); ++i)
		os << bp[i].c_svm << " ";
	os << std::endl;

}


bool CROSS_V::read_cv_svm(
	std::string& s,
	std::vector<cv_param>& bp
	)
{
	unsigned int ui;
	std::vector<unsigned int> v;

	std::ifstream ifile(s.c_str());

	if(!ifile)
	{	std::cout << "[ERROR] Can't read LOOCV SVM parameters" << std::endl;
		return false;
	}

	std::string line, str;

	getline(ifile, line);
	{
		std::istringstream input(line, std::istringstream::in);
		getline(input, str, ' ');

		// n_blocks
		v.clear();
		
		while(input >> ui)
			v.push_back(ui);

		bp.resize(v.size());

		for(ui = 0; ui < v.size(); ++ui)
			bp[ui].n_blocks = v[ui];
	}

	getline(ifile, line);
	{
		std::istringstream input(line, std::istringstream::in);
		getline(input, str, ' ');

		// sz_block
		v.clear();
		
		while(input >> ui)
			v.push_back(ui);
		
		for(ui = 0; ui < v.size(); ++ui)
			bp[ui].sz_block = v[ui];
	}

	getline(ifile, line);
	{
		std::istringstream input(line, std::istringstream::in);
		getline(input, str, ' ');

		// k_means
		v.clear();
		
		while(input >> ui)
			v.push_back(ui);
		
		for(ui = 0; ui < v.size(); ++ui)
			bp[ui].k_means = v[ui];
	}

	getline(ifile, line);
	{
		std::istringstream input(line, std::istringstream::in);
		getline(input, str, ' ');
	
		// c_svm
		double d;
		std::vector<double> vd;
		
		while(input >> d)
			vd.push_back(d);
		
		for(ui = 0; ui < vd.size(); ++ui)
			bp[ui].c_svm = vd[ui];
	}

	ifile.close();

	return true;
}


std::ostream& operator << (
	std::ostream &os,
	const CROSS_V::cv_param &p
	)
{
	return os << p.n_blocks << " " << p.sz_block << " " << p.k_means << " " << p.c_svm << "; ";
}
