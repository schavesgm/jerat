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

        // Utils
        unsigned num_inputs;
        unsigned seed;
        std::mt19937 random_eng;

        Correlator( Input*, unsigned, unsigned = 123 );
        ~Correlator();
        void central_value( const unsigned = 1 );
        void bootstrap_central( const unsigned = 200, 
            const unsigned = 1 );

    private:

        bool bool_central = false;
        bool bool_bootstrap = false;

        double* avg( Matrix );
        double* std( Matrix, double* );

        Matrix reshape( Matrix, const unsigned*, const unsigned = 1 );

        void free_matrix( Matrix* );
};

#endif
