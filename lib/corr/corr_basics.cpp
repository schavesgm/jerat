#include "corr.hpp"
/* 
   This file contains the basic definitions of the Corr class.
*/

Correlator::Correlator( struct Input* inputs, 
        unsigned num_inputs, unsigned seed ) {
    
    // Create a RAW pointer with the correct dimensions
    this->RAW_DATA = new Matrix[num_inputs];
    this->num_inputs = num_inputs;

    // // Load the data in file inside a pointer 
    for ( unsigned ni = 0; ni < num_inputs; ni++ ) {
        this->RAW_DATA[ni] = load_data( inputs[ni] );
    }

    // Set the seed
    this->seed = seed;
    this->random_eng.seed(this->seed);
}

Correlator::~Correlator() {
    // Destructor -- Free all the pointers
    
    this->free_matrix( this->RAW_DATA );
    if ( this->bool_central )
        this->free_matrix( this->central );
    if ( this->bool_sig2noise )
        this->free_matrix( this->sig2noise );
    if ( this->bool_bootstrap )
        this->free_matrix( this->boots_central );

    // Free tmax and tmin
    if ( this->bool_tmax )
        delete[] this->t_max;
    if ( this->bool_tmin )
        delete[] this->t_min;
}

void Correlator::central_value( const unsigned col ) {
    /* 
       Method to calculate the central value of the correlation
       function data coming from simulations.  The central value is
       defined as,

            \bar{C}(\tau) = 1/N_C \sum_i^{N_C} C_i(\tau)
        its error is defined as, 
            Var(\bar{C}(\tau)) = 1/N_C \sum_i^{N_C} (
                C_i(\tau) - \bar{C}(\tau )^2

    */
    // Let the code know you have the central value calculated
    this->bool_central = true;
    
    // Allocate the memory
    this->central = new Matrix[this->num_inputs];

    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {

        // Calculate the central value for each input
        unsigned rows = this->RAW_DATA[ni].row_size;
        unsigned cols = this->RAW_DATA[ni].col_size;
        unsigned n_tau = this->RAW_DATA[ni].time_extent;
        unsigned n_configs = rows / n_tau;

        unsigned shape[2] = { n_configs, n_tau };
        struct Matrix slice = 
            this->reshape( this->RAW_DATA[ni], shape, 1 );

        double* avg_corr = this->avg( slice );
        double* var_corr = this->var( slice, avg_corr );

        double* cent_ni = new double[n_tau * 2];

        // Fill a matrix with these values
        for ( unsigned nt = 0; nt < n_tau; nt++ ) {
            cent_ni[nt * 2 + 0] = avg_corr[nt];
            cent_ni[nt * 2 + 1] = var_corr[nt];
        }
        this->central[ni] = { cent_ni, n_tau, 2, n_tau };
        delete [] avg_corr;
        delete [] var_corr;
    }
}

void Correlator::bootstrap_central( const unsigned nboot, 
        const unsigned col ) {
    /*
       Method to generate a boostrap estimation of the correlation
       function.
    */
    // Let the code know you have calculated bootstrap

    this->bool_bootstrap = true;

    // Allocate the memory
    this->boots_central = new Matrix[this->num_inputs];

    // Auxiliary variables
    double* avg; 
    double* var;

    unsigned index, pick_nc;

    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {

        // Get the dimensions of the file
        unsigned rows = this->RAW_DATA[ni].row_size;
        unsigned cols = this->RAW_DATA[ni].col_size;
        unsigned n_tau = this->RAW_DATA[ni].time_extent;

        // unsigned n_configs = rows / n_tau;
        unsigned n_configs = 2;

        // Generate the sampler
        std::uniform_int_distribution<int> sampler(0, n_configs);

        // Reshape the matrix
        unsigned shape[2] = { n_configs, n_tau };
        struct Matrix slice = 
            this->reshape( this->RAW_DATA[ni], shape, 1 );

        // Pointer and structure to hold the samples
        double* hold_resample = new double[rows]{ 0.0 };
        struct Matrix resample;

        // Get the average and the variance for each sample
        double* hold_avg = new double[nboot * n_tau]{ 0.0 };
        double* hold_var = new double[nboot * n_tau]{ 0.0 };

        // Run the bootstrap
        for ( unsigned nb = 0; nb < nboot; nb++ ) {
            // Generate a bootstrap resample
            for ( unsigned nc = 0; nc < n_configs; nc++ ) {
                pick_nc = sampler(this->random_eng);
                for ( unsigned nt = 0; nt < n_tau; nt++ ) {
                    index = pick_nc * n_tau + nt;
                    hold_resample[nc * n_tau + nt] = 
                        slice.data[index];
                }
            }
            // Calculate the average of that resample
            resample = { hold_resample, n_configs, n_tau, n_tau };

            avg = this->avg( resample );
            var = this->var( resample, avg );
            
            // Fill hold_avg and hold_var with this estimations
            for ( unsigned nt = 0; nt < n_tau; nt++ ) {
                hold_avg[nb * n_tau + nt] = avg[nt];
                hold_var[nb * n_tau + nt] = var[nt];
            }
        }
        // Convert into Matrix 
        Matrix est_avg = { hold_avg, nboot, n_tau, n_tau };
        Matrix est_var = { hold_var, nboot, n_tau, n_tau };

        // Calculate the average on all the resamples
        double* best_avg = this->avg( est_avg );
        double* best_var = this->avg( est_var );

        // Fill a matrix with these values
        double* best_corr =  new double[n_tau * 2]{ 0.0 };
        for ( unsigned nt = 0; nt < n_tau; nt++ ) {
            best_corr[nt * 2 + 0] = best_avg[nt];
            best_corr[nt * 2 + 1] = best_var[nt];
        }

        // Set the matrix 
        this->boots_central[ni] = { best_corr, n_tau, 2, n_tau };

        // Free space
        delete [] avg;
        delete [] var;
        delete [] best_avg; 
        delete [] best_var;
        delete [] resample.data;
        delete [] slice.data;

    }
}

void Correlator::sig_to_noise() {
    /*
       Method to calculate signal to noise in a correlation function.
       The method implements,
            StN(C(\tau)) = \bar{C}(\tau) / sqrt(Var(C(\tau))
    */

    // Make sure the central value is calculated
    if ( !this->bool_central ) {
        this->central_value(1);
    }
    this->bool_sig2noise = true;

    // Allocate the memory
    this->sig2noise = new Matrix[this->num_inputs];

    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {

        unsigned n_tau = this->central[ni].time_extent;
        unsigned cols = this->central[ni].col_size;
        double* sig_noise = new double[n_tau];

        for( unsigned nt = 0; nt < n_tau; nt++ ) { 
            sig_noise[nt] = \
                this->central[ni].data[nt * cols + 0] / \
                std::sqrt( this->central[ni].data[nt * cols + 1] );
        }

        this->sig2noise[ni] = { sig_noise, n_tau, 1, n_tau };
    }
}

 
// void Corr::cov_matrix( unsigned tmin, unsigned tmax, 
//         unsigned nboot ) {
// 
//     // We need to include the last point
//     unsigned window = tmax - tmin + 1; 
//     unsigned rows = this->raw.row_size;
//     unsigned cols = this->raw.col_size;
//     unsigned n_tau = this->raw.time_extent;
//     unsigned n_configs = rows / n_tau;
// 
//     std::uniform_int_distribution<int> dist(0, n_configs);
// 
//     // Slice the matrix 
//     unsigned shape[2] = { n_configs, n_tau };
//     struct matrix get_col = this->reshape( this->raw, shape, 1 );
// 
//     // Pointer and structure to hold the samples
//     double* hold_sample = new double[rows];
//     struct matrix resamp;
// 
//     // Get the average and the variance for each sample
//     unsigned area = window * window;
//     double* hold_cov = new double[nboot * area];
// 
//     // Auxiliary variables
//     unsigned index, pick_nc, aux_t;
// 
//     // Pointers to hold the data
//     double* cov_sample;
// 
//     for ( unsigned nb = 0; nb < nboot; nb++ ) {
//         // Generate a bootstrap resample
//         for ( unsigned nc = 0; nc < n_configs; nc++ ) {
//             pick_nc = dist(this->random_eng);
//             aux_t = tmin;
//             for ( unsigned nt = 0; nt < window + 1; nt++ ) {
//                 index = pick_nc * n_tau + aux_t;
//                 hold_sample[nc * window + nt] = get_col.data[index];
//                 aux_t += 1;
//             }
//         }
//         // Calculate the average of that resample
//         resamp = { hold_sample, n_configs, window, n_tau };
//         cov_sample = this->cov( resamp );
// 
//         // Fill the resampled matrix with the covariance
//         for ( unsigned nt = 0; nt < area; nt++ ) {
//             hold_cov[nb * area + nt] = cov_sample[nt];
//         }
//     }
//     
//     struct matrix est_cov = { hold_cov, nboot, area, n_tau };
// 
//     // Calculate the average on all the resamples
//     double* best_cov = this->avg( est_cov );
// 
//     delete [] hold_sample;
//     delete [] cov_sample;
// 
//     this->covmat = { best_cov, window, window, n_tau };
// }

