#include "io_files.hpp"

struct Matrix load_data( Input input, std::string comm_char ) {
    /*
       Function to load data from file. The function is able to skip
       comments inside the file. Default comment character is #.
    */

    // Retrieve the data from the Input structure
    std::string file_name = input.file_name;
    unsigned row_size = input.row_size;
    unsigned col_size = input.col_size;
    unsigned time_extent = input.time_extent;
    bool rescale = input.rescale;
    unsigned col_rescale = input.col_rescale;
    double res_factor = input.res_factor;
    
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
                if(rescale) {
                    if( i == col_rescale ) {
                        value_cell *= res_factor;
                    }
                }
                store[aux_row*col_size+i] = value_cell;
            }
        aux_row += 1;
        }
    }
    in.close();
    struct Matrix data = { store, row_size, col_size, time_extent };
    return data;
}
