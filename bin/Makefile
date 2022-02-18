LIBS_LINUX = -lSDL2main -lSDL2 -lSDL2_image -lGLU -lGL -lGLEW
LIBS_WIN = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -lUser32 -lShell32 -lglew32

linux:
	gcc -c test.c
	gcc -o test test.o -L. -lgln $(LIBS_LINUX)
	rm *.o
	./test

win:
	gcc -c test.c
	gcc -o test.exe test.o -L../opengl/lib -L. -lgln $(LIBS_WIN)
	erase *.o
	test.exe

