/*!
 \file sift.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef SIFT_HPP
#define SIFT_HPP

#include <vector>
#include <cmath>
#include <algorithm>

#include "param.hpp"
#include "ts.hpp"

/*!
 \namespace SIFT
*/
namespace SIFT
{
	/*!
	 \brief SIFT Structure containing L and DoG
	 \param dog Difference of Gaussian
	 \param l Gaussian Filtered Series
	 \details SIFT Structure containing Gaussian Filtered Series and Difference of Gaussian Matrix
	*/
	typedef struct
	{
		std::vector< std::vector<double> > dog;
		std::vector< std::vector<double> > l;
	} st_sift;

	/*!
	 \brief Interest Points
	 \param tm Time
	 \param sc Scale
	 \details Interest Points defined by the scale and time where detected
	*/
	typedef struct
	{
		int tm;
		int sc;
	} ipoints;

	/*!
	 \brief Gaussian Filter
	 \param sigma Standard deviation
	 \return Gaussian Filter
	 \details Gaussian Filter that return a vector of size \f$4\times\sigma\f$
	*/
	std::vector<double> g(
		const double& sigma
	);

	/*!
	 \brief Gaussian Filter
	 \param sigma Standard deviation
	 \param length Length of the filter
	 \return Gaussian Filter
	 \details Gaussian Filter that return a vector of size \f$length\times\sigma\f$
	*/
	std::vector<double> g(
		const double & sigma,
		const int& length
	);

	/*!
	 \brief Filtered Time Series
	 \param ts Time Series
	 \param sigma Parameter for the filter
	 \return Filtered Time Series
	*/
	template<typename T>
	std::vector<double> l(
		const std::vector<T>& ts,
		const double& sigma
	);

	/*!
	 \brief Filter time series and computes its DoG
	 \param ts Considered Time Series
	 \param sigma Parameter for the filter
	 \param k_sc Ratio between two consecutive scales
	 \param n_sc Number of scales
	 \return Filtered Time Series + its DoG
	 \details Computes the filtered time series and its Difference-of-Gaussian Matrix
	*/
	template<typename T>
	SIFT::st_sift difference_of_gaussians(
		const std::vector<T>& ts,
		const double& sigma,
		const double& k_sc,
		const unsigned int& n_sc
	);

	/*!
	 \brief Computes Interest Points
	 \param x Data
	 \param vec_ip Interest Points for each Time Series
	 \param vec_st st_sift for each Time Series
	 \param p Set of parameters of BoTSW
	*/
	template<typename T>
	void get_ip(
		const std::vector< TS::time_series_class<T> >& x,
		std::vector< std::vector<SIFT::ipoints> >& vec_ip,
		std::vector<SIFT::st_sift>& vec_st,
		const PARAM& p
	);

	/*!
	 \brief Get neighboors of a presumed interest point
	 \param dog Difference-of-Gaussian
	 \param tm Time of presumed interest point
	 \param sc Scale of presumed interest point
	 \return List of neighboors
	*/
	std::vector<double> get_neighboors(
		const std::vector< std::vector<double> >& dog,
		const int& tm,
		const int& sc
	);

	/*!
	 \brief Interest Points
	 \param dog Difference-of-Gaussian
	 \param threshold Threshold for the validation of interest points (in general 0.)
	 \return List of Interest Points
	*/
	std::vector< SIFT::ipoints > interest_points(
		const std::vector< std::vector<double> >& dog,
		const double& threshold
	);

	/*!
	 \brief Dense Points
	 \param dog Difference-of-Gaussian
	 \param dense Step between two points
	 \return List of Dense Interest Points
	*/
	std::vector< SIFT::ipoints > dense_points(
		const std::vector< std::vector<double> >& dog,
		const unsigned int& dense
	);

	/*!
	 \brief Get Feature Vectors
	 \param l Filtered Time Series
	 \param vip List of Interest Points
	 \param n_b Number of blocks
	 \param a Size of block
	 \return List of feature vectors
	*/
	std::vector< std::vector<float> > get_feature_vectors(
		const std::vector< std::vector<double> >& l,
		const std::vector< SIFT::ipoints >& vip,
		const unsigned int& n_b,
		const unsigned int& a
	);
	
}

/*!
 \brief Output stream of st_sift
 \param os Output Stream
 \param s SIFT Structure
*/
std::ostream& operator << (
	std::ostream &os,
	const SIFT::st_sift &s
);

/*!
 \brief Output stream of ipoints
 \param os Output Stream
 \param p Interest Point
*/
std::ostream& operator << (
	std::ostream &os,
	const SIFT::ipoints &p
);

#include "../src/sift.tpp"

#endif // SIFT_HPP
