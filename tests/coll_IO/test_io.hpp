#ifndef TEST_IO_H
#define TEST_IO_H

#include "io_files.hpp"
#include "catch.hpp"

TEST_CASE( "Testing IO", "[IO]" ) {

    SECTION( "Testing opening and loading file" ) {

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
    SECTION( "Testing searching for data in file" ) {

        std::string input_file = "./coll_IO/input.dat";
        std::vector<std::string> vals = 
            get_key( input_file, "files" );
        std::vector<std::string> nums = 
            get_key( input_file, "numeros" );
        std::vector<double> dnums = to_double( nums );
        std::vector<unsigned> unums = to_unsigned( nums );

        std::vector<std::string> res_strings = 
            { "hola", "caracola", "pepsicola" };
        std::vector<double> res_doubles = 
            { 1.0, 2.0, 3.0, 4.0, 5.0 };
        std::vector<unsigned> res_unsigned = 
            { 1, 2, 3, 4, 5 };

        for ( unsigned i = 0; i < vals.size(); i++ ) {
            REQUIRE( vals[i] == res_strings[i] );
            REQUIRE( dnums[i] == res_doubles[i] );
            REQUIRE( unums[i] == res_unsigned[i] );
        }

        vals.clear();
        nums.clear();
        dnums.clear();
        unums.clear();
        res_strings.clear();
        res_doubles.clear();
        res_unsigned.clear();
    }
}

#endif

