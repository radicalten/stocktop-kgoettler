CC=gcc
CFLAGS=--std=c89
LIBS=-lcurses

stocks:
	$(CC) $(CFLAGS) ./stocks.c $(LIBS) -o $@

.PHONY : clean

clean:
	rm -rf ./*.o stocks
