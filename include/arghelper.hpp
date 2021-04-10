#pragma once

#include <getopt.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "constants.hpp"

struct ArgInput {
	int _length = LENGTH;
	int _width = WIDTH;
	bool _show_hint = true;
};

void print_help(char * program_name);

bool read_args(ArgInput & input, int argc, char ** argv);
