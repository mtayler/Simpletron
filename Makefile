CC=gcc
CFLAGS=-Wall -pedantic
LDFLAGS=
SOURCES=simpletron.c
EXECUTABLE=build/simpletron

all:
	$(CC) $(CFLAGS) $(SOURCES) -g -o $(EXECUTABLE)

prod:
	$(CC) $(CFLAGS) $(SOURCES) $(EXECUTABLE)

run:
	$(EXECUTABLE)
