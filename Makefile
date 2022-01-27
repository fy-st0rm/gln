CC = gcc
SRC = *.c
OBJ = *.o
OUT = libgln.a

LIBS = -lglfw -lGLU -lGL -lGLEW

all:
	$(CC) -c $(SRC)
	ar rcs $(OUT) $(OBJ)

	rm *.o
	mv $(OUT) bin/

