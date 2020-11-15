# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC=gcc
CFLAGS=-g -Wall
LIBS=-lcurses -lcurl -ljson-c
SRCDIR=src
TESTDIR=test

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: stocktop

stocktop:  $(SRCDIR)/api.o $(SRCDIR)/stocks.o
	$(CC) $(CFLAGS) -o stocktop $(SRCDIR)/api.o $(SRCDIR)/stocks.o $(LIBS) 

$(SRCDIR)/api.o:  $(SRCDIR)/api.c $(SRCDIR)/api.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/api.c -lcurl -ljson-c -o $(SRCDIR)/api.o

# To create the object file counter.o, we need the source files
# counter.c and counter.h:
#
$(SRCDIR)/stocks.o:  $(SRCDIR)/stocks.c $(SRCDIR)/stocks.h 
	$(CC) $(CFLAGS) -c $(SRCDIR)/stocks.c -o $(SRCDIR)/stocks.o

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) stocktop $(SRCDIR)/*.o *~
	$(RM) $(TESTDIR)/*.o

test : $(TESTDIR)/test.o
	./$(TESTDIR)/test.o

$(TESTDIR)/test.o: $(TESTDIR)/test.c
	$(CC) $(TESTDIR)/test.c -lcheck -Isrc/ src/api.o -lcurl -ljson-c -o $(TESTDIR)/test.o
