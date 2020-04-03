#ifndef DEFS_HPP
#define DEFS_HPP

// File to store definitions
#include <string>
#include <vector>

// Input structure
struct Input {
    std::string file_name;
    unsigned row_size, col_size, time_extent;
    bool rescale = false;
    unsigned col_rescale = 1;
    double res_factor = 1.0;
};

// Matrix structure
struct Matrix {
    double* data;
    unsigned row_size, col_size;
    unsigned time_extent;
};

#endif
