/*!
 \file ts.tpp
 \author adeline.bailly@uhb.fr
*/

#ifndef TS_TPP
#define TS_TPP

template<typename T>
void TS::read_data(
	const std::string& file,
	std::vector< time_series_class<T> >& vec
	)
{
	std::ifstream ifile (file, std::ifstream::in);
	
	if(!ifile)
	{
		std::cout << "[ERROR] Can't open data file " << file << std::endl;
		UTILS::_exit();
	}

	std::vector<TYPE> ts;
	time_series_class<TYPE> tscl;
	
	float v;
	unsigned int min_class = UINT_MAX;
	T e;

	std::string line;
	
	while(getline(ifile, line))
	{
		std::istringstream input(line, std::istringstream::in);

		input >> v;

		if(v != -1)
			tscl.c = (unsigned int) v;
		else
			tscl.c = 0u;

		if(tscl.c < min_class)
			min_class = tscl.c;

		while(input >> e)
			ts.push_back(e);

		e = *std::min_element(ts.begin(), ts.end());
		for(auto it = ts.begin(); it != ts.end(); ++it)
			*it -= e;

		e = *std::max_element(ts.begin(), ts.end());
		if ( e != 0)
			for(auto it = ts.begin(); it != ts.end(); ++it)
				*it /= e;

		tscl.ts = ts;

		vec.push_back(tscl);

		ts.clear();
	}

	if(vec[vec.size()-1].ts.size() == 0)
		vec.pop_back();

	if(min_class > 0)
		for(v=0; v<vec.size(); ++v)
			vec[v].c -= min_class;

	ifile.close();
}


template<typename T>
std::ostream& operator << (
	std::ostream& os,
	const std::vector< TS::time_series_class<T> >& d
	)
{
	for(auto it=d.begin(); it!=d.end(); ++it)
	{
		os << (*it).c;
		for(auto jt=(*it).ts.begin(); jt!=(*it).ts.end(); ++jt)
			os << " " << (*jt);
		os << std::endl;
	}
	return os;
}

#endif // TS_TPP
