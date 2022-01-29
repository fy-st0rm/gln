LIBS = -lSDL2main -lSDL2 -lGLU -lGL -lGLEW
all:
	gcc -c test.c
	gcc -o test test.o -L. -lgln $(LIBS)
	./test
