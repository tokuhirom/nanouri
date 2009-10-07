all: test
	prove --verbose --exec '/bin/sh -c ' ./test

test: test.c picouri.h
	gcc -g -Wall -Wextra -o test test.c

