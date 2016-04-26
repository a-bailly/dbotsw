/*!
 \file botsw.tpp
 \author adeline.bailly@uhb.fr
*/


template<typename T>
std::vector<unsigned int> BOTSW::compute_clf_svm(
	const TS::_DATA<T>& data,
	const PARAM& param,
	const std::string& output
	)
{
	std::ofstream ofcv;
	std::string str;

	std::vector<CROSS_V::cv_param> best_param;

	if(output != "")
	{
		if(param.k_fold)
			str = output + "kfold_cv";
		else
			str = output + "loo_cv";

		if(boost::filesystem::exists(str) && !boost::filesystem::is_empty(str) && CROSS_V::read_cv_svm(str, best_param))
			;
		else
			ofcv.open(str, std::ofstream::out);
	}

	if(best_param.size() == 0)
	{
		if(param.k_fold)
			kfold_cv_svm(best_param, data.train, param);
		else
			loocv_svm(best_param, data.train, param);
	}
	else
	{
		std::cout << "CV parameters extracted from file" << std::endl;

		if(param.k_fold)
			str = output + "e_rate_kf";
		else
			str = output + "e_rate";
	}

	if(ofcv.is_open())
	{
		CROSS_V::print_cv_svm(ofcv, best_param);
		ofcv.close();
	}

	// Train
	std::vector< std::vector< SIFT::ipoints > > train_ip;
	std::vector< std::vector< SIFT::ipoints > > test_ip;
	
	std::vector<SIFT::st_sift> train_st;
	std::vector<SIFT::st_sift> test_st;

	SIFT::get_ip(data.train, train_ip, train_st, param);
	SIFT::get_ip(data.test, test_ip, test_st, param);

	std::vector< std::vector<unsigned int> > attributed_class;
	std::vector<unsigned int> final_attributed_class;

	attributed_class.resize(best_param.size());
	final_attributed_class.resize(data.test.size());

	unsigned int l;

	for(l=0; l<best_param.size(); ++l)
		attributed_class[l] = BOTSW::compute_clf_svm_class(data, best_param[l], train_ip, train_st, test_ip, test_st, param);

	double acc = 0.;

	if(best_param.size() == 1)
	{
		for(l=0; l<data.test.size(); ++l)
		{
			if(attributed_class[0][l] == data.test[l].c)
				++acc;
		}
	}
	else
	{
		std::vector<unsigned int> v;
		int nb_class = param.cp_kfold.size();

		unsigned int i;
		
		for(l=0; l<data.test.size(); ++l)
		{
			v.assign(nb_class, 0u);
			for(i=0; i<best_param.size(); ++i)
				++v[attributed_class[i][l]];

			final_attributed_class[l] = std::distance(v.begin(), std::max_element(v.begin(), v.end()));

			if(data.test[l].c == final_attributed_class[l])
				++acc;
		}
	}

	acc /= data.test.size();

	if(output != "")
	{
		std::ofstream ofs;
		if(param.k_fold)
			str = output + "e_rate_kf";
		else
			str = output + "e_rate";

		ofs.open(str, std::ofstream::out);
		ofs << 1.-acc << std::endl;
		ofs.close();
	}

	std::cout << "ER " << 1.-acc << " [" << output << "]" << std::endl;

	if(best_param.size() == 1)
		return attributed_class[0];
	else
		return final_attributed_class;
}


template<typename T>
std::vector<unsigned int> BOTSW::compute_clf_svm_class(
	const TS::_DATA<T>& data,
	const CROSS_V::cv_param& best_param,
	const std::vector< std::vector< SIFT::ipoints > >& train_ip,
	const std::vector<SIFT::st_sift>& train_st,
	const std::vector< std::vector< SIFT::ipoints > >& test_ip,
	const std::vector<SIFT::st_sift>& test_st,
	const PARAM& p
	)
{
	unsigned int nip = 0;
	unsigned int i, j, k;

	std::vector< std::vector< std::vector<float> > > train_neighborhood;
	
	for(j=0; j<train_ip.size(); ++j)
	{
		train_neighborhood.push_back(
			SIFT::get_feature_vectors(
				train_st[j].l, train_ip[j],
				best_param.n_blocks, best_param.sz_block
			)
		);
		nip += train_neighborhood[train_neighborhood.size()-1].size();
	}

	cv::Mat M(nip, 2*best_param.n_blocks, cv::DataType<float>::type);

	int m = 0;
	
	for(i=0; i<train_neighborhood.size(); i++)
	{
		for(k=0; k<train_neighborhood[i].size(); k++)
		{	for(j=0; j<train_neighborhood[i][k].size(); j++)
		 		M.at<float>(m,j) = train_neighborhood[i][k][j];
		 	++m;
		}
	}
	
	cv::Mat bestLabels, centers;
	cv::TermCriteria cvtermC(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 10, 0.001);
	const unsigned int n_attemps = 10;
	
	cv::kmeans(M, best_param.k_means, bestLabels, cvtermC, n_attemps, cv::KMEANS_PP_CENTERS, centers);

	std::vector< std::vector<float> > vcenters;

	k = (unsigned int) centers.rows;
	nip = (unsigned int) centers.cols;
	
	vcenters.resize(k);

	for(i=0; i<k; ++i)
	{
		vcenters[i].resize(nip);
	
		for(j=0; j<nip; ++j)
			vcenters[i][j] = centers.at<float>(i,j);
	}

	// Construction Histogram
	std::vector< std::vector<double> > train_histo;
	train_histo.resize(data.train.size());

	for(i=0; i<data.train.size(); ++i)
	{
		train_histo[i].assign(best_param.k_means, 0.);
		
		for(j=0; j<train_neighborhood[i].size(); ++j)
			++(train_histo[i][ED::euclDistance_vec(train_neighborhood[i][j], vcenters)]);
	}

	// Test
	std::vector< std::vector< std::vector<float> > > test_neighborhood;
	
	for(j=0; j<test_ip.size(); ++j)
	{
		test_neighborhood.push_back(
			SIFT::get_feature_vectors(
				test_st[j].l,
				test_ip[j],
				best_param.n_blocks,
				best_param.sz_block
			)
		);
	}

	std::vector< std::vector<double> > test_histo;
	test_histo.resize(data.test.size());

	for(i=0; i<data.test.size(); ++i)
	{
		test_histo[i].assign(best_param.k_means, 0.);
	
		for(j=0; j<test_neighborhood[i].size(); ++j)
			++(test_histo[i][ED::euclDistance_vec(test_neighborhood[i][j], vcenters)]);
	}

	if(p.norm_bow_ssr)
	{
		NORM::norm_bow_ssr(train_histo);
		NORM::norm_bow_ssr(test_histo);
	}
	if(p.norm_bow_idf)
	{
		std::vector<unsigned int> idf_nb_apparition;
		NORM::norm_bow_idf_train(train_histo, idf_nb_apparition);
		NORM::norm_bow_idf_test(test_histo, idf_nb_apparition, data.train.size());
	}
	if(p.norm_bow)
	{
		NORM::norm_bow(train_histo);
		NORM::norm_bow(test_histo);
	}

	struct svm_parameter svm_param;
	struct svm_problem prob;

	svm_param.svm_type = C_SVC;		svm_param.kernel_type = LINEAR;
	svm_param.degree = 3;			svm_param.gamma = 0.5;
	svm_param.coef0 = 0;			svm_param.nu = 0.5;
	svm_param.cache_size = 200;		svm_param.eps = 1e-3;
	svm_param.p = 0.1;				svm_param.shrinking = 1;
	svm_param.probability = 0;		svm_param.nr_weight = 0;
	svm_param.weight_label = NULL;	svm_param.weight = NULL;

	svm_param.C = best_param.c_svm;

	prob.l = train_histo.size();
	svm_node** x =Malloc(svm_node*, prob.l);

	unsigned int sz = train_histo[0].size();
	
	for(int i=0; i<prob.l; ++i)
	{
		struct svm_node *x_space;
		x_space = Malloc(svm_node, sz+1);
		
		for(j=0; j<sz; ++j)
		{
			x_space[j].index = j+1;
			x_space[j].value = train_histo[i][j];
		}
		
		x_space[sz].index = -1;
		x[i] = x_space;
	}

	prob.x = x;
	prob.y = Malloc(double, prob.l);
	
	for(i=0; i<data.train.size(); ++i)
		prob.y[i] = data.train[i].c;

	struct svm_model *model = svm_train(&prob,&svm_param);

	sz = test_histo[0].size();
	struct svm_node* testnode = Malloc(svm_node, sz+1);

	for(j=0; j<sz; ++j)
		testnode[j].index = j+1;
	testnode[sz].index = -1;
	
	std::vector<unsigned int> att_class;
	att_class.resize(data.test.size());

	for(i=0; i<data.test.size(); ++i)
	{
		for(j=0; j<sz; ++j)
			testnode[j].value = test_histo[i][j];

		att_class[i] = (unsigned int) svm_predict(model, testnode);
	}

	return att_class;
}
