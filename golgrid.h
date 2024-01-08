
// Game of Life (GOL) Grid functionality
// For simplicity, prefixed a Grid_ as opposed to GolGrid_
//
#ifndef GOLGRID_H
#define GOLGRID_H

#include <stdint.h>
#include <stdio.h>

// Set as first generation with contents from file
// Returns error status
//
int32_t Grid_InitializeFromFile(const char* patternFileName, char deadCell, char liveCell);

// Set the first generation od the grid to random contents
//
void Grid_InitializeAsRandom(void);

// Write the grid to a file stream
//
void Grid_Write(FILE* stream, char deadCell, char liveCell);

// Run the rules on the current grid
//
void Grid_AdvanceToNextGeneration();

// Query for the number of generations
//
uint32_t Grid_GetGenerationCount();

#endif // GOLGRID_H
