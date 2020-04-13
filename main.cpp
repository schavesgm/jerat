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

// Global variable to manage NT
unsigned NT; 
double f( std::vector<double> params, double x ) {
    return params[0] * ( std::exp( - params[1] * x ) + \
        std::exp( - params[1] * ( NT - x ) ) );
}

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
    }

    // Generate the object
    Correlator corr_data( ins, num_files, 123123123 );

    // Generate bootstrap and fit unsigneds
    unsigned long aux_boots = std::stoul( str_boots[0] );
    unsigned long aux_fits = std::stoul( str_fits[0] );
    unsigned n_boots = static_cast<std::make_unsigned<decltype(aux_boots)>::type> (aux_boots);
    unsigned n_fits = 
        static_cast<std::make_unsigned<decltype(aux_fits)>::type>
        (aux_fits);

    // Vectors used in the calculation
    std::vector<std::vector<unsigned>> t_start( num_files );
    std::vector<std::vector<double>> fit_results( num_files );
    std::vector<std::vector<double>> chisq_results( num_files );
    std::vector<std::vector<double>> in_params( num_files );
    std::vector<std::vector<double>> in_explor( num_files );

    // Populate the pointers and reserve space
    for ( unsigned ni = 0; ni < num_files; ni++ ) {

        // Create the folder to hold the results for each temperature
        std::string folder_nt = "./results/" + n_tau[ni];
        mkdir( folder_nt.c_str(), 0777 );

        // Get all possible initial times -- Create every time
        unsigned start = beg_window * un_n_tau[ni];
        unsigned end = 0.49 * un_n_tau[ni];

        // Populate the starting point vector
        for ( unsigned i = start; i < end; i++ )
            t_start[ni].push_back( i );

        // Reserve the space for the fit results and chi square
        fit_results[ni].reserve( 
            n_fits * t_start[ni].size() * dim_param );
        chisq_results[ni].reserve(
            n_fits * t_start[ni].size() );

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

    // Iterate trough number of fits
    for ( unsigned nf = 0; nf < n_fits; nf++ ) {

        // Calculate the bootstrap iteration for this iteration
        corr_data.bootstrap_central( n_boots );

        // Iterate trough all the files
        for ( unsigned ni = 0; ni < num_files; ni++ ) {

            // Set the correct NT value
            NT = un_n_tau[ni];

            // Generate the input data in the correct form
            double* data_xy = new double[un_n_tau[ni] * 2];
            for ( unsigned nt = 0; nt < un_n_tau[ni]; nt++ ) {
                data_xy[nt * 2] = nt;
                data_xy[nt * 2 + 1] = \
                    corr_data.boots_central[ni].data[nt * 2];
            }
            Matrix data = \
                { data_xy, un_n_tau[ni], 2, un_n_tau[ni] };

            // Iterate through all windows 
            for ( unsigned wi = 0; wi < t_start[ni].size(); wi++ ) {

                // Crop the data to the window
                Matrix window = \
                    select_window( data, t_start[ni][wi] );

                // Fit the data for the current file and window
                std::vector<double> slice_fit = \
                    fitNM( f, window, in_params[ni], in_explor[ni] );

                // Feed the data into the results vector
                int index;
                for ( unsigned id = 0; id < dim_param; id++ ) {
                    index = ( nf * t_start[ni].size() + wi ) * \
                        dim_param + id;
                    fit_results[ni][index] = slice_fit[id];
                }
                chisq_results[ni][nf * t_start[ni].size() + wi] = \
                    slice_fit[slice_fit.size() - 1];
                
                // Free the pointers
                delete[] window.data;

                // Free the vectors
                slice_fit.clear();
            }

            // Free the pointers
            delete[] data.data;
        }
    }

    // Calculate the average/stde of fits and flush things out
    for ( unsigned ni = 0; ni < num_files; ni++ ) {

        // Generate a copy of the fit vector
        std::vector<double> fits_nt( 
            n_fits * dim_param * t_start[ni].size() );
        for ( unsigned i = 0; i < fits_nt.size(); i++ )
            fits_nt[i] = fit_results[ni][i];

        // Generate a copy of the chi_square results
        std::vector<double> chisq_nt( 
            n_fits * t_start[ni].size() );
        for ( unsigned i = 0; i < chisq_nt.size(); i++ )
            chisq_nt[i] = chisq_results[ni][i];

        // Calculate the final results for the values
        std::vector<double> res_avg_std = avg_stde(fits_nt, n_fits);
        std::vector<double> res_chi_squ = avg_stde(chisq_nt, n_fits);

        // Flush the bootstrap estimation to a file
        std::string file_est_out;
        file_est_out = "./results/" + n_tau[ni] + "/b_central_" + \
            file_const[0] + n_tau[ni] + "x" + n_space[0] + \
            file_const[1] + "." + str_boots[0] + ".dat";
        write_matrix( file_est_out, corr_data.boots_central[ni] );

        // Flush the fitted parameters out 
        std::string file_fit_out;
        file_fit_out = "./results/" + n_tau[ni] + "/fit_" + \
            file_const[0] + n_tau[ni] + "x" + n_space[0] + \
            file_const[1] + "." + str_boots[0] + ".dat";

        write_vector( file_fit_out, res_avg_std, res_chi_squ, 
                t_start[ni][0] );

        // Free the vectors
        fits_nt.clear();
        chisq_nt.clear();
        res_avg_std.clear();
        res_chi_squ.clear();
    }

    // Hierarchy fit 
    for ( unsigned n_tau = 1; n_tau < un_n_tau.size(); n_tau++ )
        assert( un_n_tau[0] < un_n_tau[n_tau] );

    // Free the space for all the vectors of vectors
    for ( unsigned ni = 0; ni < num_files; ni++ ) {
        t_start[ni].clear();
        fit_results[ni].clear();
        in_params[ni].clear();
        in_explor[ni].clear();
    }
    t_start.clear();
    fit_results.clear();
    in_params.clear();
    in_explor.clear();

    // Free input vectors data
    file_const.clear();
    n_tau.clear();
    un_n_tau.clear();
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
