#
# Makefile for ZSpace Interview 1 : Conways Game of Life
#	2023-01-05 : Matthew Coill
#
PROGRAM_NAME = gol.exe
CC = gcc
LINK = gcc
CFLAGS = -c -Os -std=c99 -Wall -DNDEBUG
LINK_FLAGS = -o $(PROGRAM_NAME)

OBJS = \
	arguments.o \
	error.o \
	golgrid.o \
	gol.o

	
gol: $(OBJS)
	$(LINK) $(LINK_FLAGS) $^

%.o: %.c
	$(CC) $(CFLAGS) $<

debug: CFLAGS += -g
debug: CFLAGS := $(filter-out -DNDEBUG,$(CFLAGS))
debug: gol


# Header Dependencies
#
arguments.o: arguments.h error.h
error.o: error.h
golgrid.o: golgrid.h error.h 
gol.o: arguments.h error.h golgrid.h


clean_unix:
	rm *.o gol.exe	

clean_win32:
	del *.o; del gol.exe