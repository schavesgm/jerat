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

// Implementation of the chi_square function 
double chi_square( func, std::vector<double>, Matrix );

// Implementation of the Nelder-Mead algorithm
std::vector<double> 
    fitNM( func, Matrix, std::vector<double>, std::vector<double>,
        const unsigned = 1000, const unsigned = 123456 );

// Reorder the simplex and their images according to the images
void order_simplex( std::vector<double>&, std::vector<double>& );

// Calculate the centroid of the simplex
std::vector<double> get_centroid( std::vector<double>, unsigned );

// Select a time window from the Matrix structure starting at unsigned
Matrix select_window( Matrix, unsigned );

// Calculate the average and standard deviation of different fits
std::vector<double> avg_stde( std::vector<double>, unsigned );

#endif
