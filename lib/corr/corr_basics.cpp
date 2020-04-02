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

// void Corr::boot_est( const unsigned nboot, const unsigned col ) {
//     /*
//        Method to generate a bootstrap estimation of the correlation
//        function.
//     */
//     this->calc_boots = true;
// 
//     unsigned rows = this->raw.row_size;
//     unsigned cols = this->raw.col_size;
//     unsigned n_tau = this->raw.time_extent;
//     unsigned n_configs = rows / n_tau;
// 
//     std::uniform_int_distribution<double> dist(0, n_configs);
// 
//     // Pointer to hold the samples
//     double* hold_sample = new double[rows];
// 
//     // Get the average and the variance for each sample
//     double* hold_avg = new double[nboot * n_tau];
//     double* hold_var = new double[nboot * n_tau];
//     
//     // Pointer of results
//     double* best_avg = new double[n_tau];
//     double* best_var = new double[n_tau];
// 
//     unsigned pich_nc, index; 
//     double aux_mean, aux_var;
// 
//     for ( unsigned nb = 0; nb < nboot; nb++ ) {
//         // Generate a bootstrap resample
//         for ( unsigned nc = 0; nc < n_configs; nc++ ) {
//             pick_nc = dist(this->random_eng);
//             for ( unsigned nt = 0; nt < n_tau; nt++ ) {
//                 index = ( pick_nc * n_tau + nt ) * cols + col;
//                 hold_sample[nc * n_tau + nt] = 
//                     this->raw.data[index];
//             }
//         }
//         // Calculate the mean estimation for that resample
//         for ( unsigned nt = 0; nt < n_tau; nt++ ) {
//             aux_mean = 0.0;
//             for ( unsigned nc = 0; nc < n_configs; nc++ ) {
//                 index = ( nc * n_tau + nt );
//                 aux_mean += hold_sample[index];
//             }
//             hold_avg[nb * n_tau + nt] = aux_mean / n_configs;
//         }
// 
//         // Calculate the variance estimation for that resample
//         for ( unsigned nt = 0; nt < n_tau; nt++ ) {
//             aux_var = 0.0;
//             for ( unsigned nc = 0; nc < n_configs; nc++ ) {
//                 aux_var += \
//                     ( hold_sample[nc * n_tau + nt] - \
//                     hold_avg[nb * n_tau + nt] );
//             }
//             hold_var[nb * n_tau + nt] = aux_var / n_configs;
//         }
//     }
// 
//     // // Now calcualte the average value of the estimator and variance
//     // for ( unsigned nt = 0; nt < n_tau; nt++ ) {
//     //     aux_var = 0.0;
//     //     aux_mean = 0.0;
//     //     for ( unsigned nb = 0; nb < nboot; nb++ ) {
//     //     }
//     // }
// }

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

    struct matrix get_col = this->slice( this->raw, col );
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


