#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "ship.h"

class Board {
public:
  static const int SIZE = 10;

  Board();

  friend std::ostream &operator<<(std::ostream &os, const Board &board);

  void clear();

  bool placeShip(int x, int y, Ship *ship, char direction);

  void print(bool showShips) const;
  void saveToFile(std::string filename, bool showShips) const;

  bool receiveAttack(int x, int y);

  bool isGameOver() const;

private:
  std::vector<std::vector<Ship *>> grid;
  std::vector<std::vector<bool>> hits;

  bool canPlaceShip(int x, int y, int length, char direction) const;
};

#endif  
