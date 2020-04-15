#include <iostream>
#include <random>
#include <type_traits>
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>

// Load headers 
#include "corr.hpp"
#include <math.h>
#include "fit.hpp"

int main() {

    // Generate the results folder if it does not exists
    mkdir("results", 0777); // Where the results are stored

    // Read data from input file
    std::string input_f = "input.in";

    // Read the constant part of the file
    std::vector<std::string> file_const = \
        get_key( input_f, "file_const" );

    // Read the temperatures
    std::vector<std::string> n_tau = \
        get_key( input_f, "n_tau" );
    std::vector<unsigned> un_n_tau( n_tau.size() );

    // Read the spatial length
    std::vector<std::string> n_space = \
        get_key( input_f, "n_space" );
    
    // Read the number of rows
    std::vector<std::string> str_rows = \
        get_key( input_f, "rows" );
    std::vector<unsigned> un_rows( str_rows.size() );

    // Read the number of columns
    std::vector<std::string> str_cols = \
        get_key( input_f, "cols" );

    // Read number of bootstrap iterations
    std::vector<std::string> str_boots = \
        get_key( input_f, "n_boot" );

    // Read number of fits per file
    std::vector<std::string> str_fits = \
        get_key( input_f, "n_fit" );

    // Get number of input parameters
    std::vector<std::string> str_dim_param = \
        get_key( input_f, "dim_param" );
    unsigned dim_param = std::stoul( str_dim_param[0] );

    // Read input parameters
    std::vector<std::string> str_in_params = \
        get_key( input_f, "in_params" );

    // Read volume to explore in parameter space
    std::vector<std::string> str_in_explor = \
        get_key( input_f, "in_explor" );

    // Read initial percentage of window to fit
    std::vector<std::string> str_beg_window = \
        get_key( input_f, "beg_window" );
    double beg_window = std::stod( str_beg_window[0] );

    assert( str_in_explor.size() == str_in_explor.size() );

    std::vector<std::string> files;
    std::string f_name;
    for ( unsigned nt = 0; nt < n_tau.size(); nt++ ) {
        f_name = file_const[0] + n_tau[nt] + "x" + n_space[0] + \
            file_const[1];
        files.push_back( f_name );
    }

    // Generate correlator data 
    unsigned num_files = files.size();
    Input ins[num_files];

    unsigned u_row, u_col, u_ntau;
    unsigned long aux_row, aux_col, aux_ntau;
    for ( unsigned ni = 0; ni < files.size(); ni++ ) {
        aux_row = std::stoul( str_rows[ni] );
        aux_col = std::stoul( str_cols[0] );
        aux_ntau = std::stoul( n_tau[ni] );
        u_row = 
            static_cast<std::make_unsigned<decltype(aux_row)>::type>
            (aux_row);
        u_col = 
            static_cast<std::make_unsigned<decltype(aux_col)>::type>
            (aux_col);
        u_ntau = 
            static_cast<std::make_unsigned<decltype(aux_ntau)>::type>
            (aux_ntau);
        ins[ni] = { files[ni].c_str(), u_row, u_col, u_ntau };
        un_n_tau[ni] = aux_ntau;
        un_rows[ni] = aux_row;
    }

    // Generate the object
    Correlator corr_data( ins, num_files, 123123123 );

    // Generate bootstrap and fit unsigneds
    unsigned long aux_boots = std::stoul( str_boots[0] );
    unsigned long aux_fits = std::stoul( str_fits[0] );
    unsigned n_boots = 
        static_cast<std::make_unsigned<decltype(aux_boots)>::type> 
        (aux_boots);
    unsigned n_fits = 
        static_cast<std::make_unsigned<decltype(aux_fits)>::type>
        (aux_fits);


    // Hierarchy fit ----------------------------------------------

    // Check the order of the temperatures and rows
    for ( unsigned t = 0; t < un_n_tau.size() - 1; t++ ) {
        for ( unsigned tp = t + 1; tp < un_n_tau.size(); tp++ ) {
            assert( un_n_tau[t] < un_n_tau[tp] );
            assert( un_rows[t] < un_rows[tp] );
        }
    }

    // Vectors used in the calculation
    std::vector<std::vector<unsigned>> w_start( num_files );
    std::vector<std::vector<double>> in_params( num_files );
    std::vector<std::vector<double>> in_explor( num_files );
    std::vector<std::vector<double>> fit_results( num_files );

    // Populate the pointers and reserve space
    unsigned aux_hierarchy = num_files;
    unsigned size_res;
    for ( unsigned ni = 0; ni < num_files; ni++ ) {

        // Create the folder to hold the results for each temperature
        std::string folder_nt = "./results/" + n_tau[ni];
        mkdir( folder_nt.c_str(), 0777 );

        // Get all possible initial times -- Create every time
        unsigned start = beg_window * un_n_tau[ni];
        unsigned end = 0.49 * un_n_tau[ni];

        // Populate the starting point vector
        for ( unsigned i = start; i < end; i++ )
            w_start[ni].push_back( i );

        // Reserve the space for fit results and chisq_nt -- For all
        // temperatures smaller than the one we are using
        size_res = n_fits * w_start[ni].size() * \
            aux_hierarchy * ( dim_param  + 1 );

        fit_results[ni].reserve( size_res );
        fit_results[ni].resize( size_res );

        aux_hierarchy -= 1;

        // Allocate the input parameter vectors
        if ( dim_param == str_in_params.size() ) {

            // All the input values are the same
            for ( unsigned nd = 0; nd < dim_param; nd++ ) {
                in_params[ni].push_back(
                    std::stod( str_in_params[nd] ) );
                in_explor[ni].push_back(
                    std::stod( str_in_explor[nd] ) );
            }
        } else {
            assert( str_in_params.size() / dim_param == num_files );
            for ( unsigned nd = 0; nd < dim_param; nd++ ) {
                in_params[ni].push_back(
                    std::stod( str_in_params[ni * dim_param + nd] ));
                in_explor[ni].push_back(
                    std::stod( str_in_explor[ni * dim_param + nd] ));
            }
        }
    }

    // Hierarchy fit algorithm
    for ( unsigned nf = 0; nf < n_fits; nf++ ) {

        std::vector<std::vector<double>> iter_hier = 
            fit_hierarchy( corr_data, w_start, in_params, 
                    in_explor, n_boots );

        // Feed the data into the results vector
        unsigned index, sub_index, aux_files;
        unsigned aux_hier = num_files;

        for ( unsigned ni = 0; ni < num_files; ni++ ) {

            aux_files = 0;
            unsigned cols = w_start[ni].size() * ( dim_param + 1 );

            for ( unsigned in = 0; in < cols; in++ ) {
                index = ( nf * aux_hier + aux_files ) * cols + in;
                sub_index = aux_files * cols + in;
                fit_results[ni][index] = iter_hier[ni][sub_index];
            }
            aux_files += 1;

            for ( unsigned nj = ni + 1; nj < num_files; nj++ ) {
                for ( unsigned in = 0; in < cols; in++ ) {
                    index = 
                        ( nf * aux_hier + aux_files ) * cols + in;
                    sub_index = aux_files * cols + in;
                    fit_results[ni][index] = \
                        iter_hier[ni][sub_index];
                }
                aux_files += 1;
            }
            aux_hier -= 1;
        }

        // Free the vectors
        for ( unsigned i = 0; i < iter_hier.size(); i++ )
            iter_hier[i].clear();
        iter_hier.clear();
    }

    // Flush the results out in files and calcualte avg/stde
    unsigned aux_files, index;
    unsigned aux_hier = num_files;
    std::string file_est_out;
    std::string file_fit_out;

    for ( unsigned ni = 0; ni < num_files; ni++ ) {

        // Keep track of rotating files
        aux_files = 0;

        // Flush the correlation function estimate
        file_est_out = "./results/" + n_tau[ni] + "/b_central_" + \
            file_const[0] + n_tau[ni] + "x" + n_space[0] + \
            file_const[1] + "." + str_boots[0] + ".dat";
        write_matrix( file_est_out, corr_data.boots_central[ni] );

        // Generate the name for the fitting file for root ni
        std::string folder_nt = "./results/" + n_tau[ni];
        file_fit_out = folder_nt + "/fit_" + file_const[0] + \
            n_tau[ni] + "x" + n_space[0] + \
            file_const[1] + "." + str_boots[0] + ".dat";

        unsigned w_size = w_start[ni].size();

        // Select the data to process from results
        unsigned cols = w_size * ( dim_param + 1 );

        std::vector<double> slice_fits( n_fits * cols );
        std::vector<double> res_avgstd( cols );
        
        for ( unsigned nf = 0; nf < n_fits; nf++ ) {
            for ( unsigned in = 0; in < cols; in++ ) {
                index = (nf * aux_hier + aux_files) * cols + in;
                slice_fits[nf * cols + in] = fit_results[ni][index];
            }
        }
        aux_files += 1;

        // Generate the estimation for the ni values
        res_avgstd = avg_stde( slice_fits, n_fits );
        // Flush the data out into the files in folder ni
        write_vector( file_fit_out, res_avgstd, dim_param, \
            w_start[ni][0] );

        // Now the same for the subfiles
        for ( unsigned nj = ni + 1; nj < num_files; nj++ ) {

            // Generate the output file for the subfiles
            file_fit_out = folder_nt + "/fit_" + file_const[0] + \
                n_tau[nj] + "x" + n_space[0] + \
                file_const[1] + "." + str_boots[0] + ".dat";

            for ( unsigned nf = 0; nf < n_fits; nf++ ) {
                for ( unsigned in = 0; in < cols; in++ ) {
                    index = (nf * aux_hier + aux_files) * cols + in;
                    slice_fits[nf * cols + in] = 
                        fit_results[ni][index];
                }
            }

            // Generate the estimation for the nj values
            res_avgstd = avg_stde( slice_fits, n_fits );
            // Flush the data out into the files in folder ni
            write_vector( file_fit_out, res_avgstd, dim_param, \
                w_start[ni][0] );
        }

        slice_fits.clear();
        res_avgstd.clear();

        aux_hier -= 1;
    }

    // Free the space for all the vectors of vectors
    for ( unsigned ni = 0; ni < num_files; ni++ ) {
        w_start[ni].clear();
        fit_results[ni].clear();
        in_params[ni].clear();
        in_explor[ni].clear();
    }
    w_start.clear();
    fit_results.clear();
    in_params.clear();
    in_explor.clear();

    // Free input vectors data
    file_const.clear();
    n_tau.clear();
    un_n_tau.clear();
    un_rows.clear();
    n_space.clear();
    str_rows.clear();
    str_cols.clear();
    str_boots.clear();
    str_fits.clear();
    str_dim_param.clear();
    str_in_params.clear();
    str_in_explor.clear();
    str_beg_window.clear();

return 0;
}
