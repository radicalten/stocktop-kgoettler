# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC=gcc
CFLAGS=-g -Wall
LIBS=-lcurses -lcurl -ljson-c -lm
SRCDIR=src
TESTDIR=test
BUILDDIR=build

default: stocktop

stocktop:  $(BUILDDIR)/api.o $(BUILDDIR)/str.o $(BUILDDIR)/stocks.o
	$(CC) $(CFLAGS) -o $(BUILDDIR)/stocktop $(BUILDDIR)/api.o $(BUILDDIR)/str.o $(BUILDDIR)/stocks.o $(LIBS) 

$(BUILDDIR)/api.o:  $(SRCDIR)/api.c $(SRCDIR)/api.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/api.c -o $(BUILDDIR)/api.o

$(BUILDDIR)/stocks.o:  $(SRCDIR)/stocks.c $(SRCDIR)/stocks.h $(BUILDDIR)/api.o
	$(CC) $(CFLAGS) -c $(SRCDIR)/stocks.c -o $(BUILDDIR)/stocks.o

$(BUILDDIR)/str.o: $(SRCDIR)/str.c $(SRCDIR)/str.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/str.c -o $(BUILDDIR)/str.o

clean: 
	$(RM) stocktop $(SRCDIR)/*.o *~
	$(RM) $(TESTDIR)/*.o
	$(RM) $(BUILDDIR)/*

memcheck: $(BUILDDIR)/stocktop
	valgrind --leak-check=full $(BUILDDIR)/stocktop

test : $(BUILDDIR)/test.o
	$(BUILDDIR)/test.o

$(BUILDDIR)/test.o: $(TESTDIR)/test.c $(BUILDDIR)/api.o
	$(CC) $(TESTDIR)/test.c -lcheck -lrt -Isrc/ $(BUILDDIR)/api.o -lcurl -ljson-c -o $(BUILDDIR)/test.o
