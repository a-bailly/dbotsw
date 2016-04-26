/*!
 \file utils.tpp
 \author adeline.bailly@uhb.fr
*/

#ifndef UTILS_TPP
#define UTILS_TPP

template<typename T>
std::ostream& operator << (
	std::ostream& os,
	const std::vector<T>& v
	)
{
	for(auto it=v.begin(); it!=v.end(); ++it)
		os << (*it) << " ";
	os << std::endl;
	return os;
}


template<typename T>
std::ostream& operator << (
	std::ostream& os,
	const std::vector< std::vector<T> >& m
	)
{
	for(auto it=m.begin(); it!=m.end(); ++it)
	{
		for(auto jt=(*it).begin(); jt!=(*it).end(); ++jt)
			os << (*jt) << " ";
		os << std::endl;
	}
	return os;
}

#endif // UTILS_TPP
