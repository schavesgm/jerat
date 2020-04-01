#include <iostream>

// Load headers 
#include "include/io_files.h" // IO management

int main() {

    char file_name[255] = "Gen2l_48x32.meson.g5.cc";
    unsigned row_size = 106752; 
    unsigned col_size = 3;

    struct matrix data = load_data( file_name, row_size, col_size );

    for( unsigned i = 0; i < 3 * data.col_size; i++ ) {
        std::cout << data.data[i] << std::endl;
    }

}
