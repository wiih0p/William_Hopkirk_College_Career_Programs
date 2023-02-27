
CC = g++
CFLAGS = -Wall
BIN =  Mazemake Mazesolve

all: $(BIN)

.cpp: 
	$(CC) $(CFLAGS) -o $* $<

clean:
	@rm -f $(BIN)
	@rm -f maze?.txt path?.txt