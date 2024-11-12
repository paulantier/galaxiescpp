# Variables
CXX = g++
CXXFLAGS = -std=c++11 -Wall
SDL2_LIBS = `sdl2-config --cflags --libs`

# Cibles
all: main

main: main.cpp Environment.cpp Aleatoire.cpp Star.cpp Planet.cpp
	$(CXX) $(CXXFLAGS) main.cpp Environment.cpp Aleatoire.cpp Star.cpp Planet.cpp -o main $(SDL2_LIBS)

clean:
	rm -f main
