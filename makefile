OBJS	= main.o Prompts.o Bounds.o FFD_BFD.o ReadFile.o Bin.o Item.o
SOURCE	= main.cpp Prompts.cpp Bounds.cpp FFD_BFD.cpp ReadFile.cpp Bin.cpp Item.cpp
HEADER	= Prompts.hpp FFD_BFD.hpp Bounds.hpp ReadFile.hpp Bin.hpp Item.hpp
OUT	= vbpacking
CC	= g++ -std=c++0x
FLAGS	= -g -c -Wno-sign-compare

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) 


main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 

Prompts.o: Prompts.cpp
	$(CC) $(FLAGS) Prompts.cpp 

Bounds.o: Bounds.cpp
	$(CC) $(FLAGS) Bounds.cpp 

FFD_BFD.o: FFD_BFD.cpp
	$(CC) $(FLAGS) FFD_BFD.cpp 

ReadFile.o: ReadFile.cpp
	$(CC) $(FLAGS) ReadFile.cpp 

Bin.o: Bin.cpp
	$(CC) $(FLAGS) Bin.cpp 

Item.o: Item.cpp
	$(CC) $(FLAGS) Item.cpp 

clean:
	rm -f $(OBJS)
