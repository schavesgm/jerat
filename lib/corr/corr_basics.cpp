#include "corr.hpp"
/* 
   This file contains the basic definitions of the Corr class.
*/

Corr::Corr( const char* file_name, const unsigned row_size, 
    const unsigned col_size, const unsigned time_extent,
    unsigned seed) {
    
    // Load the data from file
    this->raw = load_data( file_name, row_size, \
        col_size, time_extent );

    // Set the seed
    this->random_eng.seed(seed);
}

void Corr::boot_est( const unsigned nboot, const unsigned col ) {
    /*
       Method to generate a boostrap estimation of the correlation
       function.
    */
    this->calc_boots = true;

    unsigned rows = this->raw.row_size;
    unsigned cols = this->raw.col_size;
    unsigned n_tau = this->raw.time_extent;
    unsigned n_configs = rows / n_tau;

    std::uniform_int_distribution<int> dist(0, n_configs);

    // Slice the matrix 
    unsigned shape[2] = { n_configs, n_tau };
    struct matrix get_col = this->reshape( this->raw, shape, 1 );

    // Pointer and structure to hold the samples
    double* hold_sample = new double[rows];
    struct matrix resamp;

    // Get the average and the variance for each sample
    double* hold_avg = new double[nboot * n_tau];
    double* hold_var = new double[nboot * n_tau];

    // Auxiliary variables
    unsigned index, pick_nc;
    double* avg;
    double* var;

    for ( unsigned nb = 0; nb < nboot; nb++ ) {
        // Generate a bootstrap resample
        for ( unsigned nc = 0; nc < n_configs; nc++ ) {
            pick_nc = dist(this->random_eng);
            for ( unsigned nt = 0; nt < n_tau; nt++ ) {
                index = pick_nc * n_tau + nt;
                hold_sample[nc * n_tau + nt] = get_col.data[index];
            }
        }
        // Calculate the average of that resample
        resamp = { hold_sample, n_configs, n_tau, n_tau };

        avg = this->avg( resamp );
        var = this->var( resamp, avg );

        // Fill hold_avg and hold_var with this estimations
        for ( unsigned nt = 0; nt < n_tau; nt++ ) {
            hold_avg[nb * n_tau + nt] = avg[nt];
            hold_var[nb * n_tau + nt] = var[nt];
        }
    }
    
    struct matrix est_avg = { hold_avg, nboot, n_tau, n_tau };
    struct matrix est_var = { hold_var, nboot, n_tau, n_tau };

    // Calculate the average on all the resamples
    double* best_avg = this->avg( est_avg );
    double* best_var = this->avg( est_var );

    // Fill a matrix with these values
    double* best_corr =  new double[n_tau * 2];
    
    for ( unsigned nt = 0; nt < n_tau; nt++ ) {
        best_corr[nt * 2 + 0] = best_avg[nt];
        best_corr[nt * 2 + 1] = best_var[nt];
    }

    delete [] hold_sample;
    delete [] hold_avg;
    delete [] hold_var;

    this->best_est = { best_corr, n_tau, 2, n_tau };
}

void Corr::cent_corr( const unsigned col ) {
    /* 
       Method to calculate the central value of the correlation
       function data coming from simulations.  The central value is
       defined as,

            \bar{C}(\tau) = 1/N_C \sum_i^{N_C} C_i(\tau)
        its error is defined as, 
            Var(\bar{C}(\tau)) = 1/N_C \sum_i^{N_C} (
                C_i(\tau) - \bar{C}(\tau )^2

    */
    this->calc_central = true;

    unsigned rows = this->raw.row_size;
    unsigned cols = this->raw.col_size;
    unsigned n_tau = this->raw.time_extent;
    unsigned n_configs = rows / n_tau;

    // This is ok
    unsigned shape[2] = { n_configs, n_tau };
    struct matrix get_col = this->reshape( this->raw, shape );

    double* avg_corr = this->avg( get_col );
    double* var_corr = this->var( get_col, avg_corr );

    double* central = new double[n_tau * 2];

    // Fill a matrix with these values
    for ( unsigned nt = 0; nt < n_tau; nt++ ) {
        central[nt * 2 + 0] = avg_corr[nt];
        central[nt * 2 + 1] = var_corr[nt];
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


