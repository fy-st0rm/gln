CC = gcc
SRC = src/*.c
OBJ = *.o
OUT = libgln.a

linux:
	$(CC) -c $(SRC)
	ar rcs $(OUT) $(OBJ)

	rm *.o
	mv $(OUT) bin/

win:
	$(CC) -Iopeng/include -c $(SRC)
	ar rcs $(OUT) $(OBJ)
	erase *.o
	move $(OUT) bin/

