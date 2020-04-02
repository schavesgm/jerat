#include "corr.hpp"
/*
   This file contains all the utils
*/

double* Corr::avg( struct matrix data ) {
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

double* Corr::var( struct matrix data, double* average ) {
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

struct matrix Corr::slice( struct matrix data, const unsigned col ) {
    /*
       Method to slice a matrix into a vector in a given column. 
       The initial matrix has dimensions [rows,cols], the resuling
       vector has dimenions [rows,1]
    */
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    double* hold = new double[rows];
    for ( unsigned nr = 0; nr < rows; nr++ ) {
        hold[nr] = data.data[nr * cols + col];
    }
    struct matrix ret = { hold, rows, 1, data.time_extent };
    return ret;
}
