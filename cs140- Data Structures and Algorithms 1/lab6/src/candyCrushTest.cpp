#include <cstdio>
#include <iostream>
#include <sstream>
#include "candyCrush.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  string inputLine;
  istringstream buffer;
  string command;
  int userChoice;
  int rowLength;
  candyCrush *player;
  
  if (argc != 2) {
    fprintf(stderr, "usage: candyCrushTest inputFile\n");
    return -1;
  }

  player = new candyCrush(argv[1]);
  rowLength = player->getRowLength();
  
  while (getline(cin, inputLine)) {
    buffer.clear();
    buffer.str(inputLine);
    // check for an empty line
    if (!(buffer >> command)) {
      fprintf(stderr, "empty line--missing command\n");
      continue;
    }

    if (command == "PRINT") {
      player->printCandy();
    }
    else if (command == "CHOOSE") {
      if (!(buffer >> userChoice)) {
	fprintf(stderr, "missing choice or invalid input for choice--must be an integer\n");
	continue;
      }

      if ((userChoice < 0) || (userChoice >= rowLength)) {
	fprintf(stderr, "invalid index %d: must be between 0 and %d\n",
		userChoice, rowLength-1);
	continue;
      }
      int pointsThisTurn = player->play(userChoice);
      printf("you scored %d points\n", pointsThisTurn);
    }
    else if (command == "SCORE") {
      printf("current score = %d\n", player->getScore());
    }
    else if (command == "QUIT") {
      break;
    }
    else {
      fprintf(stderr, "invalid command--valid commands are PRINT, CHOOSE, SCORE, QUIT\n");
    }
    printf("\n");
  }
}
