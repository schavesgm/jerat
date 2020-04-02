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
    this->calc_central = true;

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
    if ( !this->calc_central ) {
        this->cent_corr(1);
    }

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

int Corr::get_tmax( double tol_noise, double tol_temp ) {
    /* 
       Method to calculate the t_max to fit the data for a given
       correlation function. The algorithm to decide which t_max
       to select is given by,
       t_max = min( 
                1 / stn(G(t+a) >= tol_noise, 
                \bar{G}(t+a) < 0, t + a > tol_temp 
            )
    */ 
    int tmax = raw.time_extent - 1;

    // Check if needed data is calculated --> Get it if not
    if ( !this->calc_central )
        this->cent_corr( 1 );
    if ( !this->calc_sig2noi )
        this->sig_to_noise();

    // Choose the time
    bool first_cond, sec_cond, third_cond;
    for ( unsigned nt = 0; nt < stn.time_extent - 1; nt++ ) {

        // Check the first condition
        ( 1 / this->stn.data[nt+1] ) > tol_noise \
            ? first_cond = true : first_cond = false;

        // Check second condition
        this->cent.data[nt+1] < 0 \
            ? sec_cond = true : sec_cond = false;

        // Check third condition
        nt + 1 > tol_temp ? third_cond = true : third_cond = false;

        if ( first_cond || sec_cond || third_cond ) {
            tmax = nt;
            break;
        }
    }
    return tmax;
}
