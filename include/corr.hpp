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
};

#endif
