#include "fit.hpp"

double chi_square( func f, std::vector<double> params, Matrix data ) {

    try {
        double trial = f( params, 0.0 );
    } catch (std::exception& err) {
        std::cout << 
            "The function provided must take as arguments: " <<
            "( std::vector<double>,  double ) and returns double."
            << std::endl;
    }

    // assert( std::is_same<double, f(params, 0.0)>::value == true );
    unsigned rows = data.row_size;
    unsigned cols = data.col_size;

    double chi_sq = 0.0;
    double aux = 0.0;

    for( unsigned i = 0; i < rows; i++ ) {
        aux = f( params, data.data[i * cols] ) - \
            data.data[i * cols + 1]; 
        chi_sq += std::pow( aux, 2 );
    }
    return chi_sq / rows;
}

std::vector<double> fitNM( func f, Matrix data, 
    std::vector<double> start_guess, std::vector<double> expl_vol,
    const unsigned max_iter, const unsigned seed ) {

    // Define the constants of the algorithm
    double alpha = 1.0;
    double gamma = 2.0;
    double rho   = 0.5;
    double sigma = 0.5;

    // Declare constant of the problem
    unsigned dim_param = start_guess.size();
    unsigned dim_simp = start_guess.size() + 1;
    double non_improvement_thresh = 1e-8;
    unsigned non_improvement_maxiters = 500;
    double thresh_zero = 1e-3;  // Avoid zero values at start

    // Allocate memory for needed vectors
    std::vector<double> centroid( dim_param );

    // Reflection data
    std::vector<double> refl_point( dim_param );
    double image_refl;
    
    // Expansion
    std::vector<double> expa_point( dim_param );
    double image_expa;

    // Contraction
    std::vector<double> cont_point( dim_param );
    double image_cont;

    // Allocate memory for the simplex
    std::vector<double> simplex( dim_param * dim_simp );
    std::vector<double> min_points( dim_param );

    // Allocate memory for the images of the simplex
    std::vector<double> images_s( dim_simp );

    // Initial value of the chi square
    double image_f = chi_square( f, start_guess, data );

    // Generate the random engine and sampler
    std::mt19937 rand_eng;
    rand_eng.seed( seed );
    std::uniform_real_distribution<double> dist(-1,1);

    // Generate the initial simplex using the values given
    for ( unsigned i = 0; i < dim_simp; i++ ) {
        if ( i == 0 ) {
            for ( unsigned j = 0; j < dim_param; j++ ) {
                simplex[i * dim_param + j] = start_guess[j];
            }
        }
        else {
            for ( unsigned j = 0; j < dim_param; j++ ) {
                double rand = dist(rand_eng);
                simplex[i * dim_param + j] = start_guess[j] + \
                    rand * ( expl_vol[j] ) + thresh_zero;
            }
        }
    }

    unsigned iter_noimprov = 0;
    unsigned iter_step = 0;

    double prev_min, curr_min;

    // Start the algorithm
    while ( true ) {


        // Calculate the image of the simplex
        for ( unsigned i = 0; i < dim_simp; i++ ) {
            std::vector<double> slice( 
                simplex.begin() + i * dim_param, 
                simplex.begin() + i * dim_param + dim_param );
            images_s[i] = chi_square( f, slice, data );
        }

        // Order the simplex according to the images
        curr_min = images_s[0];
        order_simplex( simplex, images_s );
        prev_min = images_s[0];

        // for ( unsigned i = 0; i < dim_simp; i++ )
        //     std::cout << images_s[i] << " ";
        // std::cout << std::endl;

        // Control the status of the algorithm
        if ( iter_step > max_iter ) {
            min_points.assign( 
                simplex.begin(), simplex.begin() + dim_param );
            min_points.push_back( images_s[0] );
            break;
        }

        // Keep track of non-improvement iterations
        if ( abs( prev_min - curr_min ) < non_improvement_thresh ) {
            iter_noimprov += 1;
        } else {
            iter_noimprov = 0;
        }

        if ( iter_noimprov >= non_improvement_maxiters ) {
            min_points.assign( 
                simplex.begin(), simplex.begin() + dim_param );
            min_points.push_back( images_s[0] );
            break;
        }
        iter_step += 1;

        // Calculate the centroid of the simplex
        centroid = get_centroid( simplex, dim_param );

        // Reflection part r[i] = c[i] + a * ( c[i] - s_wrs[i] 
        for ( unsigned i = 0; i < dim_param; i++ ) {
            refl_point[i] = centroid[i] + alpha * \
                ( centroid[i] - \
                    simplex[(dim_simp - 1) * dim_param + i] );
        }
        image_refl = chi_square( f, refl_point, data );

        if ( images_s[0] <= image_refl && \
            image_refl < images_s[dim_simp-1] ) {
            // Replace the worst point with the reflection
            for ( unsigned i = 0; i < dim_param; i++ ) {
                simplex[(dim_simp-1) * dim_param + i] = \
                    refl_point[i];
            }
            continue;
        } else if ( image_refl < images_s[0] ) {
            // Expasion part e[i] = c[i] + g * ( r[i] - c[i] )
            for ( unsigned i = 0; i < dim_param; i++ ) {
                expa_point[i] = centroid[i] + gamma * \
                    ( refl_point[i] - centroid[i] );
            }
            image_expa = chi_square( f, expa_point, data );

            if ( image_expa < image_refl ) {
                // Replace the worst point with the expansion
                for ( unsigned i = 0; i < dim_param; i++ ) {
                    simplex[(dim_simp-1) * dim_param + i] = \
                        expa_point[i];
                }
                continue;
            } else {
                // Replace the worst point with the reflection
                for ( unsigned i = 0; i < dim_param; i++ ) {
                    simplex[(dim_simp-1) * dim_param + i] = \
                        refl_point[i];
                }
                continue;
            }
        } else {
            // Contraction part x[i] = c[i] + r * ( s_wrs[i] - c[i] )
            for ( unsigned i = 0; i < dim_param; i++ ) {
                cont_point[i] = centroid[i] + rho * \
                    ( simplex[(dim_simp-1) * dim_param + i] -
                    centroid[i] );
            }
            image_cont = chi_square( f, cont_point, data );
            if ( image_cont < images_s[dim_simp] ) {
                // Replace the worst point with the expansion
                for ( unsigned i = 0; i < dim_param; i++ ) {
                    simplex[(dim_simp-1) * dim_param + i] = \
                        cont_point[i];
                }
                continue;
            }
        }

        // Shrink the simplex
        for ( unsigned i = 1; i < dim_simp; i++ ) {
            for ( unsigned j = 0; j < dim_param; j++ ) {
                simplex[i * dim_param + j] = simplex[j] + sigma * \
                    ( simplex[i * dim_param + j] - simplex[j] );
            }
        }
        
    }

    // Free the vectors
    centroid.clear();
    refl_point.clear();
    expa_point.clear();
    cont_point.clear();
    simplex.clear();

    return min_points;
}

std::vector<double> get_centroid( 
        std::vector<double> simplex, unsigned dim_param ) {

    std::vector<double> centroid( dim_param );

    for ( unsigned i = 0; i < dim_param; i++ ) {
        centroid[i] = 0.0;
        for ( unsigned j = 0; j < dim_param; j++ ) {
            centroid[i] += simplex[j * dim_param + i];
        }
        centroid[i] = centroid[i] / dim_param;
    }
    return centroid;
}

void order_simplex( std::vector<double> &simplex, 
        std::vector<double> &image_s ) {

    unsigned dim_simp = image_s.size();
    unsigned dim_param = dim_simp - 1;
    unsigned* index_order = new unsigned[dim_simp];
    for ( unsigned i = 0; i < dim_simp; i++ ) 
        index_order[i] = i;

    // Auxiliar values to store the values and order of indices
    double aux_image;
    unsigned aux_index;

    for ( unsigned i = 0; i < dim_simp; i++ ) {
        for ( unsigned j = 0; j < dim_simp - i - 1; j++ ) {
            if ( image_s[j] > image_s[j+1] ) {
                // Order image_s
                aux_image = image_s[j];
                image_s[j] = image_s[j+1];
                image_s[j+1] = aux_image;

                // Order the index order as well
                aux_index = index_order[j];
                index_order[j] = index_order[j+1];
                index_order[j+1] = aux_index;
            }
        }
    }

    std::vector<double> aux_simplex( dim_simp * dim_param );
    // Now reorder the simplex according to index_order
    for ( unsigned i = 0; i < dim_simp; i++ ) {
        for ( unsigned j = 0; j < dim_param; j++ ) {
            aux_simplex[i * dim_param + j] = \
                simplex[index_order[i] * dim_param + j];
        }
    }

    simplex = aux_simplex;

    // Free memory
    delete[] index_order;
    aux_simplex.clear();
}