// Arguments.c
//	Implementation and definition of items from argument.h
//

#include "arguments.h"        // Implemented in this file
#include "error.h"            // Error Strings

#include <stdio.h>            // fprintf, FILE*

//
// ****************** Externally scoped data
//
const char* ARGUMENT_PATTERN_RANDOM = "RANDOM";

//
// ****************** Internal to this file
//


// Storage for initial parameter, NULL indicates "RANDOM"
//
static char* InitialRequestedParameter = NULL;

// Display the usage to a FILE stream
//
static void PrintUsage(
    FILE* stream        // Print to here
    )
{
    fprintf(stream,
        "Usage: GOL.EXE [FILENAME]\n"
        "\tFILENAME : Initialpattern file\n"
        "\t           if ommited a random pattern will be used\n"
        "\t           RANDOM as a filename will also generate a random pattern\n"
    );
}


//
// ****************** Published Functionality
//



// Arguments_Parse: 'Parse' the arguments.
// No actual parsing. Checks the argumnt count.
// Stores a pointer to first arg or null if not pressent
//
int32_t Arguments_Parse(
    int argc,       // from main(...)
    char* argv[]    // from main(...)
    )
{
    // Does not support second [or greater] parameters
    //
    if (argc > 2)
    {
        PrintUsage(stderr);
        return ERROR_TO_MANY_ARGUMENTS;
    }

    // exe name + pattern argumwnt
    //
    if (argc == 2)
    {
        InitialRequestedParameter = argv[1];
    }
    else // just the exe name
    {
        InitialRequestedParameter = NULL;
    }

    return NO_ERROR;
}

// Arguments_GetInitialPatternArgument: Requested initial pattern
// Returns: Initial pattern arg or 
//          pointer to "RANDOM" if no arg was present.
// Will always return a non-null value
const char* Arguments_GetInitialPatternArgument()
{
    if (InitialRequestedParameter == NULL)
    {
        return ARGUMENT_PATTERN_RANDOM;
    }
    return InitialRequestedParameter;
}
