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

void Corr::sig_to_noise() {
    /*
       Method to calculate signal to noise in a correlation function.
       The method implements,
            StN(C(\tau)) = \bar{C}(\tau) / sqrt(Var(C(\tau))
    */

    // Make sure the central value is calculated
    this->cent_corr(1);

    unsigned n_tau = this->cent.time_extent;
    unsigned sep = this->cent.col_size;
    double* sig2noise = new double[n_tau];

    for( unsigned nt = 0; nt < n_tau; nt++ ) { 
        sig2noise[nt] = \
            this->cent.data[nt * sep + 0] / \
            std::sqrt( this->cent.data[nt * sep + 1] );
    }

    this->stn = { sig2noise, n_tau, 1, n_tau };
}
