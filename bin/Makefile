LIBS = -lglfw -lGLU -lGL -lGLEW
all:
	gcc -c test.c
	gcc -o test test.o -L. -lgln $(LIBS)
	./test
