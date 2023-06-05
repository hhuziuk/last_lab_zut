#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <vector>

#include "board.h"
#include "ship.h"

using namespace std;

void placeShips(Board &board, const vector<Ship *> &ships, bool isPlayer) {
  for (Ship *ship : ships) {
    bool placed = false;
    while (!placed) {
      int x, y;
      char direction;

      auto isValidCoordinate = [](int x, int y) {
        return x >= 0 && x < Board::SIZE && y >= 0 && y < Board::SIZE;
      };

      // Використання лямбда-функції для валідації координат гравця
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

int main() {
  srand(time(0));

  Board playerBoard;
  Board computerBoard;
  playerBoard.saveToFile("player_board.txt", true);
  computerBoard.saveToFile("computer_board.txt", false);

  AircraftCarrier playerAircraftCarrier, computerAircraftCarrier;
  Battleship playerBattleship, computerBattleship;
  Cruiser playerCruiser, computerCruiser;
  Destroyer playerDestroyer, computerDestroyer;
  Submarine playerSubmarine, computerSubmarine;

  vector<Ship *> playerShips = {&playerAircraftCarrier, &playerBattleship,
                                &playerCruiser, &playerDestroyer,
                                &playerSubmarine};

  vector<Ship *> computerShips = {&computerAircraftCarrier, &computerBattleship,
                                  &computerCruiser, &computerDestroyer,
                                  &computerSubmarine};

  placeShips(playerBoard, playerShips, true);
  placeShips(computerBoard, computerShips, false);

  while (true) {
    cout << "Player Board:" << endl;
    playerBoard.print(true);
    cout << endl;

    cout << "Computer Board:" << endl;
    computerBoard.print(false);
    cout << endl;

    int x, y;
    char direction;
    cout << "Enter the coordinates to attack (x y): ";
    cin >> x >> y;

    // Validate the input
    while (cin.fail() || x < 0 || x >= Board::SIZE || y < 0 ||
           y >= Board::SIZE) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Invalid coordinates. Please try again." << endl;
      cout << "Enter the coordinates to attack (x y): ";
      cin >> x >> y;
    }

    bool playerHit = computerBoard.receiveAttack(x, y);
    if (playerHit) {
      cout << "Player hit the computer's ship!" << endl;
      if (computerBoard.isGameOver()) {
        cout << "Player wins!" << endl;
        break;
      }
    } else {
      cout << "Player missed!" << endl;
    }

    // Check if the game is over before the computer attacks
    if (playerBoard.isGameOver()) {
      cout << "Computer wins!" << endl;
      break;
    }

    // Computer's turn
    x = rand() % Board::SIZE;
    y = rand() % Board::SIZE;
    bool computerHit = playerBoard.receiveAttack(x, y);
    if (computerHit) {
      cout << "Computer hit your ship!" << endl;
      if (playerBoard.isGameOver()) {
        cout << "Computer wins!" << endl;
        break;
      }
    } else {
      cout << "Computer missed!" << endl;
    }

    cout << endl;
  }
  playerBoard.saveToFile("player_board.txt", true);
  computerBoard.saveToFile("computer_board.txt", false);

  return 0;
}
