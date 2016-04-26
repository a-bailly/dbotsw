/*!
 \file param.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef PARAM_HPP
#define PARAM_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.hpp"

#define TYPE float

/*!
 \brief Structure containing parameters for BoTSW
 \param k_sc Ratio between two consecutive scales
 \param sigma Gaussian filter parameter
 \param threshold Threshold
 \param n_sc Number of scales
 \param v_nblocks Vector containing possible number of blocks
 \param v_szblock Vector containing possible size of block
 \param v_k Vector containing possible \f$k\f$ for \f$k\f$means
 \param v_c_svm Vector containing possible C for linear SVM
 \param dense Dense extraction
 \param norm_bow_ssr Normalisation SSR on BoW
 \param norm_bow Normalisation L2 on BoW
 \param norm_bow_idf Normalisation IDF on BoW
 \param norm_feature_ssr Normalisation SSR on Feature Vectors
 \param k_fold K-fold Cross-Validation
 \param cp_kfold Number of time series for each class
 \param nb_class Number of class
*/
typedef struct
{
	/*!
	 \brief Ratio between two consecutive scales
	 \details Default Value 2**(1./3)
	*/
	double k_sc;
	/*!
	 \brief Gaussian filter parameter
	 \details Default Value 1.6
	*/
	double sigma;
	/*!
	 \brief Threshold for the validation of interest points
	 \details Default Value 0.0
	*/
	double threshold;

	/*!
	 \brief Number of scales
	 \details Default Value 0u
	*/
	unsigned int n_sc;

	/*!
	 \brief Vector containing possible size of block
	 \details Default Value 2 4 6 8
	*/
	std::vector<unsigned int> v_szblock;
	/*!
	 \brief Vector containing possible number of blocks
	 \details Default Value 4 8 12 16 20
	*/
	std::vector<unsigned int> v_nblocks;
	/*!
	 \brief Vector containing possible \f$k\f$ for \f$k\f$means
	 \details Default Value 32 64 128 256 512 1024
	*/
	std::vector<unsigned int> v_k;
	/*!
	 \brief Vector containing possible C for linear SVM
	 \details Default Value 1 10 100
	*/
	std::vector<double> v_c_svm;
 
	/*!
	 \brief Dense extraction
	 \details Default Value false
	*/
	unsigned int dense;
	/*!
	 \brief Normalisation L2 on BoW
	 \details Default Value false
	*/
	bool norm_bow;
	/*!
	 \brief Normalisation SSR on BoW
	 \details Default Value false
	*/
	bool norm_bow_ssr;
	/*!
	 \brief Normalisation IDF on BoW
	 \details Default Value false
	*/
	bool norm_bow_idf;

	/*!
	 \brief K-fold Cross-Validation
	 \details If 0 computes LOO-CV, else computes K-fold CV (if number of time series per class > 8).
	 Default Value 1u
	*/
	unsigned int k_fold;
	/*!
	 \brief Memorize the number of time series for each class
	 \details Default Value NULL
	*/
	std::vector<unsigned int> cp_kfold;

	/*!
	 \brief Computed by the program, memorize the number of class in the dataset
	*/
	unsigned int nb_class;
	
} PARAM;


/*!
 \brief Set parameters to their default value
 \param param Parameters to set
*/
void set_default_parameters(
	PARAM& param
);

/*!
 \brief Read parameter from file
 \param s Input Parameter File
 \param param Structure to update
*/
void read_parameters(
	const std::string& s,
	PARAM& param
);

/*!
 \brief Write/Print parameter from file
 \param s Output Stream
 \param param Parameter to write
*/
void print_parameters(
	std::ostream& os,
	const PARAM& param
);
	
#endif // PARAM_HPP
