#include "board.h"
#include <fstream>

Board::Board() {
  // Initialize the grid and hits matrices
  grid.resize(Board::SIZE, std::vector<Ship *>(Board::SIZE, nullptr));
  hits.resize(Board::SIZE, std::vector<bool>(Board::SIZE, false));
}

std::ostream &operator<<(std::ostream &os, const Board &board) {
  os << " \033[1;31m0 1 2 3 4 5 6 7 8 9\033[0m" << std::endl;
  for (int y = 0; y < Board::SIZE; ++y) {
    os << "\033[1;31m" << y << " \033[0m";
    for (int x = 0; x < Board::SIZE; ++x) {
      if (board.hits[y][x]) {
        if (board.grid[y][x] != nullptr) {
          os << "\033[1;31m" << board.grid[y][x]->getSymbol() << "\033[0m";
        } else {
          os << "\033[1;34mX\033[0m";
        }
      } else {
        if (board.grid[y][x] != nullptr) {
          os << "\033[1;37m.\033[0m";
        } else {
          os << "~";
        }
      }
      os << " ";
    }
    os << std::endl;
  }
  return os;
}

void Board::clear() {
  // Cleanup the grid and hits matrices
  for (int y = 0; y < SIZE; ++y) {
    for (int x = 0; x < SIZE; ++x) {
      grid[y][x] = nullptr;
      hits[y][x] = false;
    }
  }
}

bool Board::placeShip(int x, int y, Ship *ship, char direction) {
  int length = ship->getLength();
  if (canPlaceShip(x, y, length, direction)) {
    for (int i = 0; i < length; ++i) {
      int newX = x;
      int newY = y;
      if (direction == 'H') {
        newX += i;
      } else if (direction == 'V') {
        newY += i;
      }
      grid[newY][newX] = ship;
    }
    return true;
  }
  return false;
}

void Board::print(bool showShips) const {
  std::cout << " \033[1;31m0 1 2 3 4 5 6 7 8 9\033[0m" << std::endl;
  for (int y = 0; y < SIZE; ++y) {
    std::cout << "\033[1;31m" << y << " \033[0m";
    for (int x = 0; x < SIZE; ++x) {
      if (hits[y][x]) {
        if (grid[y][x] != nullptr) {
          std::cout << "\033[1;31m" << grid[y][x]->getSymbol() << " ";
        } else {
          std::cout << "\033[1;31mX ";
        }
      } else if (showShips && grid[y][x] != nullptr) {
        std::cout << "\033[1;31m" << grid[y][x]->getSymbol() << " ";
      } else {
        std::cout << "\033[1;32m~ ";
      }
    }
    std::cout << std::endl;
  }
}

void Board::saveToFile(std::string filename, bool showShips) const {
  std::ofstream file(filename);
  if (file.is_open()) {
    file << " 0 1 2 3 4 5 6 7 8 9" << std::endl;
    for (int y = 0; y < SIZE; ++y) {
      file << y << " ";
      for (int x = 0; x < SIZE; ++x) {
        if (hits[y][x]) {
          if (grid[y][x] != nullptr) {
            file << grid[y][x]->getSymbol() << " ";
          } else {
            file << "X ";
          }
        } else if (showShips && grid[y][x] != nullptr) {
          file << grid[y][x]->getSymbol() << " ";
        } else {
          file << "~ ";
        }
      }
      file << std::endl;
    }
    file.close();
    std::cout << "Board saved to " << filename << std::endl;
  } else {
    std::cout << "Unable to open file " << filename << std::endl;
  }
}

bool Board::receiveAttack(int x, int y) {
  if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
    return false;
  }

  if (hits[y][x]) {
    return false;
  }

  hits[y][x] = true;
  if (grid[y][x] != nullptr) {
    return true;
  }

  return false;
}

bool Board::isGameOver() const {
  for (int y = 0; y < SIZE; ++y) {
    for (int x = 0; x < SIZE; ++x) {
      if (grid[y][x] != nullptr && !hits[y][x]) {
        return false;
      }
    }
  }
  return true;
}

bool Board::canPlaceShip(int x, int y, int length, char direction) const {
  if (x < 0 || x + length > SIZE || y < 0 || y + length > SIZE) {
    return false;
  }

  if (direction == 'H') {
    if (x + length > SIZE) {
      return false;
    }
    for (int i = 0; i < length; ++i) {
      if (grid[y][x + i] != nullptr) {
        return false;
      }
      if (x + i > 0 && grid[y][x + i - 1] != nullptr) {
        return false;
      }
      if (x + i < SIZE - 1 && grid[y][x + i + 1] != nullptr) {
        return false;
      }
      if (y > 0 && grid[y - 1][x + i] != nullptr) {
        return false;
      }
      if (y < SIZE - 1 && grid[y + 1][x + i] != nullptr) {
        return false;
      }
    }
  } else if (direction == 'V') {
    if (y + length > SIZE) {
      return false;
    }
    for (int i = 0; i < length; ++i) {
      if (grid[y + i][x] != nullptr) {
        return false;
      }
      if (y + i > 0 && grid[y + i - 1][x] != nullptr) {
        return false;
      }
      if (y + i < SIZE - 1 && grid[y + i + 1][x] != nullptr) {
        return false;
      }
      if (x > 0 && grid[y + i][x - 1] != nullptr) {
        return false;
      }
      if (x < SIZE - 1 && grid[y + i][x + 1] != nullptr) {
        return false;
      }
    }
  }

  return true;
}