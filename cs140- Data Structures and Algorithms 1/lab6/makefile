#COSC 140: Lab 6 - Candy Crush (MakeFile)


# Description:
     #Use to compile "candyCrushTest". Also lets students automate construction
     #of a valid tar file to submit upon assignment completion.

# Author:
    # Clara Nguyen
     


# Parameters
CC     = g++
CFLAGS = --std=c++11 -g -Iinclude
EXEC   = bin/candyCrushTest

# Compilation tree
all: $(EXEC)

bin/candyCrushTest: obj/candyCrush.o obj/candyCrushTest.o
	$(CC) $(CFLAGS) -o $@ $^

# Object files
obj/candyCrush.o: src/candyCrush.cpp include/candyCrush.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

obj/candyCrushTest.o: src/candyCrushTest.cpp include/candyCrush.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

# Wipe out all executables and object files
clean:
	$(RM) $(EXEC) obj/*

# Package for submission
package: src/candyCrush.cpp include/candyCrush.hpp
	tar -cvf lab6.tar $^
