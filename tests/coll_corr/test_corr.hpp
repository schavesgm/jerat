#ifndef TEST_CORR_HPP
#define TEST_CORR_HPP

#include <iostream>

#include "corr.hpp"
#include "catch.hpp"

TEST_CASE( "Correlation function class", "[Corr]" ) {

    unsigned num_files = 2;
    std::vector<std::string> FILES;
    FILES.push_back( "../Gen2l_48x32.meson.g5.cc" );
    FILES.push_back( "../Gen2l_48x32.meson.vec.cc" );

    unsigned ROWS[num_files] = { 106752, 3 * 106752 };
    unsigned col_size = 3;
    unsigned time_extent = 48;
    unsigned seed = 1234;

    Input ins[num_files];
    for ( unsigned nf = 0; nf <  num_files; nf++ ) {
        ins[nf] = { FILES[nf].c_str(), ROWS[nf], col_size, 
            time_extent };
    }
    SECTION( "Test constructor" ) {

        // Create an object
        Correlator corr( ins, num_files, seed );

        // Check the dimensions of each RAW data
        REQUIRE( corr.RAW_DATA[0].row_size == ROWS[0] );
        REQUIRE( corr.RAW_DATA[1].row_size == ROWS[1] );
        REQUIRE( corr.RAW_DATA[0].col_size == col_size );
        REQUIRE( corr.RAW_DATA[1].col_size == col_size );
        REQUIRE( corr.RAW_DATA[0].time_extent == time_extent );
        REQUIRE( corr.RAW_DATA[1].time_extent == time_extent );

        REQUIRE( corr.seed == seed );

        // Check the values
        REQUIRE( corr.RAW_DATA[0].data[0] == 0 );
        REQUIRE( corr.RAW_DATA[0].data[1] == 
                Approx( 3.8986 ).margin( 0.0001 ) );
        REQUIRE( corr.RAW_DATA[0].data[3] == 1 );
        REQUIRE( corr.RAW_DATA[0].data[4] == 
                Approx( 0.7663 ).margin( 0.0001 ) );

        REQUIRE( corr.RAW_DATA[1].data[0] == 0 );
        REQUIRE( corr.RAW_DATA[1].data[1] == 
                Approx( -3.6249 ).margin( 0.0001 ) );
        REQUIRE( corr.RAW_DATA[1].data[3] == 1 );
        REQUIRE( corr.RAW_DATA[1].data[4] == 
                Approx( -0.6480 ).margin( 0.0001 ) );
    }
    SECTION( "Test bootstrap estimation" ) {

        // Create an object
        Correlator corr( ins, num_files );
        corr.bootstrap_central();

        // Check dimensions
        REQUIRE( corr.boots_central[0].row_size == time_extent );
        REQUIRE( corr.boots_central[1].row_size == time_extent );
        REQUIRE( corr.boots_central[0].col_size == 2 );
        REQUIRE( corr.boots_central[1].col_size == 2 );
        REQUIRE( corr.boots_central[0].time_extent == time_extent );
        REQUIRE( corr.boots_central[1].time_extent == time_extent );

        // Check the first values
        REQUIRE( corr.boots_central[0].data[0] == 
                Approx( 3.84431 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[0].data[1] == 
                Approx( 0.00159 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[0] == 
                Approx( -3.58932 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[1] == 
                Approx( 0.00060 ).margin( 0.0001 ) );

        // Check the second values
        REQUIRE( corr.boots_central[0].data[2] == 
                Approx( 0.745041 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[0].data[3] == 
                Approx( 0.000247 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[2] == 
                Approx( -0.63389 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[1].data[3] == 
                Approx( 9.61e-05 ).margin( 0.0001 ) );
 
        // Check the last values
        REQUIRE( corr.boots_central[0].data[time_extent * 2 - 2] == \
                Approx( 0.716315 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[0].data[time_extent * 2 - 1] == \
                Approx( 0.000113 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[time_extent * 2 - 2] == \
                Approx( -0.61421 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[1].data[time_extent * 2 - 1] == \
                Approx( 2.47e-05 ).margin( 0.0001 ) );
    }
    SECTION( "Test central value -> Average and Variance" ) {

        // Create an object
        Correlator corr( ins, num_files );
        corr.central_value();

        // Check the dimensions of the central value
        REQUIRE( corr.central[0].row_size == time_extent );
        REQUIRE( corr.central[1].row_size == time_extent );
        REQUIRE( corr.central[0].col_size == 2 );
        REQUIRE( corr.central[1].col_size == 2 );
        REQUIRE( corr.central[0].time_extent == time_extent );
        REQUIRE( corr.central[1].time_extent == time_extent );

        // Check the values obtained

        // Check the first values
        REQUIRE( corr.central[0].data[0] == 
                Approx( 3.86904 ).margin( 0.00001 ) );
        REQUIRE( corr.central[0].data[1] == 
                Approx( 0.00528 ).margin( 0.00001 ) );
        REQUIRE( corr.central[1].data[0] == 
                Approx( -3.60454 ).margin( 0.00001 ) );
        REQUIRE( corr.central[1].data[1] == 
                Approx( 0.002914 ).margin( 0.00001 ) );

        // Check the second values
        REQUIRE( corr.central[0].data[2] == 
                Approx( 0.741684 ).margin( 0.00001 ) );
        REQUIRE( corr.central[0].data[3] == 
                Approx( 0.00053 ).margin( 0.00001 ) );
        REQUIRE( corr.central[1].data[2] == 
                Approx( -0.627362 ).margin( 0.00001 ) );
        REQUIRE( corr.central[1].data[3] == 
                Approx( 0.000194 ).margin( 0.00001 ) );

        // Check the last values
        REQUIRE( corr.central[0].data[time_extent * 2 - 2] == \
                Approx( 0.741828 ).margin( 0.00001 ) );
        REQUIRE( corr.central[0].data[time_extent * 2 - 1] == \
                Approx( 0.000554 ).margin( 0.00001 ) );
        REQUIRE( corr.central[1].data[time_extent * 2 - 2] == \
                Approx( -0.62737 ).margin( 0.00001 ) );
        REQUIRE( corr.central[1].data[time_extent * 2 - 1] == \
                Approx( 0.000202 ).margin( 0.00001 ) );
    }
}

#endif

