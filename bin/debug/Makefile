default:
	g++ -c ../../src/*.cpp -std=c++14 -Wno-missing-braces -Wall -m64 -I ../../include
	g++ *.o -o ../../bin/debug/main -s -L ../../lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf
	./main.exe