#include <iostream>

// Load headers 
// #include "corr.hpp"
#include "io_files.hpp"

int main() {

    char file_name[255] = "Gen2l_48x32.meson.g5.cc";
    unsigned row_size = 106752; 
    unsigned col_size = 3;
    unsigned time_extent = 48;

    Input input = { file_name, row_size, col_size, time_extent };
    std::cout << input.file_name << " " << input.row_size << " " <<
        input.col_size << " " << input.time_extent << std::endl;
    std::cout << input.rescale << " " << input.col_rescale << " " <<
        input.res_factor << std::endl;

    // Corr best( file_name, row_size, col_size, time_extent );
    // best.boot_est();

    // Corr corr( file_name, row_size, col_size, time_extent, 123 );
    // corr.cov_matrix( 1, 2, 10 );
    // std::cout << corr.covmat.data[0] << " " <<
    //     corr.covmat.data[1] << " " << corr.covmat.data[2] << 
    //     " " << corr.covmat.data[3] << std::endl;
    // corr.cov_matrix( 1, 2, 100 );
    // std::cout << corr.covmat.data[0] << " " <<
    //     corr.covmat.data[1] << " " << corr.covmat.data[2] << 
    //     " " << corr.covmat.data[3] << std::endl;
    // corr.cov_matrix( 1, 2, 200 );
    // std::cout << corr.covmat.data[0] << " " <<
    //     corr.covmat.data[1] << " " << corr.covmat.data[2] << 
    //     " " << corr.covmat.data[3] << std::endl;
    // corr.cov_matrix( 1, 2, 1000 );
    // std::cout << corr.covmat.data[0] << " " <<
    //     corr.covmat.data[1] << " " << corr.covmat.data[2] << 
    //     " " << corr.covmat.data[3] << std::endl;
    // corr.cov_matrix( 0, 2, 10000 );
    // std::cout << corr.covmat.data[0] << std::endl;
    // corr.cov_matrix( 0, 2, 15000 );
    // std::cout << corr.covmat.data[0] << std::endl;
    // corr.cov_matrix( 0, 2, 20000 );
    // std::cout << corr.covmat.data[0] << std::endl;
    // for ( unsigned i = 0; i < 3; i++ ) {
    //     for ( unsigned j = 0; j < 3; j++ ) {
    //         std::cout << corr.covmat.data[i * 3 + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // for ( unsigned i = 0; i < 2; i++ ) {
    //     std::cout << corr.covmat.data[i * 2 + i] << " " <<
    //     corr.best_est.data[i * 2 + 1] << std::endl;
    // }


    // for( unsigned i = 0; i < time_extent; i++ ) {
    //     std::cout << corr.best_est.data[i * 2] << " " <<
    //         corr.best_est.data[i * 2 + 1] << std::endl;
    // }

    // corr.sig_to_noise();
    // for( unsigned i = 0; i < time_extent; i++ ) {
    //     std::cout << corr.stn.data[i] << std::endl;
    // }

    // for ( unsigned i = 0; i < 20; i++ ) {
    //     corr.get_tmin( 5 );
    //     std::cout << corr.tmin << std::endl;
    // }
    // std::cout << corr.tmax << std::endl;

    //corr.get_tmax( 1.0, 0.75 * time_extent );
    // corr.get_tmin( 10 );
    // std::cout << corr.tmin << std::endl;
    return 0;
}
