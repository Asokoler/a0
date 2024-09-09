# Your GCC compiler
CC=gcc

# All the flags in the (CompSys) world
CFLAGS=-std=c11 -Wall -Werror -Wextra -pedantic -g3

# Compile your program 
file: file.c 
	$(CC) $(CFLAGS) -o file file.c

# Used for generating a zip (make ../src.zip)
#../src.zip: file.c Makefile test.sh
#	cd .. && zip src.zip src/file.c src/Makefile src/test.sh

# Used for removing generated files (make clean)
clean:
	rm -f file

.PHONY: clean
