/*!
 \file utils.cpp
 \author adeline.bailly@uhb.fr
*/

#include "../inc/utils.hpp"

void UTILS::_exit()
{
	std::cerr << std::endl << "---- STOP ----" << std::endl;
	exit(1);
}


void UTILS::_exit(
	std::string s
	)
{
	std::cerr << s << std::endl << std::endl << "---- STOP ----" << std::endl;
	exit(1);
}


int UTILS::stringToInt(
	const std::string &text
	)
{
	std::stringstream ss(text);
	int result;
	return ss >> result ? result : 0;
}


float UTILS::stringToFloat(
	const std::string &text
	)
{
	std::stringstream ss(text);
	float result;
	return ss >> result ? result : 0.;
}

void UTILS::print_time(double t)
{
	std::cout << "Time : ";
	unsigned long int tp = (unsigned long int) t;

	if(t > 3600)
	{
		tp = t/3600;
		std::cout << tp << " hour(s), ";
		t -= (tp*3600);
	}

	if(t > 60)
	{
		tp = t/60;
		std::cout << tp << " minute(s), ";
		t -= (tp*60);
	}

	std::cout << t << " second(s)." << std::endl;
}
