#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <vector>

#include "game.h"

using namespace std;

int main() {
  srand(time(0));

  Game game;
  game.start();

  return 0;
}
