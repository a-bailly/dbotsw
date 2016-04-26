/*!
 \file norm.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef NORM_HPP
#define NORM_HPP

#include <vector>
#include <cmath>
#include <numeric>

/*!
 \namespace NORM
*/
namespace NORM
{
	/*!
	 \brief SSR Normalization
	 \param[in,out] histo Set of vectors to normalize
	 \details Sign Squared Root Normalization
	*/
	void norm_bow_ssr(
		std::vector< std::vector<double> > & histo
	);

	/*!
	 \brief IDF Normalization (Train Set)
	 \param[in,out] histo Set of vectors to normalize
	 \param[out] idf_nb_apparition Parameters for normalization
	 \details Inverse Document Frequency Normalization
	*/
	void norm_bow_idf_train(
		std::vector< std::vector<double> > & histo,
		std::vector<unsigned int> & idf_nb_apparition
	);

	/*!
	 \brief IDF Normalization (Test Set)
	 \param[in,out] histo Set of vectors to normalize
	 \param[in] idf_nb_apparition Parameters for normalization
	 \param[in] sz_train Parameter for normalization
	 \details Inverse Document Frequency Normalization
	 \remark Always call after norm_bow_idf_train
	*/
	void norm_bow_idf_test(
		std::vector< std::vector<double> > & histo,
		std::vector<unsigned int> & idf_nb_apparition,
		const unsigned int & sz_train
	);

	/*!
	 \brief L2 Normalization
	 \param[in,out] histo Set of vectors to normalize
	*/
	void norm_bow(
		std::vector< std::vector<double> > & histo
	);

	/*!
	 \brief L2 Norm
	 \param[in] u Vector
	 \return L2 Norm of Vector u
	*/
	template <typename T>
	double l2_norm(
		const std::vector<T>& u
	);

}

#include "../src/norm.tpp"

#endif // NORM_HPP
