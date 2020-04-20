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

double* Correlator::std( Matrix data, double* average ) {
    /* 
       Method to calculate the std deviation of some data given the
       average.
    */
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    double* std_data = new double[cols];
    double aux_std, value; unsigned index;

    for ( unsigned nc = 0; nc < cols; nc++ ) {
        aux_std = 0.0;
        for ( unsigned nr = 0; nr < rows; nr++ ) {
            index = ( nr * cols + nc );
            value = ( data.data[index] - average[nc] );
            aux_std += value * value;
        }
        std_data[nc] = std::sqrt( aux_std / ( rows - 1 ));
    }
    return std_data;
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

