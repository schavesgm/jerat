#include "corr.hpp"

Corr::Corr( const char* file_name, const unsigned row_size, 
    const unsigned col_size, const unsigned time_extent ) {
    
    // Load the data from file
    this->raw = load_data( file_name, row_size, \
        col_size, time_extent );
}

void Corr::cent_corr( const unsigned col ) {
    /* 
       Function to calculate the central value of the 
    correlation function data coming from simulations.
    The central value is defined as,

        \bar{C}(\tau) = 1/N_C \sum_i^{N_C} C_i(\tau)
    its error is defined as,
        Var(\bar{C}(\tau)) = 1/N_C \sum_i^{N_C} 
                ( C_i(\tau) - \bar{C}(\tau )^2

    Arguments:
        data (struct corr):
            corr structure containing the raw data.
        col (unsigned):
            Column to calculate the central value.
    */

    unsigned rows = this->raw.row_size;
    unsigned cols = this->raw.col_size;
    unsigned n_tau = this->raw.time_extent;
    unsigned n_configs = rows / n_tau;

    double* central = new double[n_tau * 2];

    double aux_mean;
    unsigned index;
    // Calculate the central value of the correlation function
    for( unsigned nt = 0; nt < n_tau; nt++ ) {
        aux_mean = 0.0;
        for ( unsigned nc = 0; nc < n_configs; nc++ ) {
            index = ( nc * n_tau + nt ) * cols + col;
            aux_mean += this->raw.data[index];
        }
        central[nt * 2 + 0] = aux_mean / n_configs;
    }

    double aux_stde, value;
    // Now calculate its variance
    for( unsigned nt = 0; nt < n_tau; nt++ ) {
        aux_stde = 0.0;
        for ( unsigned nc = 0; nc < n_configs; nc++ ) {
            index = ( nc * n_tau + nt ) * cols + col;
            value = this->raw.data[index] - central[nt * 2 + 0];
            aux_stde += value * value;
        }
        central[nt * 2 + 1] = aux_stde / ( n_configs );
    }


    this->cent = { central, n_tau, 2, n_tau };
}
