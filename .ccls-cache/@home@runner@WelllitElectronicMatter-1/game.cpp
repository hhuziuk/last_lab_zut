#include "game.h"
#include <iostream>
#include <limits>

using namespace std;

Game::Game() {
  playerBoard.saveToFile("player_board.txt", true);
  computerBoard.saveToFile("computer_board.txt", false);

  playerShips = {&playerBattleship, &playerCruiser,   &playerCruiser,
                 &playerDestroyer,  &playerDestroyer, &playerDestroyer,
                 &playerSubmarine,  &playerSubmarine, &playerSubmarine,
                 &playerSubmarine};

  computerShips = {&computerBattleship, &computerCruiser,   &computerCruiser,
                   &computerDestroyer,  &computerDestroyer, &computerDestroyer,
                   &computerSubmarine,  &computerSubmarine, &computerSubmarine,
                   &computerSubmarine};

  playerGetsExtraTurn = false;
  computerGetsExtraTurn = false;
}

void Game::start() {
  placeShips(playerBoard, playerShips, true);
  placeShips(computerBoard, computerShips, false);

  while (true) {
    cout << "Player Board:" << endl;
    playerBoard.print(true);
    cout << "\033[0m" << endl;

    cout << "Computer Board:" << endl;
    computerBoard.print(false);
    cout << "\033[0m" << endl; // Додано символ скидання форматування

    if (!playerGetsExtraTurn) {
      int x, y;
      cout << "Enter the coordinates to attack (x y): ";
      bool validCoordinates = getPlayerAttackCoordinates(x, y);

      while (!validCoordinates) {
        cout << "Invalid coordinates. Please try again." << endl;
        cout << "Enter the coordinates to attack (x y): ";
        validCoordinates = getPlayerAttackCoordinates(x, y);
      }

      bool playerHit = computerBoard.receiveAttack(x, y);
      if (playerHit) {
        cout << "Player hit the computer's ship!" << endl;
        if (computerBoard.isGameOver()) {
          cout << "Player wins!" << endl;
          break;
        }
        playerGetsExtraTurn = true;
        continue;
      } else {
        cout << "Player missed!" << endl;
      }
    } else {
      playerGetsExtraTurn = false;
    }

    if (playerBoard.isGameOver()) {
      cout << "Computer wins!" << endl;
      break;
    }

    if (!computerGetsExtraTurn) {
      int x, y;
      bool validCoordinates = getComputerAttackCoordinates(x, y);

      while (!validCoordinates) {
        validCoordinates = getComputerAttackCoordinates(x, y);
      }

      bool computerHit = playerBoard.receiveAttack(x, y);
      if (computerHit) {
        cout << "Computer hit your ship!" << endl;
        if (playerBoard.isGameOver()) {
          cout << "Computer wins!" << endl;
          break;
        }
        computerGetsExtraTurn = true;
        continue;
      } else {
        cout << "Computer missed!" << endl;
      }
    } else {
      computerGetsExtraTurn = false;
    }

    cout << endl;
  }
  playerBoard.saveToFile("player_board.txt", true);
  computerBoard.saveToFile("computer_board.txt", false);
}

void Game::placeShips(Board &board, const vector<Ship *> &ships,
                      bool isPlayer) {
  for (Ship *ship : ships) {
    bool placed = false;
    while (!placed) {
      int x, y;
      char direction;

      auto isValidCoordinate = [](int x, int y) {
        return x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE;
      };

      if (isPlayer) {
        cout << "Enter the starting coordinates (x y) for the ship "
             << ship->getSymbol() << " (" << ship->getLength() << "): ";
        cin >> x >> y;

        cout << "Enter the direction (H for horizontal, V for vertical): ";
        cin >> direction;

        while (cin.fail() || !isValidCoordinate(x, y) ||
               (direction != 'H' && direction != 'V')) {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
          cout << "Invalid input. Please try again." << endl;
          cout << "Enter the starting coordinates (x y) for the ship "
               << ship->getSymbol() << " (" << ship->getLength() << "): ";
          cin >> x >> y;

          cout << "Enter the direction (H for horizontal, V for vertical): ";
          cin >> direction;
        }
      } else {
        x = rand() % Board::SIZE;
        y = rand() % Board::SIZE;
        direction = (rand() % 2 == 0) ? 'H' : 'V';
      }

      placed = board.placeShip(x, y, ship, direction);
      if (!placed && isPlayer) {
        cout << "Invalid ship placement. Please try again." << endl;
      }
    }

    if (isPlayer) {
      board.print(true);
    }
  }
}

bool Game::getPlayerAttackCoordinates(int &x, int &y) {
  cin >> x >> y;
  bool validCoordinates =
      !cin.fail() && x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE;

  if (!validCoordinates) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  return validCoordinates;
}

bool Game::getComputerAttackCoordinates(int &x, int &y) {
  x = rand() % Board::SIZE;
  y = rand() % Board::SIZE;
  return true;
}
