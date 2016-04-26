/*!
 \file norm.tpp
 \author adeline.bailly@uhb.fr
*/

#ifndef NORM_TPP
#define NORM_TPP

template <typename T>
double NORM::l2_norm(const std::vector<T>& u)
{
    double n = 0.;
    for(int x=0; x<u.size(); ++x)
    	n += (double) (u[x] * u[x]);

    return sqrt(n);
}

#endif // NORM_TPP
