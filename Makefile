CC=g++
CFLAGS=-Wall -std=c++11
TARGET=ptf

$(TARGET): main.o ball.o engine.o
	$(CC) $(CFLAGS) -o build/$(TARGET) build/engine.o build/main.o build/ball.o -lsfml-graphics -lsfml-window -lsfml-system -lpthread

ball.o: ball.cpp
	$(CC) $(CFLAGS) -o build/ball.o -c ball.cpp

engine.o: engine.cpp
	$(CC) $(CFLAGS) -o build/engine.o -c engine.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -o build/main.o -c main.cpp

clean:
	rm build/*
