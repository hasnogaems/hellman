CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage
HEADER = s21_decimal.h

all: 
	gcc *.c
	./a.out