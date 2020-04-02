#ifndef CORR_HPP
#define CORR_HPP

#include <cmath>

#include "defs.hpp"
#include "io_files.hpp"

class Corr {

    public:
        // Data
        struct corr raw;
        struct corr cent;
        struct corr stn;

        // Methods
        Corr( const char*, const unsigned, 
            const unsigned, const unsigned );
        
        // Generate the central value
        void cent_corr( const unsigned );

        // Calculate signal to noise
        void sig_to_noise();

        // Calculate t_max
        int get_tmax( double, double );

    private:
        bool calc_central = false;     
        bool calc_sig2noi = false;     

};

#endif
