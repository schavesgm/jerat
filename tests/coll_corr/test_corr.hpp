#ifndef TEST_CORR_H
#define TEST_CORR_H

#include "corr.hpp"
#include "catch.hpp"

TEST_CASE( "Correlation function class", "[Corr]" ) {

    char file_name[255] = "./coll_corr/file.dat";
    unsigned row_size = 106752;
    unsigned col_size = 3;
    unsigned time_extent = 48;

    SECTION( "Test constructor" ) {

        // Generate a Corr object
        Corr corr( file_name, row_size, col_size, time_extent );
        REQUIRE( corr.raw.row_size == row_size );
        REQUIRE( corr.raw.col_size == col_size );
        REQUIRE( corr.raw.time_extent == time_extent );
        REQUIRE( corr.raw.data[0] == 0.0 );
        REQUIRE( corr.raw.data[row_size*col_size-col_size] == 47.0 );
    }
    SECTION( "Test central value" ) {

        // Generate a Corr object
        Corr corr( file_name, row_size, col_size, time_extent );
        corr.cent_corr( 1 );

        // Check dimensions
        REQUIRE( corr.cent.row_size == corr.cent.time_extent );
        REQUIRE( corr.cent.col_size == 2 );

        // Check the first values
        REQUIRE( corr.cent.data[0] == Approx( 3.86904 ) );
        REQUIRE( 
            corr.cent.data[1] == Approx( 0.00528 ).margin( 0.00001 ) 
        );

        // Check the second values
        REQUIRE( corr.cent.data[2] == Approx( 0.741684 ) );
        REQUIRE( 
            corr.cent.data[3] == Approx( 0.00053 ).margin( 0.00001 )
        );

        // Check the last values
        REQUIRE( corr.cent.data[time_extent * 2 - 2] == \
                Approx( 0.741828 ) );
        REQUIRE( corr.cent.data[time_extent * 2 - 1] == \
                Approx( 0.000554 ).margin( 0.00001 ) );
    }
    SECTION( "Test signal to noise" ) {

        // Generate a Corr object
        Corr corr( file_name, row_size, col_size, time_extent );
        corr.sig_to_noise();

        // Check dimensions
        REQUIRE( corr.stn.time_extent == corr.stn.row_size );
        REQUIRE( corr.stn.col_size == 1 );

        // Check the values
        REQUIRE( corr.stn.data[0] == Approx( 53.2299 ) );
        REQUIRE( corr.stn.data[1] == Approx( 32.2152 ) );
        REQUIRE( corr.stn.data[time_extent-1] == Approx( 31.511 ) );
    }
    SECTION( "Test maximum time" ) {

        // Generate a Corr object
        int tmax;
        Corr corr( file_name, row_size, col_size, time_extent );
        tmax = corr.get_tmax( 1.0, 0.75 * time_extent ); 
        REQUIRE( tmax == 36 );
        tmax = corr.get_tmax( 1.0, 0.5 * time_extent ); 
        REQUIRE( tmax == 24 );
        tmax = corr.get_tmax( 0.5, 0.5 * time_extent ); 
        REQUIRE( tmax == 24 );
        tmax = corr.get_tmax( 0.1, 0.5 * time_extent ); 
        REQUIRE( tmax == 7 );
    }
}
#endif

