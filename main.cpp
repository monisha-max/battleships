#include <iostream>
#include <string>
#include <limits>

using namespace std;

const int GAME_BOARD_WIDTH = 10;
const int GAME_BOARD_HEIGHT = 10;
const int NUM_SHIP_TYPES = 5;

const char WATER_SYMBOL = '~';
const char SHIP_SYMBOL = 'S';
const char HIT_SYMBOL = 'H';
const char MISS_SYMBOL = '*';

struct Coordinates {
    int X;
    int Y;
};

struct Ship {
    string name;
    int length;
    Coordinates positions[5];
    bool hitFlag[5];
} ships[NUM_SHIP_TYPES];

struct PlayerBoard {
    char grid[GAME_BOARD_WIDTH][GAME_BOARD_HEIGHT];
} players[3];

enum ShipDirection { HORIZONTAL, VERTICAL };

struct ShipPlacement {
    ShipDirection direction;
    Ship shipType;
};

bool gameInProgress = false;

// Function declarations
void LoadShips();
void ResetGameBoard();
void DrawGameBoard(int);
ShipPlacement GetUserShipPlacement();
bool GetUserAttackCoordinates(int&, int&, int);
bool CheckGameOver(int);

int main()
{
    LoadShips();
    ResetGameBoard();

    for (int currentPlayer = 1; currentPlayer < 3; ++currentPlayer)
    {
        for (int currentShip = 0; currentShip < NUM_SHIP_TYPES; ++currentShip)
        {
            system("cls");
            DrawGameBoard(currentPlayer);

            cout << "\n";
            cout << "PLACING(Player " << currentPlayer << ")\n\n";
            cout << "PLACE YOUR SHIPS ACCORDING TO THIS FORMAT 0 A 1 THIS MEANS 0- HORIZANTAL(1-VERTICAL), A-ROW,1 COLUMN, this will be your starting point :\n";
            cout << "the ship that u are currently placing is " << ships[currentShip].name << " of spaces " << ships[currentShip].length << "\n";
            cout << "where do you want it placed?";

            ShipPlacement shipPlacement;
            shipPlacement.shipType.positions[0].X = -1;
            while (shipPlacement.shipType.positions[0].X == -1)
            {
                shipPlacement = GetUserShipPlacement();
            }

            shipPlacement.shipType.length = ships[currentShip].length;
            shipPlacement.shipType.name = ships[currentShip].name;

            players[currentPlayer].grid[shipPlacement.shipType.positions[0].X][shipPlacement.shipType.positions[0].Y] = SHIP_SYMBOL;

            for (int i = 1; i < shipPlacement.shipType.length; ++i)
            {
                if (shipPlacement.direction == HORIZONTAL) {
                    shipPlacement.shipType.positions[i].X = shipPlacement.shipType.positions[i - 1].X + 1;
                    shipPlacement.shipType.positions[i].Y = shipPlacement.shipType.positions[i - 1].Y;
                }
                if (shipPlacement.direction == VERTICAL) {
                    shipPlacement.shipType.positions[i].Y = shipPlacement.shipType.positions[i - 1].Y + 1;
                    shipPlacement.shipType.positions[i].X = shipPlacement.shipType.positions[i - 1].X;
                }

                players[currentPlayer].grid[shipPlacement.shipType.positions[i].X][shipPlacement.shipType.positions[i].Y] = SHIP_SYMBOL;
            }
        }
    }

    gameInProgress = true;
    int currentPlayer = 1;
    do {
        int opponentPlayer;
        if (currentPlayer == 1) opponentPlayer = 2;
        if (currentPlayer == 2) opponentPlayer = 1;
        system("cls");
        DrawGameBoard(opponentPlayer);

        bool validInput = false;
        int x, y;
        while (!validInput) {
            validInput = GetUserAttackCoordinates(x, y, currentPlayer);
        }

        if (players[opponentPlayer].grid[x][y] == SHIP_SYMBOL) players[opponentPlayer].grid[x][y] = HIT_SYMBOL;
        if (players[opponentPlayer].grid[x][y] == WATER_SYMBOL) players[opponentPlayer].grid[x][y] = MISS_SYMBOL;

        int gameResult = CheckGameOver(opponentPlayer);
        if (gameResult != 0) {
            gameInProgress = false;
            break;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    } while (gameInProgress);

    system("cls");
    cout << "\n\nCONGRATULATIONS!!!PLAYER " << currentPlayer << " HAS WON THE GAME!\n\n\n\n";

    system("pause");
    return 0;
}

bool CheckGameOver(int opponentPlayer)
{
    bool gameOver = true;
    for (int w = 0; w < GAME_BOARD_WIDTH; ++w) {
        for (int h = 0; h < GAME_BOARD_HEIGHT; ++h) {
            if (players[opponentPlayer].grid[w][h] == SHIP_SYMBOL) {
                gameOver = false;
                return gameOver;
            }
        }
    }
    return gameOver;
}

bool GetUserAttackCoordinates(int& x, int& y, int currentPlayer)
{
    cout << "\nPLAYER " << currentPlayer << ",Enter coordinates to attack (e.g., A 1): ";
    char xChar;
    cin >> xChar >> y;

    x = xChar - 'A';

    if (x < 0 || x >= GAME_BOARD_WIDTH || y < 0 || y >= GAME_BOARD_HEIGHT || cin.fail()) {
        cout << "Invalid input. Please enter valid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    if (players[3 - currentPlayer].grid[x][y] == SHIP_SYMBOL) {
        cout << "Hit!\n";
    } else {
        cout << "Miss!\n";
    }

    return true;
}

ShipPlacement GetUserShipPlacement()
{
    int direction, x, y;
    ShipPlacement tmp;
    tmp.shipType.positions[0].X = -1;

    char xChar;
    cin >> direction >> xChar >> y;

    x = xChar - 'A';

    if (direction != 0 && direction != 1 || cin.fail()) {
        cout << "Invalid input. Please enter valid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return tmp;
    }

    if (x < 0 || x >= GAME_BOARD_WIDTH || cin.fail()) {
        cout << "Invalid input. Please enter valid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return tmp;
    }

    if (y < 0 || y >= GAME_BOARD_HEIGHT || cin.fail()) {
        cout << "Invalid input. Please enter valid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return tmp;
    }

    tmp.direction = static_cast<ShipDirection>(direction);
    tmp.shipType.positions[0].X = x;
    tmp.shipType.positions[0].Y = y;
    return tmp;
}

void LoadShips()
{
    ships[0].name = "Frigate"; ships[0].length = 1;
    ships[1].name = "Corvette"; ships[1].length = 2;
    ships[2].name = "Destroyer"; ships[2].length = 3;
    ships[3].name = "Battleship"; ships[3].length = 4;
    ships[4].name = "Aircraft carrier"; ships[4].length = 5;
}

void ResetGameBoard()
{
    for (int playerNum = 1; playerNum < 3; ++playerNum)
    {
        for (int w = 0; w < GAME_BOARD_WIDTH; ++w) {
            for (int h = 0; h < GAME_BOARD_HEIGHT; ++h) {
                players[playerNum].grid[w][h] = WATER_SYMBOL;
            }
        }
    }
}

void DrawGameBoard(int currentPlayer)
{
    cout << "Player " << currentPlayer << "'s GAME BOARD\n";
    cout << "----------------------\n";

    cout << "   ";
    for (int w = 0; w < GAME_BOARD_WIDTH; ++w) {
        char alphabet = 'A' + w;
        cout << alphabet << "  ";
    }
    cout << "\n";

    for (int h = 0; h < GAME_BOARD_HEIGHT; ++h) {
        for (int w = 0; w < GAME_BOARD_WIDTH; ++w) {
            if (w == 0) cout << h << " ";

            if (!gameInProgress) cout << players[currentPlayer].grid[w][h] << "  ";

            if (gameInProgress && players[currentPlayer].grid[w][h] != SHIP_SYMBOL)
            {
                cout << players[currentPlayer].grid[w][h] << "  ";
            }
            else if (gameInProgress && players[currentPlayer].grid[w][h] == SHIP_SYMBOL)
            {
                cout << WATER_SYMBOL << "  ";
            }

            if (w == GAME_BOARD_WIDTH - 1) cout << "\n";
        }
    }
}
