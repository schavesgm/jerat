#include "corr.hpp"

void Corr::get_tmax( double tol_noise, double tol_temp ) {
    /* 
       Method to calculate the t_max to fit the data for a given
       correlation function. The algorithm to decide which t_max
       to select is given by,
       t_max = min( 
                1 / stn(G(t+a) >= tol_noise, 
                \bar{G}(t+a) < 0, t + a > tol_temp 
            )
    */ 
    // Check if needed data is calculated --> Get it if not

    this->tmax = this->raw.time_extent - 1;

    if ( !this->calc_central )
        this->cent_corr( 1 );
    if ( !this->calc_sig2noi )
        this->sig_to_noise();


    // Choose the time
    bool first_cond, sec_cond, third_cond;
    for ( unsigned nt = 0; nt < stn.time_extent - 1; nt++ ) {

        // Check the first condition
        ( 1 / this->stn.data[nt+1] ) > tol_noise \
            ? first_cond = true : first_cond = false;

        // Check second condition
        this->cent.data[nt+1] < 0 \
            ? sec_cond = true : sec_cond = false;

        // Check third condition
        nt + 1 > tol_temp ? third_cond = true : third_cond = false;

        if ( first_cond || sec_cond || third_cond ) {
            this->tmax = nt;
            break;
        }
    }
    this->calc_tmax = true;
}

void Corr::get_tmin( unsigned tplat ) {
    /*
       Get the beginning of the time window by sampling an 
       integer from the following interval.
                t_min \in [2a, t_max - t_plat ];
    */
    if ( !this->calc_tmax )
        this->get_tmax( 1.0, 0.75 * this->raw.time_extent );

    std::uniform_int_distribution<int> dist(2, this->tmax - tplat );
    this->tmin = dist(this->random_eng);
}
