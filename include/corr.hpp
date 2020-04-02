#ifndef CORR_HPP
#define CORR_HPP

#include <cmath>
#include <random>

#include "defs.hpp"
#include "io_files.hpp"

class Corr {

    public:
        // Data
        struct corr raw;
        struct corr cent;
        struct corr stn;

        unsigned tmax, tmin;

        // Utils
        unsigned seed;
        std::mt19937 sample_tmin;

        // Methods
        Corr( const char*, const unsigned, const unsigned, 
            const unsigned, unsigned = 12341234 );
        
        // Generate the central value
        void cent_corr( const unsigned );

        // Calculate signal to noise
        void sig_to_noise();

        // Calculate t_max
        void get_tmax( double, double );

        // Sample a t_min
        void get_tmin( unsigned ); 

    private:
        bool calc_central = false;     
        bool calc_sig2noi = false;     
        bool calc_tmax = false;

};

#endif
