CXX = gcc
CXXFLAGS = -pedantic -Wall -Werror -std=c11
NAME = lab4
DIR = ../src

all: io.o
	$(CXX) $(CXXFLAGS) io.o main.c -o $(NAME)
io.o: io.h
	$(CXX) $(CXXFLAGS) -c io.c
clean:
	rm -f *.o $(NAME)