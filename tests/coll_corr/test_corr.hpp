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
                Approx( 3.82699 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[0].data[1] == 
                Approx( 0.00117 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[0] == 
                Approx( -3.5797 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[1] == 
                Approx( 0.00043 ).margin( 0.0001 ) );

        // Check the second values
        REQUIRE( corr.boots_central[0].data[2] == 
                Approx( 0.739083 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[0].data[3] == 
                Approx( 0.000175 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[2] == 
                Approx( -0.62902 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[1].data[3] == 
                Approx( 7.419e-05 ).margin( 0.0001 ) );

        // Check the last values
        REQUIRE( corr.boots_central[0].data[time_extent * 2 - 2] == \
                Approx( 0.71723 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[0].data[time_extent * 2 - 1] == \
                Approx( 8.97537e-05 ).margin( 0.0001 ) );
        REQUIRE( corr.boots_central[1].data[time_extent * 2 - 2] == \
                Approx( -0.616186 ).margin( 0.00001 ) );
        REQUIRE( corr.boots_central[1].data[time_extent * 2 - 1] == \
                Approx( 2.81392e-05 ).margin( 0.0001 ) );
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
    SECTION( "Test signal to noise" ) {

        // Create an object
        Correlator corr( ins, num_files );
        corr.sig_to_noise();

        // Check dimensions
        REQUIRE( corr.sig2noise[0].time_extent == 
                corr.sig2noise[0].row_size );
        REQUIRE( corr.sig2noise[0].col_size == 1 );
        REQUIRE( corr.sig2noise[1].time_extent == 
                corr.sig2noise[1].row_size );
        REQUIRE( corr.sig2noise[1].col_size == 1 );

        // Check the values

        // For the first file
        REQUIRE( corr.sig2noise[0].data[0] == Approx( 53.2299 ) );
        REQUIRE( corr.sig2noise[0].data[1] == Approx( 32.2152 ) );
        REQUIRE( corr.sig2noise[0].data[time_extent-1] == 
                Approx( 31.511 ) );

        // For the second file
        REQUIRE( corr.sig2noise[1].data[0] == Approx( -66.7626 ) );
        REQUIRE( corr.sig2noise[1].data[1] == Approx( -44.9311 ) );
        REQUIRE( corr.sig2noise[1].data[time_extent-1] == 
                Approx( -44.0566 ) );
    }
    SECTION( "Test maximum time" ) {

        // Generate a Correlator object
        Correlator corr( ins, num_files );
        unsigned* tmax;

        tmax = corr.get_tmax( 1.0, 0.75 ); 
        REQUIRE( corr.t_max[0] == 36 );
        REQUIRE( tmax[0] == 36 );
        REQUIRE( corr.t_max[1] == 36 );
        REQUIRE( tmax[1] == 36 );

        tmax = corr.get_tmax( 1.0, 0.5 ); 
        REQUIRE( corr.t_max[0] == 24 );
        REQUIRE( tmax[0] == 24 );
        REQUIRE( corr.t_max[1] == 24 );
        REQUIRE( tmax[1] == 24 );

        tmax = corr.get_tmax( 0.5, 0.5 ); 
        REQUIRE( corr.t_max[0] == 24 );
        REQUIRE( tmax[0] == 24 );
        REQUIRE( corr.t_max[1] == 24 );
        REQUIRE( tmax[1] == 24 );

        tmax = corr.get_tmax( 0.1, 0.5 ); 
        REQUIRE( corr.t_max[0] == 7 );
        REQUIRE( tmax[0] == 7 );
        REQUIRE( corr.t_max[1] == 24 );
        REQUIRE( tmax[1] == 24 );
    }
    SECTION( "Test minimum time" ) {

        // Generate a Corr object
        Correlator corr( ins, num_files );
        unsigned* tmin;

        tmin = corr.get_tmin( 10 );
        REQUIRE( corr.t_min[0] == 19 );
        REQUIRE( tmin[0] == 19 );
        REQUIRE( corr.t_min[1] == 19 );
        REQUIRE( tmin[1] == 19 );

        Correlator cpy( ins, num_files );
        tmin = cpy.get_tmin( 5 );
        REQUIRE( cpy.t_min[0] == 22 );
        REQUIRE( tmin[0] == 22 );
        REQUIRE( cpy.t_min[1] == 23 );
        REQUIRE( tmin[1] == 23 );
    }
    SECTION( "Test covariance matrix and tt matrix" ) {

        // Generate a Corr object
        Correlator corr( ins, num_files );

        // Generate the initial and final times
        unsigned* tmin = new unsigned[num_files];
        unsigned* tmax = new unsigned[num_files];
        for ( unsigned i = 0; i < num_files; i++ ) {
            tmin[i] = 0;
            tmax[i] = 2;
        }

        // Generate the covariance matrix for 10 iterations
        corr.cov_matrix( tmin, tmax, 1 );

        // Check the dimensions
        for ( unsigned i = 0; i < num_files; i++ ) {
            REQUIRE( corr.cov_mat[i].row_size == 3 );
            REQUIRE( corr.cov_mat[i].col_size == 3 );
            REQUIRE( corr.cov_mat[i].row_size == 
                    corr.tt_mat[i].row_size );
            REQUIRE( corr.cov_mat[i].col_size == 
                    corr.tt_mat[i].col_size );
            REQUIRE( corr.cov_mat[i].time_extent == time_extent );
            REQUIRE( corr.tt_mat[i].time_extent == time_extent );
        }
        
        double first_A[3] = { 0.00557809, 0.00153316, 0.000654207 };
        double first_B[3] = { 0.00487467, 0.000977171, 0.000363822 };

        double secon_A[3] = { 0.00153316, 0.000543694, 0.000231497 };
        double secon_B[3] = { 0.000977171, 0.00025876, 9.31834e-05 };

        double third_A[3] = { 0.000654207, 0.00023149, 0.000131097 };
        double third_B[3] = { 0.00036382, 9.31834e-05, 4.77836e-05 };

        for ( unsigned i = 0; i < 3; i++ ) {
            REQUIRE( corr.cov_mat[0].data[i] == 
                Approx( first_A[i] ).margin( 0.0001 ) );
            REQUIRE( corr.cov_mat[1].data[i] == 
                Approx( first_B[i] ).margin( 0.0001 ) );
            REQUIRE( corr.cov_mat[0].data[3 + i] == 
                Approx( secon_A[i] ).margin( 0.0001 ) );
            REQUIRE( corr.cov_mat[1].data[3 + i] == 
                Approx( secon_B[i] ).margin( 0.0001 ) );
            REQUIRE( corr.cov_mat[0].data[6 + i] == 
                Approx( third_A[i] ).margin( 0.0001 ) );
            REQUIRE( corr.cov_mat[1].data[6 + i] == 
                Approx( third_B[i] ).margin( 0.0001 ) );
        }

        // Generate a Corr object
        // Generate the covariance matrix for 10 iterations

        // Check the dimensions
        // for ( unsigned i = 0; i < num_files; i++ ) {
        //     REQUIRE( corr.cov_mat[i].row_size == 3 );
        //     REQUIRE( corr.cov_mat[i].col_size == 3 );
        //     REQUIRE( corr.cov_mat[i].row_size == 
        //             corr.tt_mat[i].row_size );
        //     REQUIRE( corr.cov_mat[i].col_size == 
        //             corr.tt_mat[i].col_size );
        //     REQUIRE( corr.cov_mat[i].time_extent == time_extent );
        //     REQUIRE( corr.tt_mat[i].time_extent == time_extent );
        // }
        // 
        // double first_A[3] = { 0.00557809, 0.00153316, 0.000654207 };
        // double first_B[3] = { 0.00487467, 0.000977171, 0.000363822 };

        // double secon_A[3] = { 0.00153316, 0.000543694, 0.000231497 };
        // double secon_B[3] = { 0.000977171, 0.00025876, 9.31834e-05 };

        // double third_A[3] = { 0.000654207, 0.00023149, 0.000131097 };
        // double third_B[3] = { 0.00036382, 9.31834e-05, 4.77836e-05 };

        // for ( unsigned i = 0; i < 3; i++ ) {
        //     REQUIRE( corr.cov_mat[0].data[i] == 
        //         Approx( first_A[i] ).margin( 0.0001 ) );
        //     REQUIRE( corr.cov_mat[1].data[i] == 
        //         Approx( first_B[i] ).margin( 0.0001 ) );
        //     REQUIRE( corr.cov_mat[0].data[3 + i] == 
        //         Approx( secon_A[i] ).margin( 0.0001 ) );
        //     REQUIRE( corr.cov_mat[1].data[3 + i] == 
        //         Approx( secon_B[i] ).margin( 0.0001 ) );
        //     REQUIRE( corr.cov_mat[0].data[6 + i] == 
        //         Approx( third_A[i] ).margin( 0.0001 ) );
        //     REQUIRE( corr.cov_mat[1].data[6 + i] == 
        //         Approx( third_B[i] ).margin( 0.0001 ) );
        // }

        delete[] tmin;
        delete[] tmax;

    }
}
#endif

