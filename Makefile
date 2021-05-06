CC = gcc
LIBS = -lm -Wall -O3
FILES = iris.c



iris: $(FILES)	
		$(CC) $(FILES) $(LIBS) 

clean:
	rm -v -f a.out