#ifndef CORR_HPP
#define CORR_HPP

#include <cmath>
#include <random>
#include <assert.h>

#include "defs.hpp"
#include "io_files.hpp"

class Correlator {

    public:
        // Data
        Matrix* RAW_DATA;
        Matrix* central;
        Matrix* boots_central;
        Matrix* sig2noise;
        Matrix* cov_mat;
        Matrix* tt_mat;

        unsigned* t_max;
        unsigned* t_min;
        
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
        void cov_matrix( unsigned*, unsigned*, unsigned = 200 );

        unsigned* get_tmax( double, double, bool = false );
        unsigned* get_tmin( unsigned ); 

        // void covariance( const unsigned, const unsigned,
        //         const unsigned = 200 );

    private:

        bool bool_central = false;
        bool bool_sig2noise = false;
        bool bool_bootstrap = false;
        bool bool_covariance = false;
        bool bool_tmax = false;
        bool bool_tmin = false;

        double* avg( Matrix );
        double* var( Matrix, double* );
        double* cov( Matrix );

        Matrix reshape( Matrix, const unsigned*, const unsigned = 1 );
        Matrix reduce( Matrix, const unsigned );
        

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

        // struct matrix reshape( 
        //     struct matrix, const unsigned[2], const unsigned = 1 );
};

#endif
