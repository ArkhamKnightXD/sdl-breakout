1. Install the latest version of MinGW-w64 via MSYS2: https://code.visualstudio.com/docs/cpp/config-mingw
 
Actual MinGW-w64 version use: gcc-g++ versions (Rev3, Built by MSYS2 project) 13.2.0

Actual SDL version use: https://github.com/libsdl-org/SDL/releases/download/release-2.30.2/SDL2-devel-2.30.2-mingw.zip

To build use this command: g++ -Llib -o game  main.cpp -lSDL2
To run use this command: ./game

To build and run automatically:
powershell: ./run 
bash: ./run.bash  