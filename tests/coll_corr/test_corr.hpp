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

        /// Generate the constructor
        Corr corr( file_name, row_size, col_size, time_extent );
        REQUIRE( corr.raw.row_size == row_size );
        REQUIRE( corr.raw.col_size == col_size );
        REQUIRE( corr.raw.time_extent == time_extent );

    }
}
#endif

