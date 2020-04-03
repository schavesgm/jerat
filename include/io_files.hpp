#ifndef IO_FILES_HPP
#define IO_FILES_HPP

#include <iostream>
#include <fstream>          // Used to load data from file
#include <string>           // Used to load data from file
#include <vector>           // Used to load data from file
#include <sstream>          // Used to load data from file
#include <iterator>         // Used to load large data from file

// Load definitions
#include "defs.hpp"

struct Matrix load_data( struct Input, std::string = "#" );

#endif
