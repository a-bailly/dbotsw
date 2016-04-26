/*!
 \file param.cpp
 \author adeline.bailly@uhb.fr
*/

#include "../inc/param.hpp"

void set_default_parameters(
		PARAM& param
	)
{
	param.sigma = 1.6;
	param.k_sc = 1.257013374521;
	param.threshold = 0.0;
	param.n_sc = 0u;
	param.dense = 1;
	param.norm_bow = true;
	param.norm_bow_idf = false;
	param.norm_bow_ssr = true;
	param.k_fold = 1u;

	{   static const unsigned int arr[] = {4, 8};
		param.v_szblock.assign(arr, arr+sizeof(arr)/sizeof(arr[0]));
	}
	{   static const unsigned int arr[] = {4, 8, 12, 16, 20};
		param.v_nblocks.assign(arr, arr+sizeof(arr)/sizeof(arr[0]));
	}
	{   static const unsigned int arr[] = {32, 64, 128, 256, 512, 1024};
		param.v_k.assign(arr, arr+sizeof(arr)/sizeof(arr[0]));
	}
	{   static const double arr[] = {1., 10., 100.};
		param.v_c_svm.assign(arr, arr+sizeof(arr)/sizeof(arr[0]));
	}
}


void read_parameters(
	const std::string& s,
	PARAM& param
	)
{
	bool b;
	double d;
	unsigned int ui;
	std::vector<unsigned int> v;
	std::vector<double> vd;

	std::ifstream ifile(s.c_str());

	if(!ifile)
		UTILS::_exit("[ERROR] Can't open one of the file containing parameters");

	std::string line, str;

	while(getline(ifile, line))
	{
		std::istringstream input(line, std::istringstream::in);
		getline(input, str, ' ');

		if(str == "k_sc")
		{   input >> d;
			param.k_sc = d;
		}
		else if(str == "sigma")
		{   input >> d;
			param.sigma = d;
		}
		else if(str == "threshold")
		{   input >> d;
			param.threshold = d;
		}
		else if(str == "sz_block")
		{   v.clear();
			while(input >> ui)
				v.push_back(ui);
			param.v_szblock = v;
		}
		else if(str == "n_sc")
		{   input >> ui;
			param.n_sc = ui;
		}
		else if(str == "n_blocks")
		{   v.clear();
			while(input >> ui)
			{
				if(ui % 2)
					UTILS::_exit("Number of blocks must be even.");
				v.push_back(ui);
			}
			param.v_nblocks = v;
		}
		else if(str == "k")
		{   v.clear();
			while(input >> ui)
				v.push_back(ui);
			param.v_k = v;
		}
		else if(str == "c_svm")
		{   vd.clear();
			while(input >> d)
				vd.push_back(d);
			param.v_c_svm = vd;
		}
		else if(str == "dense")
		{   input >> ui;
			param.dense = ui;
		}
		else if(str == "norm_bow")
		{   input >> b;
			param.norm_bow = b;
		}
		else if(str == "norm_bow_idf")
		{   input >> b;
			param.norm_bow_idf = b;
		}
		else if(str == "norm_bow_ssr")
		{   input >> b;
			param.norm_bow_ssr = b;
		}
		else if(str == "k_fold")
		{   input >> ui;
			param.k_fold = ui;
		}
		else
			std::cout << "parameter " << str << " not known" << std::endl;
	}

	ifile.close();
}


void print_parameters(
	std::ostream& os,
	const PARAM& param
	)
{
	os << "k_sc " << param.k_sc << std::endl;
	os << "sigma " << param.sigma << std::endl;
	os << "threshold " << param.threshold << std::endl;
	os << "n_sc " << param.n_sc << std::endl;
	os << "sz_block ";
	for(unsigned int i=0; i<param.v_szblock.size(); ++i)
		os << param.v_szblock[i] << " ";
	os << std::endl;
	os << "n_blocks ";
	for(unsigned int i=0; i<param.v_nblocks.size(); ++i)
		os << param.v_nblocks[i] << " ";
	os << std::endl;
	os << "k ";
	for(unsigned int i=0; i<param.v_k.size(); ++i)
		os << param.v_k[i] << " ";
	os << std::endl;
	os << "c_svm ";
	for(unsigned int i=0; i<param.v_c_svm.size(); ++i)
		os << param.v_c_svm[i] << " ";
	os << std::endl;
	os << "dense " << param.dense << std::endl;
	os << "norm_bow " << param.norm_bow << std::endl;
	os << "norm_bow_idf " << param.norm_bow_idf << std::endl;
	os << "norm_bow_ssr " << param.norm_bow_ssr << std::endl;
	os << "k_fold " << param.k_fold << std::endl;
}
