
// GolGrid.c
//  Game of Life Grid, storage and manipulation.
//
#include "golgrid.h"
#include "error.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//
// ****************** Internal to this file
//



// Constant ROW and COL lengths
//
#define GRID_ROWS 8
#define GRID_COLS 8

// Alive and dead cell values
// -- Internally, not for display
//
#define GRID_DEAD_CELL 0
#define GRID_LIVE_CELL 1


// Underlying Grid type
//
typedef uint8_t GolGridType[GRID_ROWS][GRID_COLS];

// Two grids, one will always be valid.
// The pointers below will change to indicate which is which
//
static GolGridType GridA;
static GolGridType GridB;

// Holders for which grid is current
//
static GolGridType* ValidGrid = &GridA;
static GolGridType* WorkGrid = &GridB;

// How many generations has this grid been through
//
static uint8_t GridGenerations = 0;

// Simple accessor, always from the valid grid
//
static uint8_t Get(int row, int column)
{
    return (*ValidGrid)[row][column];
}

// Check valid grid, returns true if living, false if dead
//
static bool IsCellAlive(int row, int column)
{
    return ((*ValidGrid)[row][column] == GRID_LIVE_CELL);
}


// SetAlive: Set an indvidual cell to alive
//
static void SetAlive(int row, int column)
{
    (*WorkGrid)[row][column] = GRID_LIVE_CELL;
}

// SetDead: Set an indvidual cell to dead
//
static void SetDead(int row, int column)
{
    (*WorkGrid)[row][column] = GRID_DEAD_CELL;
}

// SwapGrids
// Swap the work-grid with the active-grid
// This will happen after an update to the work-grid is completed
//
static void SwapGrids()
{
    GolGridType* tmp = ValidGrid;
    ValidGrid = WorkGrid;
    WorkGrid = tmp;
}

// For each cell in a row
// Determine and store the next state.
//
static void AdvanceRowToNextGeneration(
    int previousRow,
    int currentRow,
    int nextRow
)
{
    // Cursors for walking the columns (cells) in a row
    //
    int previousColumn = (GRID_COLS - 1);        // Grid wraps left and right
    int currentColumn = 0;
    int nextColumn = 1;

    // The logic relies on these values being zero and 1.
    //
    assert(GRID_DEAD_CELL == 0);
    assert(GRID_LIVE_CELL == 1);

    do
    {
        // Count all living of the 8 possible neighbors
        //
        int neighborCount = 0;

        // Ceck three cell neighbors 'below'
        //
        neighborCount += Get(previousRow, previousColumn);
        neighborCount += Get(previousRow, currentColumn);
        neighborCount += Get(previousRow, nextColumn);

        // Check left and right neighbors in same row
        //
        neighborCount += Get(currentRow, previousColumn);
        neighborCount += Get(currentRow, nextColumn);

        // Ceck three cell neighbors 'above'
        //
        neighborCount += Get(nextRow, previousColumn);
        neighborCount += Get(nextRow, currentColumn);
        neighborCount += Get(nextRow, nextColumn);

        // Apply GOL rules to determine new state based on neighbor count and current state
        //

        // Alive or dead, if it has three neightbors it will be alive
        //
        if (neighborCount == 3)
        {
            SetAlive(currentRow, currentColumn);
        }
        else
        {
            // Most other states are dead
            //
            SetDead(currentRow, currentColumn);
            if (Get(currentRow, currentColumn) == 1)
            {
                // Not three neighbors, but currently alive.
                // Only stays alive if two neighbors
                //
                if (neighborCount == 2)
                {
                    SetAlive(currentRow, currentColumn);
                }
            }
            // else - currently dead and does not have three neighbors
            //            Stays dead.
        }

        // Advance row cursors
        //
        previousColumn = currentColumn;
        ++currentColumn;
        nextColumn = (currentColumn + 1) % GRID_COLS;

    } while (currentColumn < GRID_COLS);
}


//
// ****************** Published Functionality
//

// Grid_InitializeFromFile
//    Write the first generation of a grid from a file.
//  The code doesn't allow much slop with the format.
//  Must use the correct symbols and each line must end with a newline
//  and be the correct length.
//
int32_t Grid_InitializeFromFile(
    const char* patternFileName,
    char deadCell,  // Char in file to indicate a dead cell
    char liveCell   // Char in file to indicate a live cell
)
{
    int32_t result = NO_ERROR;
    FILE* patternFile = fopen(patternFileName, "r");

    if (patternFile == NULL)
    {
        fprintf(stderr, "Could not open requested pattern file: %s\n", patternFileName);
        return ERROR_FILE_OPEN_FAILURE;
    }

    // Walk the file from the start.
    // For each row, read all chars, then read off new-line char.
    //
    fseek(patternFile, SEEK_SET, 0);
    for (int row = 0; row < GRID_ROWS; ++row)
    {
        for (int col = 0; col < GRID_COLS; ++col)
        {
            int nextCell = fgetc(patternFile);
            if (nextCell == EOF)
            {
                fprintf(stderr, "File is too short");
                result = ERROR_INVALID_FILE_FORMAT;
                goto EXIT;
            }
            if ((uint8_t)nextCell == deadCell)
            {
                SetDead(row, col);
            }
            else if ((uint8_t)nextCell == liveCell)
            {
                SetAlive(row, col);
            }
            else
            {
                fprintf(stderr, "Invalid cell %c at [%d,%d]\r\n", (uint8_t)nextCell, row, col);
                result = ERROR_INVALID_FILE_FORMAT;
                goto EXIT;
            }
        }
        // read off the newline char to advance
        fgetc(patternFile);
    }

    // Make the first generation active
    //
    SwapGrids();
    GridGenerations = 1;

EXIT:
    fclose(patternFile);
    return result;
}

// Grid_InitializeAsRandom: Initialize grid to a random first-generation state
//
void Grid_InitializeAsRandom()
{
    // Enough items to possible seed every cell
    //
    int numberTries = rand() % (GRID_ROWS * GRID_COLS);

    // Initialize all the work-grid cells to DEAD
    //
    memset((void*)(*WorkGrid), GRID_DEAD_CELL, GRID_ROWS * GRID_COLS);

    // Set a random number of cells to alive.
    // It does not matter if the same cell is set ALIVE twice.
    //
    while (numberTries-- > 0)
    {
        int randCol = rand() % GRID_COLS;
        int randRow = rand() % GRID_ROWS;
        SetAlive(randCol, randRow);
    }

    SwapGrids();
    GridGenerations = 1;
}

// Grid_AdvanceToNextGeneration
// Easy - For each row, advance it to the next generation :-)
//
void Grid_AdvanceToNextGeneration()
{
    int previousRow = (GRID_ROWS - 1);    // Grid wraps top to bottom
    int currentRow = 0;
    int nextRow = 1;

    // Treating non-initialization as a logic error.
    // Initialization reports any failures.
    //
    assert(GridGenerations >= 1);
    do
    {
        AdvanceRowToNextGeneration(previousRow, currentRow, nextRow);

        // Advance row cursors
        //
        previousRow = currentRow;
        ++currentRow;
        nextRow = (currentRow + 1) % GRID_ROWS;
    } while (currentRow < GRID_ROWS);

    // Commit the next generation
    //
    SwapGrids();
    ++GridGenerations;
}

// Grid_Write : Write grid to sent stream
//
void Grid_Write(
    FILE* stream,   // Target stream
    char deadCell,  // This character is used for a dead-ell
    char liveCell   // This character is used for a live-cell
    )
{
    for (int row = 0; row < GRID_ROWS; ++row)
    {
        for (int column = 0; column < GRID_COLS; ++column)
        {
            fprintf(stream, "%c", IsCellAlive(row, column) ? liveCell : deadCell);
        }
        fprintf(stream, "\n");
    }
}

// Grid_GetGenerationCount : returns the number of generations the grid has been run
//
uint32_t Grid_GetGenerationCount()
{
    return GridGenerations;
}
