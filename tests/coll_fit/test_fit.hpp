#ifndef TEST_FIT_H
#define TEST_FIT_H

#include <vector>
#include <random>
#include <math.h>

#include "fit.hpp"
#include "catch.hpp"

double f_1( std::vector<double> params, double x ) {
    return params[0] * x * x + params[1] * x;
}

double f_2( std::vector<double> params, double x ) {
    return params[0] * std::exp( -x ) + \
        params[1] * std::pow( x, 2 ) - params[2] / ( x + 1 );
}

double f_3( std::vector<double> params, double x ) {
    return params[0] * std::exp( - params[1] * x ) + \
        std::cos( params[2] * x ) + \
        std::sin( params[3] * x );
}

double f_4( std::vector<double> params, double x ) {
    return params[0] * std::exp( - params[1] * x ) + \
        params[2] * std::exp( - params[3] * ( 48 - x ) );
}

TEST_CASE( "Testing Fit", "[Fit]" ) {

    SECTION( "Testing chi square" ) {


        // Generate the parameters
        std::vector<double> params = { 1.0, -1.0 };
        unsigned inputs = 10;

        double* vals_1 = new double[inputs * 2];
        double* vals_2 = new double[inputs * 2];
        double* vals_3 = new double[inputs * 2];

        double x;
        for ( unsigned i = 0; i < inputs; i++ ) {
            x = 0.1 * i;
            vals_1[i * 2] = x;
            vals_2[i * 2] = x;
            vals_3[i * 2] = x;
            vals_1[i * 2 + 1] = f_1( params, x );
            vals_2[i * 2 + 1] = f_1( params, x ) + 0.1;
            vals_3[i * 2 + 1] = f_1( params, x ) + 0.01;
        }
        Matrix data_1 = { vals_1, inputs, 2, 1 };
        Matrix data_2 = { vals_2, inputs, 2, 1 };
        Matrix data_3 = { vals_3, inputs, 2, 1 };

        double chi_1 = chi_square( f_1, params, data_1 );
        double chi_2 = chi_square( f_1, params, data_2 );
        double chi_3 = chi_square( f_1, params, data_3 );

        REQUIRE( chi_1 == Approx( 0.0 ).margin( 0.01 ) );
        REQUIRE( chi_2 == Approx( 0.01 ).margin( 0.001 ) );
        REQUIRE( chi_3 == Approx( 0.0001 ).margin( 0.00001 ) );


        delete[] data_1.data;
        delete[] data_2.data;
        delete[] data_3.data;
    }
    SECTION( "Testing reordering matrix" ) {

        unsigned dim_param_1 = 2;
        unsigned dim_simp_1 = dim_param_1 + 1;
        std::vector<double> simplex_1 = { 0, 0, 1, 1, 2, 2 };
        std::vector<double> image_s_1 = { 0.5, 0.1, 0.3 };

        double correct_im_1[dim_simp_1] = { 0.1, 0.3, 0.5 };
        double correct_si_1[dim_simp_1 * dim_param_1] = \
            { 1, 1, 2, 2, 0, 0 };

        // Reorder the matrix according to image_s
        order_simplex( simplex_1, image_s_1 );

        // Check correct dimensions
        REQUIRE( simplex_1.size() == dim_param_1 * dim_simp_1 );
        REQUIRE( image_s_1.size() == dim_simp_1 );

        for ( unsigned ir = 0; ir < dim_simp_1; ir++ ) {
            REQUIRE( image_s_1[ir] == correct_im_1[ir] );
            for ( unsigned ic = 0; ic < dim_param_1; ic++ ) {
                REQUIRE( simplex_1[ir * dim_param_1 + ic] ==
                        correct_si_1[ir * dim_param_1 + ic] );
            }
        }

        unsigned dim_param_2 = 3;
        unsigned dim_simp_2 = dim_param_2 + 1;
        std::vector<double> simplex_2 = \
            { 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3 };
        std::vector<double> image_s_2 = \
            { -0.1, 0.0, -0.3, -0.2 };

        double correct_im_2[dim_simp_2] = \
            { -0.3, -0.2, -0.1, 0.0 };
        double correct_si_2[dim_simp_2 * dim_param_2] = \
            { 2, 2, 2, 3, 3, 3, 0, 0, 0, 1, 1, 1 };

        // Reorder the matrix according to image_s
        order_simplex( simplex_2, image_s_2 );

        // Check correct dimensions
        REQUIRE( simplex_2.size() == dim_param_2 * dim_simp_2 );
        REQUIRE( image_s_2.size() == dim_simp_2 );

        for ( unsigned ir = 0; ir < dim_simp_2; ir++ ) {
            REQUIRE( image_s_2[ir] == correct_im_2[ir] );
            for ( unsigned ic = 0; ic < dim_param_2; ic++ ) {
                REQUIRE( simplex_2[ir * dim_param_2 + ic] ==
                        correct_si_2[ir * dim_param_2 + ic] );
            }
        }
        simplex_1.clear();
        simplex_2.clear();
        image_s_1.clear();
        image_s_2.clear();
    }
    SECTION( "Testing linear Nelder-Mead - Case 1" ) {

        // True parameters
        std::vector<double> true_params = { 1.0, -0.5 };
        unsigned num_points = 100;

        // Random engine
        std::mt19937 r_eng;
        r_eng.seed( 7654321 );
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
            vals[i * 2] = 0.1 * i;
            vals[i * 2 + 1] = f_1( true_params, 0.1 * i ) + \
                0.01 * sampler(r_eng);
        }
        Matrix data = { vals, num_points, 2, 1 };

        // Fit the data
        std::vector<double> fit = fitNM( f_1, data, f_params, \
                f_params );
 
        // Check the values
        REQUIRE( fit[0] == Approx( 1 ).margin( 0.00001 ) );
        REQUIRE( fit[1] == Approx( -0.49991 ).margin( 0.0001 ) );
        REQUIRE( fit[2] == Approx( 3.34e-5 ).margin( 0.000001 ) );

        // Check some dimensions
        REQUIRE( fit.size() == true_params.size() + 1 );

        true_params.clear();
        f_params.clear();
        delete[] data.data;
    }
    SECTION( "Testing linear Nelder-Mead - Case 2" ) {

        // True parameters
        std::vector<double> true_params = { 3.45, -0.25, 0.5 };
        unsigned num_points = 10;

        // Random engine
        std::mt19937 r_eng;
        r_eng.seed( 12341234 );
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
            vals[i * 2] = 0.1 * i;
            vals[i * 2 + 1] = f_2( true_params, 0.1 * i ) + \
                0.001 * sampler(r_eng);
        }
        Matrix data = { vals, num_points, 2, 1 };

        // Fit the data
        std::vector<double> fit = fitNM( f_2, data, f_params, \
                f_params );

        // Check the values
        REQUIRE( fit[0] == Approx( 3.44404 ).margin( 0.0001 ) );
        REQUIRE( fit[1] == Approx( -0.2514 ).margin( 0.0001 ) );
        REQUIRE( fit[2] == Approx( 0.49445 ).margin( 0.0001 ) );
        REQUIRE( fit[3] == Approx( 2.0e-07 ).margin( 0.0001 ) );

        // Check some dimensions
        REQUIRE( fit.size() == true_params.size() + 1 );

        true_params.clear();
        f_params.clear();
        delete[] data.data;
    }
    SECTION( "Testing non-linear Nelder-Mead - Case 1" ) {

        // True parameters
        std::vector<double> true_params = { 0.25, 0.1, 0.2, 0.3  };
        unsigned num_points = 100;

        // Random engine
        std::mt19937 r_eng;
        r_eng.seed( 13579 );
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
            vals[i * 2] = 0.1 * i;
            vals[i * 2 + 1] = f_3( true_params, 0.1 * i ) + \
                0.01 * sampler(r_eng);
        }
        Matrix data = { vals, num_points, 2, 1 };

        // Fit the data
        std::vector<double> fit = fitNM( f_3, data, f_params, \
                f_params );

        // Check the values
        REQUIRE( fit[0] == Approx( 0.24983 ).margin( 0.0001 ) );
        REQUIRE( fit[1] == Approx( 0.10176 ).margin( 0.0001 ) );
        REQUIRE( fit[2] == Approx( 0.19952 ).margin( 0.0001 ) );
        REQUIRE( fit[3] == Approx( 0.30018 ).margin( 0.0001 ) );
        REQUIRE( fit[4] == Approx( 3.3e-05 ).margin( 0.0001 ) );

        // Check some dimensions
        REQUIRE( fit.size() == true_params.size() + 1 );

        true_params.clear();
        f_params.clear();
        delete[] data.data;
    }
    SECTION( "Testing non-linear Nelder-Mead - Case 2" ) {

        // True parameters
        std::vector<double> true_params = { 1, 0.25, 1, 0.25  };
        unsigned num_points = 48;

        // Random engine
        std::mt19937 r_eng;
        r_eng.seed( 24680 );
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
            vals[i * 2 + 1] = f_4( true_params, i ) + \
                0.01 * sampler(r_eng);
        }
        Matrix data = { vals, num_points, 2, 1 };

        // Fit the data
        std::vector<double> fit = fitNM( f_4, data, f_params, \
                f_params );

        // Check the values
        REQUIRE( fit[0] == Approx( 0.9910 ).margin( 0.0001 ) );
        REQUIRE( fit[1] == Approx( 0.2450 ).margin( 0.0001 ) );
        REQUIRE( fit[2] == Approx( 0.9956 ).margin( 0.0001 ) );
        REQUIRE( fit[3] == Approx( 0.2509 ).margin( 0.0001 ) );
        REQUIRE( fit[4] == Approx( 2.4e-05 ).margin( 0.0001 ) );

        // Check some dimensions
        REQUIRE( fit.size() == true_params.size() + 1 );

        true_params.clear();
        f_params.clear();
        delete[] data.data;
    }
}

#endif

