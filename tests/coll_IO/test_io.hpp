#ifndef TEST_IO_HPP
#define TEST_IO_HPP

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
    SECTION( "Testing flushing Matrix to file" ) {

        // File to save the data
        std::string file_out = "./coll_IO/out_matrix.dat";

        // Generate the fake data
        unsigned num_data = 4;
        double* data_xy = new double[num_data * 2];
        for ( unsigned i = 0; i < num_data; i++ ) {
            data_xy[i * 2] = i;
            data_xy[i * 2 + 1] = 2 * i;
        }

        // Generate the Matrix to flush out
        Matrix data = { data_xy, num_data, 2, 10 };

        write_matrix( file_out, data );

        // Load the data to check it works
        Input input_matrix = { file_out, num_data, 3, 10 };
        Matrix load_matrix = load_data( input_matrix );

        // Check dimensions
        REQUIRE( load_matrix.row_size == num_data );
        REQUIRE( load_matrix.col_size == 3 );

        unsigned cols = load_matrix.col_size;
        for ( unsigned i = 0; i < num_data; i++ ) {
            REQUIRE( load_matrix.data[i * cols] == i );
            for ( unsigned j = 0; j < 2; j++ ) {
                REQUIRE( load_matrix.data[i * cols + 1 + j] ==
                    data.data[i * 2 + j] );
            }
        }

        // Free the pointers
        delete[] data.data;
        delete[] load_matrix.data;
    }
    SECTION( "Testing flushing vector to file" ) {

        // File to save the data
        std::string file_out = "./coll_IO/out_vector.dat";

        // Generate the fake data
        unsigned wi_start = 2;
        unsigned num_data = 6;
        unsigned dim_param = 2;
        unsigned num_cols = ( dim_param + 1 ) * 2;

        std::vector<unsigned> wi_final( num_data );
        for ( unsigned i = 0; i < wi_final.size(); i++ ) {
            wi_final[i] = wi_start + 1 + i;
        }

        std::vector<double> results( num_data * num_cols );
        for ( unsigned i = 0; i < num_data; i++ ) {
            for ( unsigned j = 0; j < num_cols; j++ ) {
                results[i * num_cols + j] = i + j;
            }
        }

        write_vector( file_out, results, wi_final, wi_start );

        // Load the data to check it works
        Input input_matrix = 
            { file_out, num_data, num_cols + 1, 10 };
        Matrix load_matrix = load_data( input_matrix );

        // Check dimensions
        REQUIRE( load_matrix.row_size == num_data );
        REQUIRE( load_matrix.col_size == num_cols + 1 );

        unsigned cols = load_matrix.col_size;
        for ( unsigned i = 0; i < num_data; i++ ) {
            REQUIRE( load_matrix.data[i * cols] == wi_final[i] );
            for ( unsigned j = 0; j < num_cols; j++ ) {
                REQUIRE( 
                    load_matrix.data[i * (num_cols + 1) + 1 + j] ==
                    results[i * num_cols + j]
                );
            }
        }

        // Free vectors and pointers
        wi_final.clear();
        results.clear();
        delete[] load_matrix.data;
    }
}

#endif

