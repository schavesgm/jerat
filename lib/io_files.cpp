#include "io_files.hpp"

struct corr load_data( 
    const char file_name[255], const unsigned row_size, 
    const unsigned col_size, const unsigned time_extent, 
    std::string comm_char, const bool res_bool, 
    const unsigned res_col, const double res_val ) {
    /*
       Function to load data from file. The function is able to skip
       comments inside the file. Default comment character is #.
    */

    // Open the stream
    std::ifstream in(file_name);
    // Dummy line that will store each line);
    std::string line;   
    // Pointer to store the data into
    double* store = new double[row_size * col_size];

    // Handle comments
    unsigned aux_row = 0;

    while( std::getline( in, line ) ) {

        // Generate a input object to operate with files
        std::istringstream ss(line);

        // Placing all tokens inside a vector
        std::istream_iterator<std::string> begin(ss), end;
        // Save data into vector

        std::vector<std::string> tokens( begin, end );

        double value_cell;
        if( tokens[0] != comm_char ) {
            for( unsigned i = 0; i < tokens.size(); i++ ) {
                value_cell = std::stod( tokens[i] );
                if(res_bool) {
                    if( i == res_col ) {
                        value_cell *= res_val;
                    }
                }
                store[aux_row*col_size+i] = value_cell;
            }
        aux_row += 1;
        }
    }
    in.close();
    struct corr data = { store, row_size, col_size, time_extent };
    return data;
}
