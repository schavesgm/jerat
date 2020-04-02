#ifndef CORR_HPP
#define CORR_HPP

#include <cmath>
#include <random>

#include "defs.hpp"
#include "io_files.hpp"

class Corr {

    public:
        // Data
        struct matrix raw;
        struct matrix best_est;
        struct matrix cent;
        struct matrix stn;
        struct matrix cov;

        unsigned tmax, tmin;

        // Utils
        unsigned seed;
        std::mt19937 random_eng;

        // Methods
        Corr( const char*, const unsigned, const unsigned, 
            const unsigned, unsigned = 12341234 );
        
        // Generate a bootstrap estimation
        void boot_est( const unsigned = 200, const unsigned = 1 );
        // Generate the central value
        void cent_corr( const unsigned = 1 );
        // Calculate signal to noise
        void sig_to_noise();
        // Calculate t_max
        void get_tmax( double, double );
        // Sample a t_min
        void get_tmin( unsigned ); 
        // Calculate the covariance matrix using bootstrap
        void cov_matrix( unsigned = 200 );

    private:
        bool calc_boots = false;
        bool calc_central = false;     
        bool calc_sig2noi = false;     
        bool calc_tmax = false;
        
        double* avg( struct matrix );
        double* var( struct matrix, double* );
        struct matrix reshape( 
            struct matrix, const unsigned[2], const unsigned = 1 );



};

#endif
