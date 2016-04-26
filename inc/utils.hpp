/*!
 \file utils.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

/*!
 \namespace UTILS
*/
namespace UTILS
{
	/*!
	 \brief Exit Function
	*/
	void _exit();

	/*!
	 \brief Exit Function
	 \param s String to print before exit
	*/
	void _exit(
		std::string s
	);

	/*!
	 \brief Output Stream for vectors
	 \param os Output Stream
	 \param v Vector to print
	*/
	template<typename T>
	std::ostream& operator << (
		std::ostream& os,
		const std::vector<T>& v
	);

	/*!
	 \brief Output Stream for matrix
	 \param os Output Stream
	 \param m Matrixto print
	*/
	template<typename T>
	std::ostream& operator << (
		std::ostream& os,
		const std::vector< std::vector<T> >& m
	);

	/*!
	 \brief Conversion from string to int
	 \param text String to convert
	 \return Int equivalent of text
	*/
	int stringToInt(
		const std::string &text
	);

	/*!
	 \brief Conversion from string to float
	 \param text String to convert
	 \return Float equivalent of text
	*/
	float stringToFloat(
		const std::string &text
	);
	
	/*!
	 \brief Print time in HH:MM:SS
	 \param t Time
	*/
	void print_time(
		double t
	);
}

#include "../src/utils.tpp"

#endif // UTILS_HPP
