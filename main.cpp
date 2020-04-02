#include <iostream>

// Load headers 
#include "corr.hpp"

int main() {

    char file_name[255] = "Gen2l_48x32.meson.g5.cc";
    unsigned row_size = 106752; 
    unsigned col_size = 3;
    unsigned time_extent = 48;

    Corr corr( file_name, row_size, col_size, time_extent, 123 );

    // for( unsigned i = 0; i < row_size * col_size; i++ ) {
    //     std::cout << corr.raw.data[i] << std::endl;
    // }

    // corr.cent_corr( 1 );
    // corr.sig_to_noise();
    // for( unsigned i = 0; i < time_extent; i++ ) {
    //     std::cout << corr.stn.data[i] << std::endl;
    // }

    for ( unsigned i = 0; i < 20; i++ ) {
        corr.get_tmin( 5 );
        std::cout << corr.tmin << std::endl;
    }
    std::cout << corr.tmax << std::endl;

    //corr.get_tmax( 1.0, 0.75 * time_extent );
    // corr.get_tmin( 10 );
    // std::cout << corr.tmin << std::endl;
    return 0;
}
