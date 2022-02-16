CC = gcc
SRC = src/*.c
OBJ = *.o
OUT = libgln.a

all:
	$(CC) -c $(SRC)
	ar rcs $(OUT) $(OBJ)

	rm *.o
	mv $(OUT) bin/

