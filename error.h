
// Error.h
// Very simple error system, mostly for readability.
// Negative numbers are errors
// These values are returned from the program.
//
#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>


// Defined Codes
//
#define NO_ERROR 0
#define ERROR_TO_MANY_ARGUMENTS -1
#define ERROR_FILE_OPEN_FAILURE -2
#define ERROR_INVALID_FILE_FORMAT -3

// Convert the error code to a simple string
//
const char* Error_GetString(int32_t error);

#endif // ERROR_H

