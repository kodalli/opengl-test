CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lGLEW -lglfw -lGL -lm

all: main

main: main.c
	$(CC) $(CFLAGS) -o main main.c $(LIBS)

clean:
	rm -f main

