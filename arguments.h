
// Arguments.h
//	Simple argument handler.
//		Supports a single parameter which is "random" or it refeences a pattern file
//		The parameter is optional.
//
//	The input is very strict with regards to the number of parameters, one or none only.
//  Note the parsing does not check validity or exitence of the file.
//
#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdint.h>

//	Argument string in place of pattern file.
//	Used to indicate a random initial pattern.
//
extern const char* ARGUMENT_PATTERN_RANDOM;

// Argument support functions
//
int32_t Arguments_Parse(int argc, char* argv[]);
const char* Arguments_GetInitialPatternArgument();

#endif // ARGUMENTS_H
