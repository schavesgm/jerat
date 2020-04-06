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
    if ( this->bool_covariance ) {
        this->free_matrix( this->cov_mat );
        this->free_matrix( this->tt_mat );
    }

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

 
void Correlator::cov_matrix( unsigned* t_min, unsigned* t_max, 
        unsigned nboot ) {

    this->bool_covariance = true;

    // Allocate the memory
    this->cov_mat = new Matrix[this->num_inputs];
    this->tt_mat = new Matrix[this->num_inputs];

    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {

        assert( t_max[ni] < this->RAW_DATA[ni].time_extent );
        assert( t_min[ni] < t_max[ni] );

        // We need to include the last point
        unsigned window = t_max[ni] - t_min[ni] + 1; 
        unsigned area = window * window;
        unsigned rows = this->RAW_DATA[ni].row_size;
        unsigned cols = this->RAW_DATA[ni].row_size;
        unsigned n_tau = this->RAW_DATA[ni].time_extent;
        unsigned n_configs = rows / n_tau;
        std::uniform_int_distribution<int> sampler(0, n_configs);
        
        // Slice the matrix
        unsigned shape[2] = { n_configs, n_tau };
        Matrix slice = this->reshape( this->RAW_DATA[ni], shape, 1 );

        // Auxiliary variables
        unsigned index, pick_nc, aux_t;

        // Results matrix
        double* cov_bootstrap = new double[nboot * area];

        for ( unsigned nb = 0; nb < nboot; nb++ ) {

            double* hold_resample = new double[n_configs * window];

            for ( unsigned nc = 0; nc < n_configs; nc++ ) {
                pick_nc = sampler(this->random_eng); 
                aux_t = t_min[ni];

                for ( unsigned nt = 0; nt < window; nt++ ) {
                    index = pick_nc * n_tau + aux_t;
                    hold_resample[nc * window + nt] = 
                        slice.data[index];
                    aux_t += 1;
                }
            }

            // Now calculate the covariance and store it
            Matrix resample = 
                { hold_resample, n_configs, window, n_tau };
            double* cov_sample = this->cov( resample );

            // Feed the data inside the whole matrix
            for ( unsigned nt = 0; nt < area; nt++ )
                cov_bootstrap[nb * area + nt] = cov_sample[nt];
        }

        // Get the average on the covariance estimations
        Matrix covariance = { cov_bootstrap, nboot, area, n_tau };
        double* best_cov = this->avg( covariance );
        double* best_tt = new double[window * window]{ 0.0 };

        // Generate the T matrix as the diagonal of C
        for ( unsigned i = 0; i < window * window; i++ ) {
            best_tt[i * window + i ] = best_cov[i * window + i];
        }

        for ( unsigned i = 0; i < window; i++ ) {
            for ( unsigned j = 0; j < window; j++ ) {
                std::cout << best_cov[i * window + j ] << " ";
            }
            std::cout << std::endl;
        }

        // Save the data
        this->cov_mat[ni] = { best_cov, window, window, n_tau };
        this->tt_mat[ni] = { best_tt, window, window, n_tau };

        // Free the space
        delete[] slice.data;
        delete[] cov_bootstrap;
    }
}

