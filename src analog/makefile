CC=g++
CXXFLAGS=-W -Wall -ansi -pedantic -std=c++11
EXEC=../analog
SRC=analog.cpp
OBJ=analog.o

all : $(EXEC)

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^

analog.o : analog.h

%.o : %.cpp
	$(CC) -o $@ -c $<  $(CXXFLAGS)

.PHONY : clean mrproper

clean :
	rm -rf *.o

mrproper : clean
	rm -rf $(EXEC)
	 
