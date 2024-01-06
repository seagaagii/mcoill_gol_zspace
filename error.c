
// Error.c
// Very simple error system, mostly for readability.
//
#include "error.h"


//
// ****************** Published Functionality
//


// Error_GetString : Convert error code to simple message
//
const char* Error_GetString(
	int32_t error		// Error Code to look-up
	)
{
	switch (error)
	{
		case NO_ERROR: return "SUCCESS";
		case ERROR_TO_MANY_ARGUMENTS: return "Too many arguments sent to program";
		case ERROR_FILE_OPEN_FAILURE: return "Unable to open requested initial file";
		case ERROR_INVALID_FILE_FORMAT: return "Initial file is incorrect format";
	}
	return "Unknown Error Code";
}
