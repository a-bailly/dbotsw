/*!
 \file botsw.cpp
 \author adeline.bailly@uhb.fr
*/


#include "../inc/botsw.hpp"

namespace fs = boost::filesystem;

int main(int argc, char *argv[])
{

	PARAM param;
	set_default_parameters(param);

	std::string folder = "";		std::string trainfile = "";
	std::string testfile = "";		std::string paramfile = "";
	std::string output = "";

	static struct option long_options[] = {
		{"folder", 1, 0, 'f'},		{"train", 1, 0, 't'},
		{"xp_test", 1, 0, 'x'},		{"paramfile", 1, 0, 'p'},
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

	if(paramfile != "" && fs::exists(paramfile))
		read_parameters(paramfile, param);
	else if(!fs::exists(paramfile))
	{
		std::cout << "[WARNING] parameters file not found - Default parameters used" << std::endl;
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
		testfile  = folder + '/' + testfile + "_TEST";

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
	
	std::random_shuffle(data.train.begin(), data.train.end());

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

	time_t t;
	time_t tb;
	time(&t);

	std::ofstream ofs;
	
	if (testfile == "")
	{
		std::vector<CROSS_V::cv_param> best_param;
		
		std::string save_cv;

		if(param.k_fold)
			CROSS_V::kfold_cv_svm(best_param, data.train, param);
		else
			CROSS_V::loocv_svm(best_param, data.train, param);

		if(param.k_fold)
			save_cv = output + "kfold_cv";
		else
			save_cv = output + "loo_cv";

		ofs.open(save_cv, std::ofstream::out);

		if(ofs.is_open())
		{
			CROSS_V::print_cv_svm(ofs, best_param);
			ofs.close();
		}
	}
	else
	{	
		std::vector<unsigned int> v_att;
		v_att = BOTSW::compute_clf_svm(data, param, output);
		
		std::string str = output + "details";
		
		ofs.open(str, std::ofstream::out);
		for(unsigned int i=0u; i<v_att.size(); ++i)
			ofs << v_att[i] << std::endl;
		ofs.close();
	}

	time(&tb);
	UTILS::print_time(tb-t);

	return 1;
}
