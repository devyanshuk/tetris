#pragma once

#include <getopt.h>
#include <iostream>
#include "constants.hpp"

struct ArgInput {
    int _length = LENGTH;
    int _width = WIDTH;
};

void print_help(char * program_name);

bool read_args(ArgInput & input, int argc, char ** argv);