LIBS_LINUX = -lglnx -lSDL2main -lSDL2 -lSDL2_image -lGLU -lGL -lGLEW -lm
LIBS_WINDOWs = -lglnx -lmingw32 -lopengl32 -lUser32 -lGdi32 -lShell32 -lglew32 -lSDL2main -lSDL2 -lSDL2_image
SRC = test.c 
OBJ = test.o
OUT = test 

linux:
	gcc -c -I../../includes $(SRC)
	gcc -o $(OUT) $(OBJ) -L../../bin $(LIBS_LINUX)
	rm *.o
	./$(OUT)

win:
	gcc -c -I../dependency/GLEW/include -I../dependency/SDL2/include -I../includes $(SRC)
	gcc -o $(OUT) $(OBJ) -L../dependency/GLEW/lib -L../dependency/SDL2/lib -L../bin $(LIBS_WINDOWs)
	erase *.o
	$(OUT)
