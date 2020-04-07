#include <iostream>

// Load headers 
#include "corr.hpp"

void trial( struct Input* ins, int num ) {
    for ( int ni = 0; ni < num; ni++ ) {
        std::cout << ins[ni].file_name << std::endl;
    }
}

int main() {

    unsigned num_files = 2;
    std::vector<std::string> FILES;
    FILES.push_back( "Gen2l_48x32.meson.g5.cc" );
    FILES.push_back( "Gen2l_48x32.meson.vec.cc" );

    unsigned ROWS[num_files] = { 106752, 3 * 106752 };
    unsigned col_size = 3;
    unsigned time_extent = 48;

    Input ins[num_files];
    for ( unsigned nf = 0; nf <  num_files; nf++ ) {
        ins[nf] = { FILES[nf].c_str(), ROWS[nf], col_size, 
            time_extent };
    }
    Correlator corr( ins, num_files );
    // corr.bootstrap_central();

    unsigned* tmin = new unsigned[num_files];
    unsigned* tmax = new unsigned[num_files];
    for ( unsigned i = 0; i < num_files; i++ ) {
        tmin[i] = 0;
        tmax[i] = 2;
    }

    corr.cov_matrix( tmin, tmax, 1 );
    std::cout << "----" << std::endl;
    corr.cov_matrix( tmin, tmax, 100 );
    // corr.cov_matrix( tmin, tmax, 100 );
    // corr.cov_matrix( 0, 0, 0, 2, 1000 );
    // corr.cov_matrix( 0, 0, 0, 2, 5000 );
    // corr.bootstrap_central();
    // corr.get_tmax( 1.0, 0.5 ); 
    // std::cout << corr.t_max[0] << " " << corr.t_max[1] << std::endl;
    // corr.get_tmax( 0.5, 0.5 ); 
    // std::cout << corr.t_max[0] << " " << corr.t_max[1] << std::endl;
    // corr.get_tmax( 0.1, 0.5 ); 
    // std::cout << corr.t_max[0] << " " << corr.t_max[1] << std::endl;

    // for ( unsigned i = 0; i < time_extent; i++ ) {
    //     std::cout << corr.boots_central[0].data[2 * i] <<
    //         std::endl;
    // }
    // std::cout << corr.boots_central[0].data[0] 
    //     << " " << corr.boots_central[0].data[1] 
    //     << std::endl;
    // std::cout << corr.boots_central[1].data[0] 
    //     << " " << corr.boots_central[1].data[1] 
    //     << std::endl;
    // std::cout << corr.boots_central[0].data[2] 
    //     << " " << corr.boots_central[0].data[3] 
    //     << std::endl;
    // std::cout << corr.boots_central[1].data[2] 
    //     << " " << corr.boots_central[1].data[3] 
    //     << std::endl;
    // std::cout << corr.boots_central[0].data[time_extent * 2 - 2] 
    //     << " " << corr.boots_central[0].data[time_extent * 2 - 1] 
    //     << std::endl;
    // std::cout << corr.boots_central[1].data[time_extent * 2 - 2] 
    //     << " " << corr.boots_central[1].data[time_extent * 2 - 1] 
    //     << std::endl;

    // std::cout << corr.sig2noise[1].data[0] << " " <<
    //     corr.sig2noise[1].data[1] << " " << 
    //     corr.sig2noise[1].data[time_extent - 1] << std::endl;
    // std::cout << corr.boots_central[1].data[0] << " " <<
    //     corr.boots_central[1].data[1] << std::endl;
    // std::cout << corr.boots_central[1].data[2] << " " <<
    //     corr.boots_central[1].data[3] << std::endl;
    // std::cout << corr.boots_central[1].data[time_extent * 2 - 2] << 
    //     " " << corr.boots_central[1].data[time_extent * 2 - 1] 
    //     << std::endl;


    // Define the input variables
    // Input ins = { FILES[0].c_str(), 120, 1, 128 };
    // const char file_g5[255] = "Gen2l_48x32.meson.g5.cc";
    // const char file_vec[255] = "Gen2l_48x32.meson.g5.cc";
    // std::cout << file_g5 << std::endl;
    // char** FILES = new char*[2];
    // for ( unsigned nf = 0; nf < 2; nf++ ) {
    //     FILES[nf] = new char[255];
    // }
    // FILES[0] = file_g5;
    // FILES[1] = file_vec;
    // std::cout << FILES[0] << " " << FILES[1] << std::endl;

    // unsigned row_size = 106752; 
    // unsigned col_size = 3;
    // unsigned time_extent = 48;

    // Input* input = new Input[2];
    // input[0] = { file_name, row_size, col_size, time_extent };
    // input[1] = { file_name, row_size, col_size, time_extent };

    // Correlator corrs( input, 2 );
    // std::cout << corrs.RAW_DATA[0].data[0] << std::endl;

    // for ( unsigned i = 0; i < 10; i++ ) {
    //     std::cout << CORRS.RAW_DATA[0].data[i] << " ";
    // }

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
