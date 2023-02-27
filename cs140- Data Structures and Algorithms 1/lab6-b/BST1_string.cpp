#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include "BST.h"

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cerr << "usage: bst1 datafile\n";
    return 0;
  } 

  string key;
  bst<string> T;

  ifstream fin(argv[1]);

  while (fin >> key)
    T.insert(key);

  fin.close();

  T.print_bylevel();
}
