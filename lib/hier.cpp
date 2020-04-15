#include "fit.hpp"

double f( std::vector<double> params, double x, 
        std::vector<double> args ) {
    return params[0] * ( std::exp( - params[1] * x ) + \
        std::exp( - params[1] * ( args[0] - x ) ) );
}

std::vector<std::vector<double>> 
    fit_hierarchy( 
        Correlator& corr,
        std::vector<std::vector<unsigned>> wi_final, 
        std::vector<unsigned> wi_start, 
        std::vector<std::vector<double>> in_params, 
        std::vector<std::vector<double>> in_explor, 
        unsigned n_boot 
    ) {

    // Calculate the bootstrap estimation of the correlators
    corr.bootstrap_central( n_boot );

    // Get number of temperatures in correlator object
    unsigned num_files = corr.num_inputs;
    unsigned dim_param = in_params[0].size();

    // Vector to hold the results for this iteration
    std::vector<std::vector<double>> results( num_files );

    // Auxiliary variables
    unsigned aux_hierarchy = num_files;
    unsigned size_to, aux_files;

    // Iterate through main files 
    for ( unsigned to = 0; to < num_files; to++ ) {

        // Number of total windows in this file
        unsigned num_windows = wi_final[to].size();

        // Allocate the space for the results
        unsigned size_to = 
            num_windows * aux_hierarchy * ( dim_param + 1);
        results[to].reserve( size_to );
        results[to].resize( size_to );

        aux_hierarchy -= 1;
        aux_files = 0;

        // Change the value of Nt in the fit by using arguments
        unsigned n_tau =
                corr.boots_central[to].time_extent;
        double arg_ntau = 
            corr.boots_central[to].time_extent;
        std::vector<double> args = { arg_ntau };

        // Iterate through all the windows in this file
        for ( unsigned nw = 0; nw < num_windows; nw++ ) {

            // Crop the bootstrap estimation to the window
            Matrix w_data = select_window( 
                corr.boots_central[to], 
                wi_start[to], wi_final[to][nw] );
            
            std::vector<double> w_fit = \
                fitNM( f, w_data, in_params[to], \
                in_explor[to], args );

            // Feed the data into the vector
            unsigned index;
            for ( unsigned in = 0; in < w_fit.size(); in++ ) {
                index = ( aux_files * num_windows + nw ) * \
                    w_fit.size() + in;
                results[to][index] = w_fit[in];
            }
            delete[] w_data.data;
        }

        // Keep track of correct allocation of each subfile
        aux_files += 1;

        // Now fit all the subfiles
        for ( unsigned tf = to + 1; tf < num_files; tf++ ) {

            // Change the value of Nt in the fit by using arguments
            unsigned n_tau =
                    corr.boots_central[tf].time_extent;
            double arg_ntau = 
                corr.boots_central[tf].time_extent;
            args = {  arg_ntau };

            // Iterate trough all the windows
            for ( unsigned nw = 0; nw < num_windows; nw++ ) {

                // Crop the bootstrap estimation to the window
                Matrix w_data = select_window( 
                    corr.boots_central[tf], 
                    wi_start[to], wi_final[to][nw] );

                std::vector<double> w_fit = \
                    fitNM( f, w_data, in_params[tf], \
                    in_explor[tf], args );

                // Feed the data into the vector
                unsigned index;
                for ( unsigned in = 0; in < w_fit.size(); in++ ) {
                    index = ( aux_files * num_windows + nw ) * \
                        w_fit.size() + in;
                    results[to][index] = w_fit[in];
                }
                delete[] w_data.data;
            }

            aux_files += 1;
        }
        args.clear();
    }
    return results;
}

/* --------------------------------------------------------------- */
std::vector<double> avg_stde( std::vector<double> data, 
        unsigned n_fits ) {

    // Dimensions of the data unrolled
    unsigned rows = n_fits;
    unsigned cols = data.size() / rows;

    std::vector<double> res_avg_std( 2 * cols );

    // Calculate the average on the data by column
    double aux_avg;
    for ( unsigned nc = 0; nc < cols; nc++ ) {
        aux_avg = 0.0;
        for ( unsigned nr = 0; nr < rows; nr++ ) {
            aux_avg += data[nr * cols + nc];
        }
        res_avg_std[nc * 2] = aux_avg / rows;
    }

    // Calculate the standard deviation on the data by column
    double aux_std, value;
    for ( unsigned nc = 0; nc < cols; nc++ ) {
        aux_std = 0.0;
        for ( unsigned nr = 0; nr < rows; nr++ ) {
            value = data[nr * cols + nc] - res_avg_std[nc * 2];
            aux_std += value * value;
        }
        res_avg_std[nc * 2 + 1] = \
            std::sqrt( aux_std / ( rows - 1 ) );
    }

    return res_avg_std;
}
