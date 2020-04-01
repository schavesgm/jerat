#ifndef IO_FILES_HPP
#define IO_FILES_HPP

#include <fstream>          // Used to load data from file
#include <string>           // Used to load data from file
#include <vector>           // Used to load data from file
#include <sstream>          // Used to load data from file
#include <iterator>         // Used to load large data from file

// Load definitions
#include "defs.h"

struct matrix load_data( 
    const char, const unsigned, const unsigned, std::string,
    const bool, const unsigned, const double );

#include "../lib/io_files.cpp"

#endif
