#include <iostream>
#include <random>

// Load headers 
#include "corr.hpp"
#include <math.h>
#include "fit.hpp"

// double f( std::vector<double> params, double x ) {
//     return params[0] * std::exp( - params[1] * x ) + \
//         std::cos( params[2] * x ) + \
//         std::sin( params[3] * x );
// }

double f( std::vector<double> params, double x ) {
    return params[0] * std::exp( - params[1] * x ) + \
        params[2] * std::exp( - params[3] * ( 48 - x ) );
}

int main() {

    // True parameters
    std::vector<double> true_params = { 1, 0.25, 1, 0.25 };
    unsigned num_points = 48;

    // Random engine
    std::mt19937 r_eng;
    r_eng.seed( 24680  );
    std::uniform_real_distribution<double> sampler( -1, 1 );

    // Generate fake parameters
    std::vector<double> f_params( true_params.size() );
    for ( unsigned i = 0; i < true_params.size(); i++ ) {
        f_params[i] = true_params[i] + \
            true_params[i] * sampler(r_eng); 
    }

    // Create the fake data with added noise
    double* vals = new double[num_points * 2];
    for( unsigned i = 0; i < num_points; i++ ) {
        vals[i * 2] = i;
        vals[i * 2 + 1] = f( true_params, i ) + \
            0.01 * sampler(r_eng);
    }
    Matrix data = { vals, num_points, 2, 1 };

    // Fit the data
    std::vector<double> fit = fitNM( f, data, f_params, f_params );

    for ( unsigned i = 0; i < fit.size() - 1; i++ ) 
        std::cout << fit[i] << " ";
    std::cout << std::endl;
    std::cout << "Chi square: " << fit[fit.size()-1] << std::endl;

    // true_params.clear();
    // f_params.clear();
    // delete[] data.data;

return 0;
}
