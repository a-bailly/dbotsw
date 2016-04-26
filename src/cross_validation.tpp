/*!
 \file cross_validation.tpp
 \author adeline.bailly@uhb.fr
*/

#ifndef CROSS_V_TPP
#define CROSS_V_TPP

template<typename T>
bool CROSS_V::loocv_svm(
	std::vector<cv_param>& best_param, 
	const std::vector< TS::time_series_class<T> >& train, 
	const PARAM& p
	)
{
	struct svm_parameter svm_param;

	svm_param.svm_type = C_SVC;		svm_param.kernel_type = LINEAR;
	svm_param.degree = 3;			svm_param.gamma = 0.5;
	svm_param.coef0 = 0;			svm_param.nu = 0.5;
	svm_param.cache_size = 200;		svm_param.eps = 1e-3;
	svm_param.p = 0.1;				svm_param.shrinking = 1;
	svm_param.probability = 0;		svm_param.nr_weight = 0;
	svm_param.weight_label = NULL;	svm_param.weight = NULL;

	struct svm_problem prob;
	
	prob.l = train.size()-1;
	prob.y = Malloc(double, prob.l);

	svm_node** x = Malloc(svm_node*, prob.l+1);
	svm_node** x_tp = Malloc(svm_node*, prob.l);

	std::vector<SIFT::st_sift> train_st;		// DOG + L
	std::vector< std::vector< SIFT::ipoints > > train_ip;

	SIFT::get_ip(train, train_ip, train_st, p);

	unsigned int i, j, k, n=0u, sz;
	unsigned int predict;
	int m;
	double best = -DBL_MAX;

	std::vector<double> acc;
	acc.resize(p.v_c_svm.size());

	cv::TermCriteria cvtermC(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 10, 0.1);
	
	std::vector< std::vector<double> > train_histo;
	train_histo.resize(train.size());

	for(auto it=p.v_nblocks.begin(); it!=p.v_nblocks.end(); ++it)
	{
		for(auto jt=p.v_szblock.begin(); jt!=p.v_szblock.end(); ++jt)
		{
			std::vector< std::vector< std::vector<float> > > train_neighborhood;
		
			n = 0u;

			for(i=0u; i<train_ip.size(); ++i)
			{
				train_neighborhood.push_back(SIFT::get_feature_vectors(train_st[i].l, train_ip[i], (*it), (*jt)));
				n += train_neighborhood[train_neighborhood.size()-1].size();
			}

			cv::Mat M(n, 2*(*it), cv::DataType<float>::type);
			
			m = 0;
			
			for(i=0u; i<train_neighborhood.size(); ++i)
				for(k=0; k<train_neighborhood[i].size(); ++k)
				{
					for(j=0u; j<train_neighborhood[i][k].size(); ++j)
				 		M.at<float>(m,j) = train_neighborhood[i][k][j];
				 	++m;
				}

			for(auto kt=p.v_k.begin(); kt!=p.v_k.end(); ++kt)
			{
				if((*kt) < n)
				{
					cv::Mat bestLabels, centers;
					cv::kmeans(M, (*kt), bestLabels, cvtermC, 2, cv::KMEANS_PP_CENTERS, centers);

					std::vector< std::vector<float> > vcenters;
					k = (unsigned int) centers.rows;
					sz = (unsigned int) centers.cols;
					vcenters.resize(k);

					for(i=0; i<k; ++i)
					{	vcenters[i].resize(sz);
						for(j=0; j<sz; ++j)
							vcenters[i][j] = centers.at<float>(i,j);
					}

					for(i=0; i<train.size(); ++i)
					{
						train_histo[i].assign((*kt), 0.);
						for(j=0; j<train_neighborhood[i].size(); ++j)
							++(train_histo[i][ED::euclDistance_vec(train_neighborhood[i][j], vcenters)]);
					}

					// IF NORM BOW
					if(p.norm_bow_ssr)
						NORM::norm_bow_ssr(train_histo);
					if(p.norm_bow_idf)
					{
						std::vector<unsigned int> idf_nb_apparition;
						NORM::norm_bow_idf_train(train_histo, idf_nb_apparition);
					}
					if(p.norm_bow)
						NORM::norm_bow(train_histo);

					for(i=0u; i<=prob.l; ++i)
					{
						struct svm_node *x_space;
						x_space = Malloc(svm_node, (*kt)+1);
						for(j=0; j<(*kt); ++j)
						{	x_space[j].index = j+1;
							x_space[j].value = train_histo[i][j];
						}
						x_space[(*kt)].index = -1;
						x[i] = x_space;
					}

					for(i=1; i<train.size(); ++i)
					{	x_tp[i-1] = x[i];
						prob.y[i-1] = train[i].c;
					}

					sz = train_histo[0].size();
					struct svm_node* testnode = Malloc(svm_node, sz+1);
					
					for(j=0; j<sz; ++j)
						testnode[j].index = j+1;
					testnode[sz].index = -1;

					for(k=0; k<acc.size(); ++k)
						acc[k] = 0.;

					for(i=0; i<train_histo.size(); ++i)
					{
						prob.x = x_tp;

						for(j=0; j<sz; ++j)
							testnode[j].value = train_histo[i][j];

						k = 0;
						
						for(auto lt=p.v_c_svm.begin(); lt!=p.v_c_svm.end(); ++lt)
						{
							svm_param.C = (*lt);
							struct svm_model *model = svm_train(&prob,&svm_param);
	
							predict = svm_predict(model, testnode);
							if(predict == train[i].c)
								++(acc[k]);
							++k;
						}

						if(i < train_histo.size()-1)
						{	prob.y[i] = train[i].c;
							x_tp[i] = x[i];
						}
					}

					update_best_param(acc, best, best_param, (*it), (*jt), (*kt), p.v_c_svm);
				}
			}
		}
	}

	svm_destroy_param(&svm_param);
	free(prob.y);
	free(prob.x);

	return true;
}


template<typename T>
bool CROSS_V::kfold_cv_svm(
	std::vector<cv_param>& best_param, 
	const std::vector< TS::time_series_class<T> >& train, 
	const PARAM& p
	)
{
	struct svm_parameter svm_param;

	svm_param.svm_type = C_SVC;		svm_param.kernel_type = LINEAR;
	svm_param.degree = 3;			svm_param.gamma = 0.5;
	svm_param.coef0 = 0;			svm_param.nu = 0.5;
	svm_param.cache_size = 200;		svm_param.eps = 1e-3;
	svm_param.p = 0.1;				svm_param.shrinking = 1;
	svm_param.probability = 0;		svm_param.nr_weight = 0;
	svm_param.weight_label = NULL;	svm_param.weight = NULL;

	struct svm_problem prob;

	std::vector< std::vector<svm_node*> > x_tp;
	x_tp.resize(p.k_fold);

	std::vector< std::vector<unsigned int> > kfold_classes;
	kfold_classes.resize(p.k_fold);

	std::vector<SIFT::st_sift> train_st;		// DOG + L
	std::vector< std::vector< SIFT::ipoints > > train_ip;

	SIFT::get_ip(train, train_ip, train_st, p);

	unsigned int i, j, k, n=0u;
	int m, n_ip=0;
	double best = -DBL_MAX;

	std::vector<double> acc;
	acc.resize(p.v_c_svm.size());

	for(i=0; i<train_ip.size(); ++i)
		n_ip += train_ip[i].size();

	cv::TermCriteria cvtermC(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 10, 0.1);
	
	std::vector< std::vector<double> > train_histo;
	train_histo.resize(train.size());

	for(auto it=p.v_nblocks.begin(); it!=p.v_nblocks.end(); ++it)
	{
		for(auto jt=p.v_szblock.begin(); jt!=p.v_szblock.end(); ++jt)
		{
			std::vector< std::vector< std::vector<float> > > train_neighborhood;
			
			n = 0;

			for(i=0; i<train_ip.size(); ++i)
			{	train_neighborhood.push_back(SIFT::get_feature_vectors(train_st[i].l, train_ip[i], (*it), (*jt)));
				n += train_neighborhood[train_neighborhood.size()-1].size();
			}

			cv::Mat M(n, 2*(*it), cv::DataType<float>::type);
			
			m = 0;
			
			for(i=0; i<train_neighborhood.size(); ++i)
				for(k=0u; k<train_neighborhood[i].size(); ++k)
				{
					for(j=0u; j<train_neighborhood[i][k].size(); ++j)
				 		M.at<float>(m,j) = train_neighborhood[i][k][j];
				 	++m;
				}

			for(auto kt=p.v_k.begin(); kt!=p.v_k.end(); ++kt)
			{
				if((*kt) < n)
				{
					cv::Mat bestLabels, centers;
					cv::kmeans(M, (*kt), bestLabels, cvtermC, 2, cv::KMEANS_PP_CENTERS, centers);

					std::vector< std::vector<float> > vcenters;
					k = (unsigned int) centers.rows;
					m = (unsigned int) centers.cols;
					vcenters.resize(k);

					for(i=0; i<k; ++i)
					{	vcenters[i].resize(m);
						for(j=0; j<m; ++j)
							vcenters[i][j] = centers.at<float>(i,j);
					}

					for(i=0u; i<train.size(); ++i)
					{	train_histo[i].assign((*kt), 0.);
						for(j=0u; j<train_neighborhood[i].size(); ++j)
							++(train_histo[i][ED::euclDistance_vec(train_neighborhood[i][j], vcenters)]);
					}

					// IF NORM BOW
					if(p.norm_bow_ssr)
						NORM::norm_bow_ssr(train_histo);
					if(p.norm_bow_idf)
					{
						std::vector<unsigned int> idf_nb_apparition;
						NORM::norm_bow_idf_train(train_histo, idf_nb_apparition);
					}
					if(p.norm_bow)
						NORM::norm_bow(train_histo);

					create_folds(p, train, train_histo, x_tp, (*kt), kfold_classes);

					for(k=0; k<acc.size(); ++k)
						acc[k] = 0.;

					for(i=0; i<p.k_fold; ++i)
					{
						m = train.size() - x_tp[i].size();
						
						svm_node** xx = Malloc(svm_node*, m);
						
						prob.l = m;
						prob.y = Malloc(double, prob.l);

						m = -1;
						
						for(j=0; j<x_tp.size(); ++j)
						{
							if(j == i)
								continue;
							for(k=0; k<x_tp[j].size(); ++k)
							{
								xx[++m] = x_tp[j][k];
								prob.y[m] = kfold_classes[j][k];
							}
						}

						prob.x = xx;

						k = 0;
						
						for(auto lt=p.v_c_svm.begin(); lt!=p.v_c_svm.end(); ++lt)
						{
							svm_param.C = (*lt);
							struct svm_model *model = svm_train(&prob,&svm_param);
	
							for(m=0; m< (int)x_tp[i].size(); ++m)
							{
								if(svm_predict(model, x_tp[i][m]) == kfold_classes[i][m])//train[i].c)
									++(acc[k]);
							}
							++k;
						}
					}

					update_best_param(acc, best, best_param, (*it), (*jt), (*kt), p.v_c_svm);
				}
			}
		}
	}

	svm_destroy_param(&svm_param);
	free(prob.y);
	free(prob.x);

	return true;
}


template<typename T>
void CROSS_V::create_folds(
	const PARAM& param,
	const std::vector< TS::time_series_class<T> >& train,
	const std::vector< std::vector<double> >& train_histo,
	std::vector<std::vector< svm_node*> >& x_tp,
	const unsigned int k,
	std::vector< std::vector<unsigned int> >& kfold_classes
	)
{
	unsigned int i, j;

	for(i=0; i<x_tp.size(); ++i)
		x_tp[i].clear();
	x_tp.clear();
	x_tp.resize(param.k_fold);

	for(i=0; i<kfold_classes.size(); ++i)
		kfold_classes[i].clear();
	kfold_classes.clear();
	kfold_classes.resize(param.k_fold);

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, param.k_fold - 1);
    
    unsigned int irandom, imin;
     
	int nb_class = param.cp_kfold.size();

	std::vector< std::vector<int> > kfold_attribution;
	kfold_attribution.resize(nb_class);
	
	for(i=0; i<kfold_attribution.size(); ++i)
		kfold_attribution[i].assign(param.k_fold, 0);

	for(i=0; i<train.size(); ++i)
	{
		struct svm_node *x_space;
		x_space = Malloc(svm_node, k+1);
		for(j=0; j<k; ++j)
		{	x_space[j].index = j+1;
			x_space[j].value = train_histo[i][j];
		}
		x_space[k].index = -1;

		irandom = dis(gen);
		imin = *std::min_element(kfold_attribution[train[i].c].begin(), kfold_attribution[train[i].c].end());

		while(kfold_attribution[train[i].c][irandom] != imin)
		{	++irandom;
			if(irandom >= param.k_fold)
				irandom = 0;
		}

		++kfold_attribution[train[i].c][irandom];
		x_tp[irandom].push_back(x_space);
		kfold_classes[irandom].push_back(train[i].c);
	}
}

#endif // CROSS_V_TPP
