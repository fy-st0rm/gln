LIBS_LINUX = -lglnx -lSDL2main -lSDL2 -lSDL2_image -lGLU -lGL -lGLEW -lm
SRC = test.c 
OBJ = test.o
OUT = test 

linux:
	gcc -c -I../includes $(SRC)
	gcc -o $(OUT) $(OBJ) -L../bin $(LIBS_LINUX)
	rm *.o
	./$(OUT)
