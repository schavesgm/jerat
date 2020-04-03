#ifndef TEST_IO_H
#define TEST_IO_H

#include "io_files.hpp"
#include "catch.hpp"

TEST_CASE( "Testing IO", "[IO]" ) {

    std::string file_name = "./coll_IO/file.dat";
    unsigned row_size = 3;
    unsigned col_size = 3;
    unsigned time_extent = 48;

    Input input_1 = 
        { file_name, row_size, col_size, time_extent };
    Input input_2 = 
        { file_name, row_size, col_size, time_extent };
    input_2.rescale = true;
    input_2.res_factor = 2.0;
    
    struct Matrix data_1 = load_data( input_1 );
    struct Matrix data_2 = load_data( input_2 );

    REQUIRE( data_1.row_size == row_size );
    REQUIRE( data_1.row_size == row_size );
    REQUIRE( data_1.time_extent == time_extent );
    REQUIRE( data_2.row_size == row_size );
    REQUIRE( data_2.row_size == row_size );
    REQUIRE( data_2.time_extent == time_extent );

    double row_zero_1[3] = { 1.0, 2.0, 3.0 };
    double row_zero_2[3] = { 1.0, 4.0, 3.0 };
    double row_secn_1[3] = { 3.0, 4.0, 5.0 };
    double row_secn_2[3] = { 3.0, 8.0, 5.0 };

    for ( unsigned i = 0; i < data_1.col_size; i++ ) {
        REQUIRE( data_1.data[i] == row_zero_1[i] );
        REQUIRE( data_2.data[i] == row_zero_2[i] );
        REQUIRE( data_1.data[2*col_size+i] == row_secn_1[i] );
        REQUIRE( data_2.data[2*col_size+i] == row_secn_2[i] );
    }
}

#endif

