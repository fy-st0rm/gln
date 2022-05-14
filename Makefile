CC = gcc
SRC = src/*.c 
GLNX_SRC = src/*.c src/glnx/*.c
OBJ = *.o
OUT = libgln.a
GLNX_OUT = libglnx.a

linux:
	$(CC) -c $(GLNX_SRC)
	ar rcs $(GLNX_OUT) $(OBJ)
	rm *.o
	mv $(GLNX_OUT) bin/

win:
	$(CC) -Iopengl/include -c $(GLNX_SRC)
	ar rcs $(GLNX_OUT) $(OBJ)
	erase *.o

