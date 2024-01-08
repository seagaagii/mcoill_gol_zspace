// gol.c
// This file contains the entry point (main) for this program
// It mananges the various components and runs the high-level logic.
//


#include "arguments.h"
#include "error.h"
#include "golgrid.h"

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>


// Number of generates to run the program for
//
#define GENERATIONS_TO_RUN 4

// Living and dead-cell representations
// -- This are also the expected file symbols
//
static const uint8_t DEAD_CELL_CHAR = '.';
static const uint8_t LIVE_CELL_CHAR = 'X';


// NOTE: Having trouble with my build system
// The time.h header in mingwin does not seem to be right.
// This is exported in the library, linking work just fine
//
extern int time(void*);

// CaseInsentitiveStringCompare
// This is to check for args and doesn't have to be fast
//
// Returns  true if the upper-case versions of the strings are the same
//          false otherwise
static bool CaseInsentitiveStringCompare(
    const char* lhs,
    const char* rhs
    )
{
    // First validate they are the same size or not 'equally' NULL
    //
    size_t len = 0;
    if (lhs == NULL)
    {
        return (rhs == NULL);
    }
    if (rhs == NULL)
    {
        return false;
    }

    len = strlen(lhs);
    if (len != strlen(rhs))
    {
        return false;
    }


    // Iterate over the characters comparing the upper-case version of each.
    //
    for (int checkIndex = 0; checkIndex < len; ++checkIndex)
    {
        if (toupper(lhs[checkIndex]) != toupper(rhs[checkIndex]))
        {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    const char* initialPattern = NULL;

    // Parse the argumwnts.
    //
    uint32_t result = Arguments_Parse(argc, argv);
    if (result != NO_ERROR)
    {
        goto EXIT;
    }

    // Seed the random number generator
    //
    srand(time(NULL));

    // Initialze with starting pattern for first generation
    // This should never fail to return an initial pattern
    //
    initialPattern = Arguments_GetInitialPatternArgument();
    assert(initialPattern != NULL);
    if (CaseInsentitiveStringCompare(initialPattern, ARGUMENT_PATTERN_RANDOM))
    {
        Grid_InitializeAsRandom();
    }
    else
    {
        result = Grid_InitializeFromFile(initialPattern, DEAD_CELL_CHAR, LIVE_CELL_CHAR);
        if (result != NO_ERROR)
        {
            goto EXIT;
        }
    }

    // Display 'file name' and grid for first generation
    //
    fprintf(stdout, "%s\n", initialPattern);
    Grid_Write(stdout, DEAD_CELL_CHAR, LIVE_CELL_CHAR);
    printf("\n");

    // Generate and display the rest of the generations
    //
    while (Grid_GetGenerationCount() < GENERATIONS_TO_RUN)
    {
        Grid_AdvanceToNextGeneration();
        Grid_Write(stdout, DEAD_CELL_CHAR, LIVE_CELL_CHAR);
        printf("\n");
    }

EXIT:
    if (result != NO_ERROR)
    {
        fprintf(stderr, Error_GetString(result));
    }
    return result;
}
