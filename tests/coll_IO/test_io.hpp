#ifndef TEST_IO_H
#define TEST_IO_H

#include "io_files.h"
#include "catch.hpp"

TEST_CASE( "Testing IO", "[IO]" ) {

    char file_name[255] = "./coll_IO/file.dat";
    unsigned row_size = 3;
    unsigned col_size = 3;
    
    struct Matrix data = load_data( file_name, row_size, col_size );
    REQUIRE( data.row_size == row_size );
    REQUIRE( data.row_size == row_size );

    double row_zero[3] = { 1.0, 2.0, 3.0 };
    double row_secn[3] = { 3.0, 4.0, 5.0 };
    for ( unsigned i = 0; i < data.col_size; i++ ) {
        REQUIRE( data.data[i] == row_zero[i] );
        REQUIRE( data.data[2*col_size+i] == row_secn[i] );
    }
}

#endif

