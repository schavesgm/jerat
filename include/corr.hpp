#ifndef CORR_HPP
#define CORR_HPP

#include <cmath>
#include <random>

#include "defs.hpp"
#include "io_files.hpp"

class Correlator {

    public:
        // Data
        Matrix* RAW_DATA;
        Matrix* central;
        Matrix* boots_central;
        Matrix* sig2noise;
        
        // Utils
        unsigned num_inputs;
        unsigned seed;
        std::mt19937 random_eng;

        Correlator( Input*, unsigned, unsigned = 123 );
        ~Correlator();
        void central_value( const unsigned = 1 );
        void bootstrap_central( const unsigned = 200, 
            const unsigned = 1 );
        void sig_to_noise();

    private:

        bool bool_central = false;
        bool bool_sig2noise = false;
        bool bool_bootstrap = false;

        double* avg( Matrix );
        double* var( Matrix, double* );

        struct Matrix reshape( 
            Matrix, const unsigned*, const unsigned = 1
        );

        void free_matrix( Matrix* );

        // struct matrix best_est;
        // struct matrix cent;
        // struct matrix stn;
        // struct matrix covmat;

        // unsigned tmax, tmin;


        // // Methods
        // Corr( const char*, const unsigned, const unsigned, 
        //     const unsigned, unsigned = 12341234 );
        // 
        // // Generate a bootstrap estimation
        // // Generate the central value
        // // Calculate signal to noise
        // void sig_to_noise();
        // // Calculate t_max
        // void get_tmax( double, double );
        // // Sample a t_min
        // void get_tmin( unsigned ); 
        // // Calculate the covariance matrix using bootstrap
        // void cov_matrix( unsigned, unsigned, unsigned = 200 );

    // private:
        // bool calc_boots = false;
        // bool calc_central = false;     
        // bool calc_sig2noi = false;     
        // bool calc_tmax = false;
        // 
        // // Mean value of data
        // double* avg( struct matrix );
        // // Variance of data
        // double* var( struct matrix, double* );
        // // Covariance of data
        // double* cov( struct matrix );

        // struct matrix reshape( 
        //     struct matrix, const unsigned[2], const unsigned = 1 );
};

#endif
