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

        // Methods
        Corr( const char*, const unsigned, 
            const unsigned, const unsigned );
        
        // Generate the central value
        void cent_corr( const unsigned );

};

#endif
