#include <iostream>
#include <random>
#include <type_traits>

// Load headers 
#include "corr.hpp"
#include <math.h>
#include "fit.hpp"

double f( std::vector<double> params, double x ) {
    return params[0] * std::exp( - params[1] * x ) + \
        params[2] * std::exp( - params[3] * ( 48 - x ) );
}

int main() {

    // Read data from input file
    std::string input_f = "input.in";

    std::vector<std::string> file_const = \
        get_key( input_f, "file_const" );

    std::vector<std::string> n_tau = \
        get_key( input_f, "n_tau" );

    std::vector<std::string> n_space = \
        get_key( input_f, "n_space" );
    
    std::vector<std::string> str_rows = \
        get_key( input_f, "rows" );

    std::vector<std::string> str_cols = \
        get_key( input_f, "cols" );

    std::vector<std::string> str_boots = \
        get_key( input_f, "n_boot" );

    std::vector<std::string> files;
    std::string f_name;
    for ( unsigned nt = 0; nt < n_tau.size(); nt++ ) {
        f_name = file_const[0] + n_tau[nt] + "x" + n_space[0] + \
            file_const[1];
        files.push_back( f_name );
        std::cout << f_name << std::endl;
    }

    // Generate correlator data 
    unsigned num_files = files.size();
    Input ins[num_files];

    unsigned u_row, u_col, u_ntau;
    unsigned long aux_row, aux_col, aux_ntau;
    for ( unsigned nf = 0; nf < files.size(); nf++ ) {
        aux_row = std::stoul( str_rows[nf] );
        aux_col = std::stoul( str_cols[0] );
        aux_ntau = std::stoul( n_tau[nf] );
        u_row = 
            static_cast<std::make_unsigned<decltype(aux_row)>::type>
            (aux_row);
        u_col = 
            static_cast<std::make_unsigned<decltype(aux_col)>::type>
            (aux_col);
        u_ntau = 
            static_cast<std::make_unsigned<decltype(aux_ntau)>::type>
            (aux_ntau);
        ins[nf] = { files[nf].c_str(), u_row, u_col, u_ntau };
    }

    // Generate the object
    Correlator corr_data( ins, num_files, 123123 );

    // Generate a bootstrap estimation
    unsigned long aux_boots = std::stoul( str_boots[0] );
    unsigned n_boots = 
        static_cast<std::make_unsigned<decltype(aux_boots)>::type>
        (aux_boots);

    // Calculate the bootstrap iteration
    corr_data.bootstrap_central( n_boots );

    // Free vector data
    file_const.clear();
    n_tau.clear();
    n_space.clear();
    files.clear();

return 0;
}
