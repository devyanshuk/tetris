#ifndef ERRORMACROS_H
#define ERRORMACROS_H

#include <iostream>

using namespace std;

#define CHECK_ERROR(is_error, message) \
    if((is_error)) { \
        cerr << message << endl << "line: " << __LINE__ << endl << "file: " << __FILE__ << endl; \
        exit(1); \
    }

#endif