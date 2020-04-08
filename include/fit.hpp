#ifndef FIT_HPP
#define FIT_HPP

#include <math.h>
#include <iostream>
#include <random>
#include <math.h>
#include <exception>

#include "defs.hpp"

// Define the function type to pass it as an argument
typedef double (*func)( std::vector<double>, double );

double chi_square( func, std::vector<double>, Matrix );
std::vector<double> 
    fitNM( func, Matrix, std::vector<double>, std::vector<double>,
        const unsigned = 1e8 );

void order_simplex( std::vector<double>&, std::vector<double>& );
std::vector<double> get_centroid( std::vector<double> );

#endif
