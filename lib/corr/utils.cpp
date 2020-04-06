#include "corr.hpp"
/*
   This file contains all the utils
*/

void Correlator::free_matrix( Matrix* matrix ) {
    
    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {
        delete[] matrix[ni].data;
    }
    delete matrix;
}

double* Correlator::avg( Matrix data ) {
    /*
       Calculate the average of data in the column axis.
    */
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    double aux_mean;
    unsigned index;

    double* avg_data = new double[cols];
    for ( unsigned nc = 0; nc < cols; nc++ ) {
        aux_mean = 0.0;
        for ( unsigned nr = 0; nr < rows; nr++ ) {
            index = ( nr * cols + nc );
            aux_mean += data.data[index];
        }
        avg_data[nc] = aux_mean / rows;
    }
    return avg_data;
}

double* Correlator::var( Matrix data, double* average ) {
    /* 
       Method to calculate the variance of some data given the
       average.
    */
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    double* var_data = new double[cols];
    double aux_var, value; unsigned index;

    for ( unsigned nc = 0; nc < cols; nc++ ) {
        aux_var = 0.0;
        for ( unsigned nr = 0; nr < rows; nr++ ) {
            index = ( nr * cols + nc );
            value = ( data.data[index] - average[nc] );
            aux_var += value * value;
        }
        var_data[nc] = aux_var / rows;
    }
    return var_data;
}

double* Correlator::cov( Matrix data ) {

    // Get the dimensions of the data
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    // Calculate the average of the sample
    double* avg = this->avg( data );

    double aux_cov, value_1, value_2;
    double* cov = new double[cols * cols];

    for ( unsigned t = 0; t < cols; t++ ) {
        for ( unsigned tp = 0; tp < cols; tp++ ) {
            aux_cov = 0.0;
            for ( unsigned nr = 0; nr < rows; nr++ ) {
                value_1 = ( data.data[nr * cols + t] - avg[t] );
                value_2 = ( data.data[nr * cols + tp] - avg[tp] );
                aux_cov += value_1 * value_2;
            }
            cov[t * cols + tp] = aux_cov / rows;
        }
    }
    return cov;
}

Matrix Correlator::reshape( 
    Matrix data, const unsigned shape[2], const unsigned col) {
    /*
       Method to slice a matrix into a vector in a given column. 
       The initial matrix has dimensions [rows,cols], the resuling
       vector has dimenions [rows,1]
    */
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    unsigned fin_rows = shape[0];
    unsigned fin_cols = shape[1];

    double* hold = new double[rows];
    for ( unsigned nr = 0; nr < rows; nr++ ) {
        hold[nr] = data.data[nr * cols + col];
    }

    struct Matrix ret = \
        { hold, fin_rows, fin_cols, data.time_extent };
    return ret;
}

Matrix Correlator::reduce( Matrix data, const unsigned nf ) {
    /*
       Method to reduce the dimensionality of a Matrix structure.
       The method reduces the initial row_size into nf by calculating
       the average every chunk row_size / nf.
    */
    unsigned cols = data.col_size;
    unsigned no = data.row_size;
    unsigned n_tau = data.time_extent;

    assert( no > nf );

    unsigned num_chunks = no / nf;
    unsigned beg, end, index, aux_start;

    double* reduced_data = new double[nf * cols];

    for ( unsigned nc = 0; nc < nf; nc++ ) {

        double* data_chunk = new double[num_chunks * cols];

        beg = num_chunks * nc;
        end = num_chunks * ( nc + 1 );
        aux_start = 0;

        for ( unsigned ni = beg; ni < end; ni++ ) {
            for ( unsigned nt = 0; nt < cols; nt++ ) {
                index = ni * cols + nt;
                data_chunk[aux_start] = data.data[index];
                aux_start += 1;
            }
        }

        Matrix chunk = {data_chunk, num_chunks, cols, n_tau};
        double* avg = this->avg( chunk );

        // Feed the average into the reduced data
        for ( unsigned nt = 0; nt < cols; nt++ )
            reduced_data[nc * cols + nt] = avg[nt];

        // Free unwanted space
        delete[] data_chunk;
        delete[] avg;
    }

    // Generate the output data 
    Matrix reduced = { reduced_data, nf, cols, n_tau };
    return reduced;
}
