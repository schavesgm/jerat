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

    Input ins[num_files];
    for ( unsigned nf = 0; nf <  num_files; nf++ ) {
        ins[nf] = { FILES[nf].c_str(), ROWS[nf], col_size, 
            time_extent };
    }
    SECTION( "Test constructor" ) {

        // Create an object
        Correlator corr( ins, num_files );

        // Check the dimensions of each RAW data
        REQUIRE( corr.RAW_DATA[0].row_size == ROWS[0] );
        REQUIRE( corr.RAW_DATA[1].row_size == ROWS[1] );
        REQUIRE( corr.RAW_DATA[0].col_size == col_size );
        REQUIRE( corr.RAW_DATA[1].col_size == col_size );
        REQUIRE( corr.RAW_DATA[0].time_extent == time_extent );
        REQUIRE( corr.RAW_DATA[1].time_extent == time_extent );

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
    // for ( unsigned nf = 0; nf < num_files; nf++ ) {
    //     const char* name = FILES[nf].c_str();
    //     ins[nf] = { name, ROWS[nf], col_size, time_extent };
    // }

    // SECTION( "Test constructor" ) {


    //     // Generate a Corre
    //     Correlator corr( ins, num_files );

    //     // REQUIRE( corr.raw.row_size == row_size );
    //     // REQUIRE( corr.raw.col_size == col_size );
    //     // REQUIRE( corr.raw.time_extent == time_extent );
    //     // REQUIRE( corr.raw.data[0] == 0.0 );
    //     // REQUIRE( corr.raw.data[row_size*col_size-col_size] == 47.0 );
    // }
    // SECTION( "Test central value -- Average and Variance" ) {

    //     // Generate a Corr object
    //     Corr corr( file_name, row_size, col_size, time_extent );
    //     corr.cent_corr( 1 );

    //     // Check dimensions
    //     REQUIRE( corr.cent.row_size == corr.cent.time_extent );
    //     REQUIRE( corr.cent.col_size == 2 );

    //     // Check the first values
    //     REQUIRE( corr.cent.data[0] == Approx( 3.86904 ) );
    //     REQUIRE( corr.cent.data[1] == 
    //             Approx( 0.00528 ).margin( 0.00001 ) );

    //     // Check the second values
    //     REQUIRE( corr.cent.data[2] == Approx( 0.741684 ) );
    //     REQUIRE( corr.cent.data[3] == 
    //             Approx( 0.00053 ).margin( 0.00001 ) );

    //     // Check the last values
    //     REQUIRE( corr.cent.data[time_extent * 2 - 2] == \
    //             Approx( 0.741828 ) );
    //     REQUIRE( corr.cent.data[time_extent * 2 - 1] == \
    //             Approx( 0.000554 ).margin( 0.00001 ) );
    // }
    // SECTION( "Test bootstrap estimation" ) {

    //     // Generate a Corr object
    //     Corr corr( file_name, row_size, col_size, time_extent );
    //     corr.boot_est();

    //     // Check dimensions
    //     REQUIRE( corr.best_est.row_size == time_extent );
    //     REQUIRE( corr.best_est.col_size == 2 );
    //     REQUIRE( corr.best_est.time_extent == time_extent );

    //     // Check first values
    //     REQUIRE( corr.best_est.data[0] == 
    //             Approx( 3.8672 ).margin( 0.0001 ) );
    //     REQUIRE( corr.best_est.data[1] == 
    //             Approx( 0.01212 ).margin( 0.001 ) );

    //     REQUIRE( corr.best_est.data[2] == 
    //             Approx( 0.741337 ).margin( 0.0001 ) );
    //     REQUIRE( corr.cent.data[3] == 
    //             Approx( 0.00078 ).margin( 0.001 ) );

    //     // Check the last values
    //     REQUIRE( corr.best_est.data[time_extent * 2 - 2] == \
    //             Approx( 0.74149 ).margin( 0.0001 ) );
    //     REQUIRE( corr.cent.data[time_extent * 2 - 1] == \
    //             Approx( 0.00080 ).margin( 0.001 ) );
    // }
    // SECTION( "Test covariance matrix and tt matrix" ) {

    //     // Generate a Corr object
    //     Corr corr( file_name, row_size, col_size, 
    //         time_extent, 123 );
    //     corr.cov_matrix( 1, 2, 10 );

    //     // Check the dimensions for covariance and tt matrix
    //     REQUIRE( corr.covmat.row_size == 2 );
    //     REQUIRE( corr.ttmat.row_size == 2 );
    //     REQUIRE( corr.covmat.col_size == 2 );
    //     REQUIRE( corr.ttmat.col_size == 2 );
    //     REQUIRE( corr.covmat.time_extent == time_extent );
    //     REQUIRE( corr.ttmat.time_extent == time_extent );

    //     // Check the values for different number of bootstrap 

    //     // For 10 bootstrap iterations
    //     REQUIRE( corr.covmat.data[0] == 
    //             Approx( 0.00069538 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[3] == 
    //             Approx( 0.000192328 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[1] == 
    //             Approx( 0.000116838 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[2] == 
    //             Approx( 0.000116838 ).margin( 0.00001 ) );

    //     REQUIRE( corr.ttmat.data[0] == 
    //             Approx( 0.00069538 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[3] == 
    //             Approx( 0.000192328 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[1] == 0.0 );
    //     REQUIRE( corr.ttmat.data[2] == 0.0 );

    //     // For 100 bootstrap iterations
    //     corr.cov_matrix( 1, 2, 100 );
    //     REQUIRE( corr.covmat.data[0] == 
    //             Approx( 0.000816549 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[3] == 
    //             Approx( 0.000238198 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[1] == 
    //             Approx( 5.20208e-05 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[2] == 
    //             Approx( 5.20208e-05 ).margin( 0.00001 ) );

    //     REQUIRE( corr.ttmat.data[0] == 
    //             Approx( 0.000816549 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[3] == 
    //             Approx( 0.000238198 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[1] == 0.0 );
    //     REQUIRE( corr.ttmat.data[2] == 0.0 );

    //     // // For 200 bootstrap iterations
    //     corr.cov_matrix( 1, 2, 200 );
    //     REQUIRE( corr.covmat.data[0] == 
    //             Approx( 0.00075124 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[3] == 
    //             Approx( 0.000211729 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[1] == 
    //             Approx( 8.97346e-05 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[2] == 
    //             Approx( 8.97346e-05 ).margin( 0.00001 ) );

    //     REQUIRE( corr.ttmat.data[0] == 
    //             Approx( 0.00075124 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[3] == 
    //             Approx( 0.000211729 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[1] == 0.0 );
    //     REQUIRE( corr.ttmat.data[2] == 0.0 );

    //     // For 1000 bootstrap iterations
    //     corr.cov_matrix( 1, 2, 1000 );
    //     REQUIRE( corr.covmat.data[0] == 
    //             Approx( 0.000770036 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[3] == 
    //             Approx( 0.000218769 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[1] == 
    //             Approx( 7.87164e-05 ).margin( 0.00001 ) );
    //     REQUIRE( corr.covmat.data[2] == 
    //             Approx( 7.87164e-05 ).margin( 0.00001 ) );

    //     REQUIRE( corr.ttmat.data[0] == 
    //             Approx( 0.000770036 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[3] == 
    //             Approx( 0.000218769 ).margin( 0.00001 ) );
    //     REQUIRE( corr.ttmat.data[1] == 0.0 );
    //     REQUIRE( corr.ttmat.data[2] == 0.0 );
    // }
    // SECTION( "Test signal to noise" ) {

    //     // Generate a Corr object
    //     Corr corr( file_name, row_size, col_size, time_extent );
    //     corr.sig_to_noise();

    //     // Check dimensions
    //     REQUIRE( corr.stn.time_extent == corr.stn.row_size );
    //     REQUIRE( corr.stn.col_size == 1 );

    //     // Check the values
    //     REQUIRE( corr.stn.data[0] == Approx( 53.2299 ) );
    //     REQUIRE( corr.stn.data[1] == Approx( 32.2152 ) );
    //     REQUIRE( corr.stn.data[time_extent-1] == Approx( 31.511 ) );
    // }
    // SECTION( "Test maximum time" ) {

    //     // Generate a Corr object
    //     Corr corr( file_name, row_size, col_size, time_extent );
    //     corr.get_tmax( 1.0, 0.75 * time_extent ); 

    //     REQUIRE( corr.tmax == 36 );
    //     corr.get_tmax( 1.0, 0.5 * time_extent ); 
    //     REQUIRE( corr.tmax == 24 );
    //     corr.get_tmax( 0.5, 0.5 * time_extent ); 
    //     REQUIRE( corr.tmax == 24 );
    //     corr.get_tmax( 0.1, 0.5 * time_extent ); 
    //     REQUIRE( corr.tmax == 7 );
    // }
    // SECTION( "Test minimum time" ) {

    //     // Generate a Corr object
    //     Corr corr( file_name, row_size, col_size, time_extent, 123 );
    //     corr.get_tmin( 10 );
    //     REQUIRE( corr.tmin == 19 );
    //     corr.get_tmin( 10 );
    //     REQUIRE( corr.tmin == 19 );

    //     Corr cpy( file_name, row_size, col_size, time_extent, 123 );
    //     cpy.get_tmin( 5 );
    //     REQUIRE( cpy.tmin == 22 );
    //     cpy.get_tmin( 5 );
    //     REQUIRE( cpy.tmin == 23 );

    // }
}
#endif

