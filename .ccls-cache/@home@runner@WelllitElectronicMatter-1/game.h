#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "ship.h"
#include <vector>

class Game {
public:
  Game();
  void start();

private:
  void placeShips(Board &board, const std::vector<Ship *> &ships, bool isPlayer);
  bool getPlayerAttackCoordinates(int &x, int &y);
  bool getComputerAttackCoordinates(int &x, int &y);

  Board playerBoard;
  Board computerBoard;

  Battleship playerBattleship, computerBattleship;
  Cruiser playerCruiser, computerCruiser;
  Destroyer playerDestroyer, computerDestroyer;
  Submarine playerSubmarine, computerSubmarine;

  std::vector<Ship *> playerShips;
  std::vector<Ship *> computerShips;

  bool playerGetsExtraTurn;
  bool computerGetsExtraTurn;
};

#endif  // GAME_H
