/*!
 \file ts.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef TS_HPP
#define TS_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <climits>
#include <algorithm>

#include "utils.hpp"
#include "param.hpp"

/*!
 \namespace TS
*/
namespace TS
{
	/*!
	 \brief Structure containing a time series and its associated class
	 \param ts Time Series
	 \param c Class
	*/
	template<typename T>
	struct time_series_class
	{
		std::vector<T> ts;
		unsigned int c;
	};

	/*!
	 \brief Structure containing a full dataset
	 \param train Train Set
	 \param test Test Set
	 \details Each set correspond to a set of time_series_class
	*/
	template<typename T>
	struct _DATA {
		std::vector< time_series_class<T> > train;
		std::vector< time_series_class<T> > test;
	};

	/*!
	 \brief Read Data
	 \param file Name of Input Data File
	 \param vec Vector of time_series_class to memorize the data
	 \details Example of data : Train Set
	 \details Input file should ideally be a text file. Each line is composed by (1st element) the class, and followed by the time series (w/ a space between each element).
	*/
	template<typename T>
	void read_data(
		const std::string& file,
		std::vector< time_series_class<T> >& vec
	);

}

/*!
 \brief Output Stream for dataset
 \param os Output Stream
 \param d Data (Ex: Train Set)
*/
template<typename T>
std::ostream& operator << (
	std::ostream& os,
	const std::vector< TS::time_series_class<T> >& d
);

#include "../src/ts.tpp"

#endif // TS_HPP
