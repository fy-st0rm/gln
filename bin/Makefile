LIBS = -lSDL2main -lSDL2 -lSDL2_image -lGLU -lGL -lGLEW
all:
	gcc -c test.c
	gcc -o test test.o -L. -lgln $(LIBS)
	rm *.o
	./test
