#ifndef SHIP_H
#define SHIP_H

#include <ostream>

class Ship {
public:
  virtual ~Ship(){};
  virtual char getSymbol() const = 0;
  virtual int getLength() const = 0;
};

class Battleship : public Ship {
public:
  char getSymbol() const override { return 'B'; }
  int getLength() const override { return 4; }
};

class Cruiser : public Ship {
public:
  char getSymbol() const override { return 'C'; }
  int getLength() const override { return 3; }
};

class Destroyer : public Ship {
public:
  char getSymbol() const override { return 'D'; }
  int getLength() const override { return 2; }
};

class Submarine : public Ship {
public:
  char getSymbol() const override { return 'S'; }
  int getLength() const override { return 1; }
};

#endif
