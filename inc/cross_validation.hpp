/*!
 \file cross_validation.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef CV_HPP
#define CV_HPP

#include <vector>
#include <random>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

#include "libsvm/svm.h"

#include "param.hpp"
#include "euclidean.hpp"
#include "norm.hpp"
#include "sift.hpp"
#include "ts.hpp"
#include "utils.hpp"


/*!
 \namespace CROSS_V
*/
namespace CROSS_V
{
	/*!
	 \struct cv_param cross_validation.hpp
	 \brief Structure containing variable parameters for Cross Validation
	 \param n_blocks Number of blocks for feature vectors
	 \param sz_block Size of block for feature vectors
	 \param k_means Parameter for k-means method
	 \param c_svm Parameter C from linear SVM
	*/
	struct cv_param {
		unsigned int n_blocks;
		unsigned int sz_block;
		unsigned int k_means;
		double c_svm;
	};
	
	/*!
	 \brief Updates vector of best cross-validation parameters according to accuracy
	 \param acc Accuracy Vector
	 \param best Best accuracy so far
	 \param best_param Vector of current best parameters
	 \param n_blocks Current number of blocks
	 \param sz_block Current size of block
	 \param k_means Current k of k-means method
	 \param v_c_svm Current C of SVM
	*/
	void update_best_param(
		const std::vector<double> & acc,
		double & best,
		std::vector<cv_param> & best_param,
		unsigned int n_blocks,
		unsigned int sz_block,
		unsigned int k_means,
		std::vector<double> v_c_svm
	);
	
	/*!
	 \brief Computes Leave-One-Out Cross-Validation with SVM classifier
	 \param best_param Vector containing best cross-validation parameters
	 \param train Train Set
	 \param param Non-Variable parameters
	*/
	template<typename T>
	bool loocv_svm(
		std::vector<cv_param>& best_param,
		const std::vector< TS::time_series_class<T> >& train,
		const PARAM& param
	);

	/*!
	 \brief Computes K-fold Cross-Validation with SVM classifier
	 \param best_param Vector containing best cross-validation parameters for accuracy
	 \param train Train Set
	 \param param Non-Variable parameters
	*/
	template<typename T>
	bool kfold_cv_svm(
		std::vector<cv_param>& best_param,
		const std::vector< TS::time_series_class<T> >& train,
		const PARAM& param
	);

	/*!
	 \brief Creation of folds for k-fold Cross-Validation
	 \param param Set of parameters
	 \param train Train Set
	 \param train_histo Histogramme of train set
	 \param x_tp 
	 \param k Number of wanted folds
	 \param kfold_classes
	*/
	template<typename T>
	void create_folds(
		const PARAM& param,
		const std::vector< TS::time_series_class<T> >& train,
		const std::vector< std::vector<double> >& train_histo,
		std::vector<std::vector< svm_node*> >& x_tp,
		const unsigned int k,
		std::vector< std::vector<unsigned int> >& kfold_classes
	);

	/*!
	 \brief Output stream of cv_param
	 \param os Output Stream
	 \param bp Set of Cross-Validation parameters to print
	*/
	void print_cv_svm(
		std::ostream& os,
		const std::vector<cv_param>& bp
	);

	/*!
	 \brief Output stream of cv_param
	 \param s Name of Input File
	 \param bp Set of Cross-Validation parameters to read
	*/
	bool read_cv_svm(
		std::string& s,
		std::vector<cv_param>& bp
	);
}

/*!
 \brief Output stream of cv_param
 \param os Output Stream
 \param cv_param Cross-Validation Parameters to print
*/
std::ostream& operator << (
	std::ostream &os,
	const CROSS_V::cv_param &p
);

#include "../src/cross_validation.tpp"

#endif // CV_HPP
