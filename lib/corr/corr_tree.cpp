#include "corr.hpp"

unsigned* Correlator::get_tmax( double tol_noise, double tol_temp, 
        bool use_positive ) {
    /* 
       Method to calculate the t_max to fit the data for a given
       correlation function. The algorithm to decide which t_max
       to select is given by,
       t_max = min( 
                1 / stn(G(t+a) >= tol_noise, 
                \bar{G}(t+a) < 0, t + a > tol_temp 
            )
    */ 
    this->bool_tmax = true;
    assert( tol_temp > 0.0 && tol_temp < 1.0 );

    this->t_max = new unsigned[this->num_inputs];
    for ( unsigned ni = 0; ni < this->num_inputs; ni++ )
        this->t_max[ni] = this->RAW_DATA[ni].time_extent - 1;

    // Take care of needed data
    if ( !this->bool_central )
        this->central_value();
    if ( !this->bool_sig2noise )
        this->sig_to_noise();

    bool first_cond, sec_cond, third_cond;
    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {

        unsigned n_tau = this->RAW_DATA[ni].time_extent;
        unsigned max_temperature = tol_temp * n_tau;

        for ( unsigned nt = 0; nt < n_tau; nt++ ) {

            // Check the first condition
            ( 1 / this->sig2noise[ni].data[nt+1] ) > tol_noise \
                ? first_cond = true : first_cond = false;

            // Check second condition
            this->central[ni].data[nt+1] < 0 \
                ? sec_cond = true : sec_cond = false;

            // Check third condition
            nt + 1 > max_temperature ? third_cond = true : \
                third_cond = false;

            if ( use_positive ) {
                if ( first_cond || sec_cond || third_cond ) {
                    this->t_max[ni] = nt;
                    break;
                }
            } else {
                if ( first_cond || third_cond ) {
                    this->t_max[ni] = nt;
                    break;
                }
            }
        }
    }
    return this->t_max;
}

unsigned* Correlator::get_tmin( unsigned tplat ) {
    /*
       Get the beginning of the time window by sampling an 
       integer from the following interval.
                t_min \in [2a, t_max - t_plat ];
    */
    // Let the code know you have calculated tmin
    this->bool_tmin = true;
    
    // Allocate some memory
    this->t_min = new unsigned[this->num_inputs];

    // Take care of needed values
    if ( !this->bool_tmax )
        unsigned* t_max = this->get_tmax( 1.0, 0.75 );

    // Sample the values
    for ( unsigned ni = 0; ni < this->num_inputs; ni++ ) {

        // Generate the sampler
        std::uniform_int_distribution<int> \
            dist(2, this->t_max[ni] - tplat );

        this->t_min[ni] = dist(this->random_eng);
    }
    return this->t_min;
}
