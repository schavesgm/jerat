#include "fit.hpp"

double f( std::vector<double> params, double x, 
        std::vector<double> args ) {
    return params[0] * ( std::exp( - params[1] * x ) + \
        std::exp( - params[1] * ( args[0] - x ) ) );
}

std::vector<std::vector<double>> 
    fit_hierarchy( 
        Correlator& corr,
        std::vector<std::vector<unsigned>> w_start, 
        std::vector<std::vector<double>> in_params, 
        std::vector<std::vector<double>> in_explor, 
        unsigned n_boot 
    ) {


    // Calculate the bootstrap estimation of the correlators
    corr.bootstrap_central( n_boot );

    // Get number of temperatures in correlator object
    unsigned num_temp = corr.num_inputs;
    unsigned dim_param = in_params[0].size();

    // Vector to hold the results for this iteration
    std::vector<std::vector<double>> results( num_temp );

    // Auxiliary variables
    unsigned aux_hierarchy = num_temp;
    unsigned size_res;
    unsigned aux_temp;

    // Iterate trough temperatures
    for ( unsigned to = 0; to < num_temp; to++ ) {

        // Number of total windows in this temeperature
        unsigned w_size = w_start[to].size();

        // Fix number of files to allocate the data as expected
        aux_temp = 0;

        unsigned n_tau = corr.boots_central[to].time_extent;
        double arg_ntau = corr.boots_central[to].time_extent;
        std::vector<double> args = { arg_ntau };

        // Allocate space for the vector in this temperature
        size_res = \
            w_start[to].size() * aux_hierarchy * ( dim_param + 1);

        aux_hierarchy -= 1;

        // Reserve the space for this temperature and its subfiles
        results[to].reserve( size_res );
        results[to].resize( size_res );

        // Generate the input data in the correct form
        double* xy_to = new double[n_tau * 2];
        for ( unsigned nt = 0; nt < n_tau; nt++ ) {
            xy_to[nt * 2] = nt;
            xy_to[nt * 2 + 1] = \
                corr.boots_central[to].data[nt * 2];
            }
        Matrix data_to = { xy_to, n_tau, 2, n_tau };

        // Fit trough all the windows for the current temperature
        for ( unsigned wi = 0; wi < w_start[to].size(); wi++ ) {

            // Crop the data to the current window
            Matrix w_data = \
                select_window( data_to, w_start[to][wi] );

            // Fit the data to that window
            std::vector<double> window_fit = \
                fitNM( f, w_data, in_params[to], \
                    in_explor[to], args );

            // Feed the results into the result vector
            unsigned index;
            for ( unsigned in = 0; in < window_fit.size(); in++ ) {
                index = ( aux_temp * w_size + wi ) * \
                    window_fit.size() + in;
                results[to][index] = window_fit[in];
            }
            // Free the pointers
            delete[] w_data.data;

            // Free the vectors
            window_fit.clear();
        }

        // Increase in one the file we are allocating now
        aux_temp += 1;

        // Now fit trough all the other lower temperatures
        for ( unsigned tf = to + 1; tf < num_temp; tf++ ) {

            // Generate the proper value of n_tau for each temp
            unsigned n_tau = corr.boots_central[tf].time_extent;
            double arg_ntau = corr.boots_central[tf].time_extent;
            std::vector<double> args = { arg_ntau };

            // Generate the input data in the correct form
            double* xy_tf = new double[n_tau * 2];
            for ( unsigned nt = 0; nt < n_tau; nt++ ) {
                xy_tf[nt * 2] = nt;
                xy_tf[nt * 2 + 1] = \
                    corr.boots_central[tf].data[nt * 2];
                }
            Matrix data_tf = { xy_tf, n_tau, 2, n_tau };

            // Fit the same window for all lower temperatures 
            for ( unsigned wi = 0; wi < w_start[to].size(); wi++ ) {

            // Crop the data to the current window
            Matrix w_data = \
                select_window( data_tf, w_start[to][wi] );

            // Fit the data to that window
            std::vector<double> window_fit = \
                fitNM( f, w_data, in_params[tf], in_explor[tf], 
                        args );

            // Feed the results into the result vector
            unsigned index;
            for ( unsigned in = 0; in < window_fit.size(); in++ ) {
                index = ( aux_temp * w_size + wi ) * \
                        window_fit.size() + in;
                results[to][index] = window_fit[in];
            }
            // Free the pointers
            delete[] w_data.data;

            // Free the vectors
            window_fit.clear();
            }
            // Increase in one the file we are allocating now
            aux_temp += 1;

            // Free pointers in this scope
            delete[] data_tf.data;
        }

        // Free the pointers
        delete[] data_to.data;
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
