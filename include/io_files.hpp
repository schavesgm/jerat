#ifndef IO_FILES_HPP
#define IO_FILES_HPP

#include <iostream>
#include <fstream>          
#include <string>           
#include <vector>           
#include <sstream>          
#include <iterator>         
#include <algorithm>


// Load definitions
#include "defs.hpp"

// Load data from a file
Matrix load_data( Input, std::string = "#" );

// Load input data from file
std::vector<std::string> get_key( std::string, std::string );
std::vector<double> to_double( std::vector<std::string> );
std::vector<unsigned> to_unsigned( std::vector<std::string> );

// Write Matrix into file
void write_matrix( std::string, Matrix );

// Write vector into file
void write_vector( std::string, std::vector<double>, unsigned, 
        unsigned );

#endif
