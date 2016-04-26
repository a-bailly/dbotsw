/*!
 \file botsw.hpp
 \author adeline.bailly@uhb.fr
*/

#ifndef BOTSW_HPP
#define BOTSW_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include <getopt.h>

#define BOOST_TEST_DYN_LINK
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>

#include "param.hpp"
#include "utils.hpp"
#include "ts.hpp"
#include "cross_validation.hpp"
#include "sift.hpp"
#include "norm.hpp"

/*!
 \namespace BOTSW
*/
namespace BOTSW
{
	/*!
	 \brief Bla TODO
	 \param data
	 \param param
	 \param output
	*/
	template<typename T>
	std::vector<unsigned int> compute_clf_svm(
		const TS::_DATA<T>& data,
		const PARAM& param,
		const std::string& output
	);

	/*!
	 \brief Bla TODO
	 \param data
	 \param best_param
	 \param train_ip
	 \param train_st
	 \param test_ip
	 \param test_st
	 \param p
	 \param idf_nb_apparition
	*/
	template<typename T>
	std::vector<unsigned int> compute_clf_svm_class(
		const TS::_DATA<T>& data,
		const CROSS_V::cv_param& best_param,
		const std::vector< std::vector< SIFT::ipoints > >& train_ip,
		const std::vector<SIFT::st_sift>& train_st,
		const std::vector< std::vector< SIFT::ipoints > >& test_ip,
		const std::vector<SIFT::st_sift>& test_st,
		const PARAM& p
	);
}

#include "../src/botsw.tpp"

#endif // BOTSW_HPP
