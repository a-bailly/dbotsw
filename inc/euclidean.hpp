/*!
 \file euclidean.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef ED_HPP
#define ED_HPP

#include <cfloat>
#include <cmath>
#include <vector>

#include "utils.hpp"

/*!
 \namespace ED
*/
namespace ED
{
	/*!
	 \brief Computes the Euclidean distance between two vectors
	 \param v First vector
	 \param w Second vector
	 \return Euclidean Distance
	*/
	template<typename T>
	double euclDistance(
		const std::vector<T>& v,
		const std::vector<T>& w
	);
	
	/*!
	 \brief Computes the Euclidean distance between a vector and a set of vectors
	 \param v Vector
	 \param m Set of vectors
	 \return Index of the Nearest-Neighbor according to the Euclidean Distance
	*/
	template<typename T>
	unsigned int euclDistance_vec(
		const std::vector<T>& v,
		const std::vector< std::vector<T> >& m
	);

}

#include "../src/euclidean.tpp"

#endif // ED_HPP
