CC=g++
CFLAGS=-Wall -std=c++11
TARGET=ptf

$(TARGET): build/main.o build/object.o build/physics_object.o build/block.o build/ball.o build/engine.o 
	$(CC) $(CFLAGS) -o build/$(TARGET) \
	build/main.o build/object.o build/physics_object.o build/block.o \
	build/ball.o build/engine.o \
	-lsfml-graphics -lsfml-window -lsfml-system -lpthread

build/main.o: main.cpp
	$(CC) $(CFLAGS) -o build/main.o -c main.cpp

build/object.o: object.cpp object.hpp
	$(CC) $(CFLAGS) -o build/object.o -c object.cpp

build/physics_object.o: physics_object.cpp physics_object.hpp
	$(CC) $(CFLAGS) -o build/physics_object.o -c physics_object.cpp

build/block.o: block.cpp block.hpp
	$(CC) $(CFLAGS) -o build/block.o -c block.cpp

build/ball.o: ball.cpp ball.hpp
	$(CC) $(CFLAGS) -o build/ball.o -c ball.cpp

build/engine.o: engine.cpp engine.hpp
	$(CC) $(CFLAGS) -o build/engine.o -c engine.cpp

clean:
	rm build/*
