CC = gcc
SRC = src/*.c
OBJ = *.o
OUT = libgln.a

win:
	$(CC) -Iopengl/include -c $(SRC)
	ar rcs $(OUT) $(OBJ)
	erase *.o
	move $(OUT) bin/

linux:
	$(CC) -c $(SRC)
	ar rcs $(OUT) $(OBJ)

	rm *.o
	mv $(OUT) bin/


