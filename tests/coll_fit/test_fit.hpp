#ifndef TEST_IO_H
#define TEST_IO_H

#include <vector>

#include "fit.hpp"
#include "catch.hpp"

TEST_CASE( "Testing Fit", "[Fit]" ) {

    SECTION( "Testing chi square" ) {

        double f( std::vector<double> params, double x ) {
            return params[0] * x * x + params[1] * x;
        }

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
            vals_1[i * 2 + 1] = f( params, x );
            vals_2[i * 2 + 1] = f( params, x ) + 0.1;
            vals_3[i * 2 + 1] = f( params, x ) + 0.01;
        }
        Matrix data_1 = { vals_1, inputs, 2, 1 };
        Matrix data_2 = { vals_2, inputs, 2, 1 };
        Matrix data_3 = { vals_3, inputs, 2, 1 };

        double chi_1 = chi_square( f, params, data_1 );
        double chi_2 = chi_square( f, params, data_2 );
        double chi_3 = chi_square( f, params, data_3 );

        REQUIRE( chi_1 == 0.0 );
        REQUIRE( chi_2 == 0.01 );
        REQUIRE( chi_1 == 0.0001 );


        delete[] data_1.data;
        delete[] data_2.data;
        delete[] data_3.data;
    }
}

#endif

