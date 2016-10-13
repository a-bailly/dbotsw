/*!
 \file botsw.cpp
 \author adeline.bailly@uhb.fr */
#include "../inc/botsw.hpp"

namespace fs = boost::filesystem;

int main(int argc, char *argv[]) {
	PARAM param;
	set_default_parameters(param);
	
	std::string folder = ""; std::string trainfile = "";
	std::string testfile = ""; std::string paramfile = "";
	std::string output = "";
	
	static struct option long_options[] = {
		{"folder", 1, 0, 'f'}, {"train", 1, 0, 't'},
		{"test", 1, 0, 'x'}, {"paramfile", 1, 0, 'p'},
		{"outputfile", 1, 0, 'o'},	{NULL, 0, NULL, 0}
	};
	
	int option_index = 0, c;
	
	while ((c = getopt_long(argc, argv, "f:t:x:p:o",
				 long_options, &option_index)) != -1)
	{
		switch (c)
		{
			case 'f':
				folder = optarg;
				break;
			case 't':
				trainfile = optarg;
				break;
			case 'x':
				testfile = optarg;
				break;
			case 'p':
				paramfile = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			default:
				std::cout << "Unrecognized option : --" << c << std::endl;
		}
	}
	
	if (optind < argc)
	{
		std::cout << "Nonexistent(s) option(s) : ";
		while (optind < argc)
			std::cout << argv[optind++] << " ";
		std::cout << std::endl;
	}
	
	{
		param.v_szblock.clear();	param.v_szblock.push_back(4);
		param.v_nblocks.clear();	param.v_nblocks.push_back(12);
		param.v_k.clear();
		param.v_c_svm.clear();
	}
	
	if (folder != "")
	{
		if(!fs::exists(folder))
			UTILS::_exit("Input folder not found");
		size_t a = 0, b = 0;
		char ch = folder.back();
		if( ch == '/')
			folder.resize(folder.size()-1);
		while((a = folder.find('/', ++a)) != std::string::npos)
			b = a;
		testfile.assign(folder, b+1, folder.size()-b);
		trainfile = folder + '/' + testfile + "_TRAIN";
		testfile = folder + '/' + testfile + "_TEST";
	}
	else
	{
		if (trainfile != "")
		{	if(!fs::exists(trainfile))
				UTILS::_exit("Train File not found");
		}
		else
			UTILS::_exit("Program needs a train file or an input folder");
		if(!fs::exists(testfile))
		{
			testfile = "";
			std::cout << "[WNG] Test File not found. Only Cross-Validation on Train File will be performed" << std::endl;
		}
	}
	
	if(output == "")
		UTILS::_exit("Output folder needed [--o]");
	else
	{
		char ch = output.back();
		const std::string sl = "/";
		if(ch != '/')
			output.append(sl);
	
		fs::path d1(fs::current_path() / output);
		std::string stemp;
		if(!fs::exists(d1))
		{
			std::size_t a = 0, b = 0;
			while((a = output.find('/', ++a)) != std::string::npos)
			{
				b = a;
				stemp.assign(output, 0, b);
				fs::path d2(fs::current_path() / stemp);
				if(!fs::exists(d2))
					fs::create_directory(d2);
			}
		}
	}
	
	TS::_DATA<TYPE> data;
	TS::read_data<TYPE>(trainfile, data.train);
	//	std::random_shuffle(data.train.begin(), data.train.end());
	
	if(testfile != "")
		TS::read_data<TYPE>(testfile, data.test);
	
	unsigned int cc;
	for(auto it = data.train.begin(); it != data.train.end(); ++it)
	{
		cc = (*it).c + 1;
		if(param.cp_kfold.size() < cc)
			param.cp_kfold.resize(cc, 0u);
		++param.cp_kfold[--cc];
	}
	
	if(data.train.size() > 300)
		param.k_fold = 10u;
	else
		param.k_fold = 0u;
	unsigned int size = data.train[0].ts.size();
	
	const unsigned int max_sc = (unsigned int)(log(0.125 * size / param.sigma) / log(param.k_sc));
	if(param.n_sc == 0 || param.n_sc > max_sc)
		param.n_sc = max_sc;
	
	unsigned int i, j, k, nip=0;
	
	// TRAIN
	{
		std::vector< std::vector< SIFT::ipoints > > train_ip;
		std::vector<SIFT::st_sift> train_st; // DOG + L
		SIFT::get_ip(data.train, train_ip, train_st, param);
	
		std::vector< std::vector< std::vector<float> > > train_neighborhood;
		for(j=0; j<train_ip.size(); ++j)
		{
			train_neighborhood.push_back(
				SIFT::get_feature_vectors(
					train_st[j].l, train_ip[j],
					param.v_nblocks[0], param.v_szblock[0]
				)
			);
			nip += train_neighborhood[train_neighborhood.size()-1].size();
		}
		
		for(i=0; i<train_neighborhood.size(); ++i)
		{
			std::ofstream ofs;
			std::string str = output + "train_" + std::to_string(i);
			ofs.open(str, std::ofstream::out);
		
			for(j=0; j<train_neighborhood[i].size(); ++j)
			{
				ofs << j % data.train[0].ts.size();
				for(k=0; k<train_neighborhood[i][j].size(); ++k)
					ofs << " " << train_neighborhood[i][j][k];
				ofs << std::endl;
			}
			ofs.close();
		}
	}
	
	// TEST
	{
		std::vector< std::vector< SIFT::ipoints > > test_ip;
		std::vector<SIFT::st_sift> test_st; // DOG + L
		SIFT::get_ip(data.test, test_ip, test_st, param);
	
		std::vector< std::vector< std::vector<float> > > test_neighborhood;
		for(j=0; j<test_ip.size(); ++j)
		{
			test_neighborhood.push_back(
				SIFT::get_feature_vectors(
					test_st[j].l, test_ip[j],
					param.v_nblocks[0], param.v_szblock[0]
				)
			);
		}
		
		for(i=0; i<test_neighborhood.size(); ++i)
		{
			std::ofstream ofs;
			std::string str = output + "test_" + std::to_string(i);
			ofs.open(str, std::ofstream::out);
			for(j=0; j<test_neighborhood[i].size(); ++j)
			{
				ofs << j % data.train[0].ts.size();
				for(k=0; k<test_neighborhood[i][j].size(); ++k)
					ofs << " " << test_neighborhood[i][j][k];
				ofs << std::endl;
			}
			ofs.close();
		}
	}
	
	return 1;
}
