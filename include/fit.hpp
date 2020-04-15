#ifndef FIT_HPP
#define FIT_HPP

#include <math.h>
#include <iostream>
#include <random>
#include <math.h>
#include <exception>

#include "defs.hpp"
#include "corr.hpp"

// Define the function type to pass it as an argument
typedef double (*func)( std::vector<double>, double, \
        std::vector<double> );

// Definition of the fit function used in the calculation
double f( std::vector<double> params, double x, std::vector<double> );

// Implementation of the chi_square function 
double chi_square( func, std::vector<double>, Matrix, 
        std::vector<double> );

// Implementation of the Nelder-Mead algorithm
std::vector<double> 
    fitNM( func, Matrix, std::vector<double>, std::vector<double>,
        std::vector<double>, const unsigned = 1000, 
        const unsigned = 123456 );

// Reorder the simplex and their images according to the images
void order_simplex( std::vector<double>&, std::vector<double>& );

// Calculate the centroid of the simplex
std::vector<double> get_centroid( std::vector<double>, unsigned );

// Fit a correlator object in a hierarchy way
std::vector<std::vector<double>> fit_hierarchy( Correlator&,
        std::vector<std::vector<unsigned>>, 
        std::vector<unsigned>, 
        std::vector<std::vector<double>>, 
        std::vector<std::vector<double>>, unsigned );

// Select a time window from the Matrix -- ( [a,b] + [N_t-b, N_t-a] )
Matrix select_window( Matrix, unsigned, unsigned );

// Calculate the average and standard deviation of different fits
std::vector<double> avg_stde( std::vector<double>, unsigned );

#endif
