/*      The code's objective is to implement a two-player Battleship game. In the initial phase, Player 1 and Player 2 
each arrange their fleet of ships on their respective grids. Following this setup, the game enters the guessing phase
 where players take turns providing coordinates to target their opponent's ships. If a given coordinate matches the location
  of an opponent's ship, the response is "Hit!", else "Miss!" The game displays two boards during this phase: one showing the player's own 
  grid and the other indicating the hits received by the current player from the opponent.
Additionally, when a player successfully hits all the segments of a particular ship, the message "Hit and Sunk" is displayed 
along with the type of ship that got sunk. The game continues until all the ships of one player are sunk, at which point a 
congratulatory message is presented, declaring the winning player     */


#include<iostream>
#include<string>
#include<limits>
using namespace std;
//dimensions of the board
const int boardWidth=10;
const int boardHeight=10;
const int numShipTypes=5;
struct Coord {
    int x;
    int y;
};
struct Ship {//defining the attributes of the ship
    string name;
    int spaces;
    char symbol;
    Coord positions[5];
    bool allHit;
} ships[numShipTypes];
enum Direction {HORIZONTAL,VERTICAL};
struct ShipPlacement {
    Direction dir;
    Ship shipType;
};
//we need to two boards, one for self and one to keep track of hits and misses from opponent player
struct PlayerObject { 
    char selfgrid[boardWidth][boardHeight]; //data grid where we maintain actual ship positions
    char opponentgrid[boardWidth][boardHeight];//grid to keep track of hits and misses from opponent player
    ShipPlacement shipPlacements[numShipTypes] ;

} players[3];
//the symbols
const char water='~';
const char hit='H';
const char miss='*';

bool gameInProgress = false;
// given 5 ships, with its names, its spaces and its symbol
void initializeShips(){ 
    ships[0].name = "Frigate"; ships[0].spaces = 1; ships[0].symbol = 'F';
    ships[1].name = "Corvette"; ships[1].spaces = 2; ships[1].symbol = 'C';
    ships[2].name = "Destroyer"; ships[2].spaces = 3; ships[2].symbol = 'D';
    ships[3].name = "Battleship"; ships[3].spaces = 4; ships[3].symbol = 'B';
    ships[4].name = "Aircraft carrier"; ships[4].spaces = 5; ships[4].symbol = 'A';
}
//setting the cells for both the player's own grid and the opponent's grid to water.
void resetGameBoard(){ 
    for (int playerNum = 1; playerNum < 3; ++playerNum) {
        for (int w = 0; w < boardWidth; ++w) {
            for (int h = 0; h < boardHeight; ++h) {
                players[playerNum].selfgrid[w][h] = water;
                players[playerNum].opponentgrid[w][h] = water;
            }
        }
    }
}
//bool self: Boolean indicating whether to display the player's own grid (self=true) or the opponent's grid (self=false)
void displayGameBoard(int currentPlayer, bool self){ 
    for (int col = 0; col < boardWidth; ++col) {
        char alphabet = 'A' + col; 
        // If it's the first column, display the row label
        cout << alphabet << "  ";
    }
    cout << "\n";
    
    for (int row = 0; row < boardHeight; ++row) {
        for (int col = 0; col < boardWidth; ++col) {
            if (col == 0) cout << row << "  "; // representing the column label
            char cellSymbol;
            if (self == true){
              cellSymbol = players[currentPlayer].selfgrid[col][row];
            } else {
                cellSymbol = players[currentPlayer].opponentgrid[col][row];
            } 

            cout << cellSymbol << "  ";
            if (col == boardWidth - 1) cout << "\n";
        }
    }
    
}

ShipPlacement getUserShipPlacement(){
    int dir, x, y;
    // Initialize a ShipPlacement structure to store user input
    ShipPlacement userShipPlacement;
    userShipPlacement.shipType.positions[0].x = -1;
    char xChar;
    cin >> dir >> xChar >> y;
    x = xChar - 'A';
    // Checking for invalid input conditions
    if (dir != 0 && dir != 1 || cin.fail() || x < 0 || x >= boardWidth || cin.fail() || y < 0 || y >= boardHeight || cin.fail()) {
        cout << "Invalid input. Please enter valid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return userShipPlacement;
    }
    // Setting the direction
    userShipPlacement.dir = static_cast<Direction>(dir);
    // Setting the x and y coordinates in the ShipPlacement structure
    userShipPlacement.shipType.positions[0].x = x;
    userShipPlacement.shipType.positions[0].y = y;
    return userShipPlacement;
}


bool getUserAttackCoordinates(int& x, int& y, int currentPlayer){
    cout << "\nplayer " << currentPlayer << ", enter the row and column to hit";
    char xChar;
    cin >> xChar >> y;

    x = xChar - 'A';

    if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight || cin.fail()) {
        cout << "Invalid input. Please enter valid input.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    char current_symbol = players[3 - currentPlayer].selfgrid[x][y];
    //checking the grid, if the coordinate given by player is equal to the below symbols, then it's a hit
    if ( current_symbol == 'F' || current_symbol == 'C' || current_symbol == 'D' || 
        current_symbol == 'B' || current_symbol == 'A') {
        cout << "Hit!\n";
        players[currentPlayer].opponentgrid[x][y] = hit;
        //continuation, if it's a hit and then checking if a ship is sunken or not 
        for (int shipIndex = 0; shipIndex < numShipTypes; ++shipIndex) {
            // 3 - currentPlayer represents the other player since there are only 2 players
            ShipPlacement& placement = players[3 - currentPlayer].shipPlacements[shipIndex];
            if(current_symbol == placement.shipType.symbol){
             bool allHit = true;
             //checking all the spaces of a particular ship if it's equal to the one we found with hit.
             for (int i = 0; i < placement.shipType.spaces; ++i) {
                int shipX = placement.shipType.positions[i].x;
                int shipY = placement.shipType.positions[i].y;
                 
               
                if (players[currentPlayer].opponentgrid[shipX][shipY] != hit) {
                    allHit = false;
                    break;  // No need to check further once the ship is found
                }
             }
             if(allHit){
                placement.shipType.allHit = true;
                cout << "Hit and Sunk! Player  " << currentPlayer << " sunk " << placement.shipType.name <<  " of Player " << 3 - currentPlayer << "!\n";
             }
            }
        }
        

    } else {
        cout << "Miss!\n";
        players[currentPlayer].opponentgrid[x][y] = miss;
    }

    return true;
}

bool isGameOver(int currentPlayer) { //finally seeing who is going to win, by checking if the spaces of the ships are all ocuupied by H's.
    
    bool allHit = true;
    for (int shipIndex = 0; shipIndex < numShipTypes; ++shipIndex){
            
            ShipPlacement& placement = players[3 - currentPlayer].shipPlacements[shipIndex];
             if(!placement.shipType.allHit){
                 allHit = false;
               
             }
    }
    if(allHit){
      cout << "congrats " << currentPlayer << " won"  << "!\n";
    }
    
    return allHit;
}

int main(){
    initializeShips();
    resetGameBoard();
    cout << "                 ------------------  PLANNING PHASE  -----------------------";
    for (int currentPlayer = 1; currentPlayer < 3; ++currentPlayer) {
        for (int currentShip = 0; currentShip < numShipTypes; ++currentShip) {
            cout << "\n";
            cout << "Planning phase of Player " << currentPlayer << ":\n"
                 << "PLACE YOUR SHIPS ACCORDING TO THIS FORMAT 0 A 1 (0-HORIZONTAL, 1-VERTICAL), A-ROW, 1-COLUMN, this will be your starting point :\n"
                 << "the ship that you are currently placing is " << ships[currentShip].name << " of spaces " << ships[currentShip].spaces << "\n"
                 << "where do you want it placed?";

            ShipPlacement shipPlacement = getUserShipPlacement();;

            shipPlacement.shipType.spaces = ships[currentShip].spaces;
            shipPlacement.shipType.name = ships[currentShip].name;
            shipPlacement.shipType.symbol = ships[currentShip].symbol;
            //first placement is processed here.
            players[currentPlayer].selfgrid[shipPlacement.shipType.positions[0].x][shipPlacement.shipType.positions[0].y] = shipPlacement.shipType.symbol;
            //from second placement, it's processed from here.
            //if horizantal the row should be +1 and column does not change, and vice versa.
            for (int i = 1; i < shipPlacement.shipType.spaces; ++i) {
                if (shipPlacement.dir == HORIZONTAL) {
                    shipPlacement.shipType.positions[i].x = shipPlacement.shipType.positions[i - 1].x + 1;
                    shipPlacement.shipType.positions[i].y = shipPlacement.shipType.positions[i - 1].y;
                }
                if (shipPlacement.dir == VERTICAL) {
                    shipPlacement.shipType.positions[i].y = shipPlacement.shipType.positions[i - 1].y + 1;
                    shipPlacement.shipType.positions[i].x = shipPlacement.shipType.positions[i - 1].x;
                }
                // if found, we are representing the cells with their ship symbols.
                players[currentPlayer].selfgrid[shipPlacement.shipType.positions[i].x][shipPlacement.shipType.positions[i].y] 
                = shipPlacement.shipType.symbol;
            }

            players[currentPlayer].shipPlacements[currentShip] = shipPlacement;//ASSOCIATING TO PLAYER'S GRID
              cout << "\n";
              cout << "Player " << currentPlayer << "'s board\n";
              cout << "   ";
            displayGameBoard(currentPlayer, true);
        }
    }
    gameInProgress = true;
    int currentPlayer = 1;
    cout << "                 ------------------  GUESSING PHASE  -----------------------";
    do {
        int opponentPlayer;
        if (currentPlayer == 1) {
            opponentPlayer = 2;
        }
        if (currentPlayer == 2) {
            opponentPlayer = 1;
        }
        cout << "\n\n";
    

        bool validInput = false;
        int x, y;
        while (!validInput) {
            validInput = getUserAttackCoordinates(x, y, currentPlayer);
        }

        cout<< "Current Playing Player "<< currentPlayer << " self board";
        cout << "\n";
        displayGameBoard(currentPlayer, true);
        cout<< "Current Playing Player "<< currentPlayer << " getting hits from Opponent Player " << opponentPlayer ;
        cout << "\n";
        displayGameBoard(opponentPlayer, false);
        if(isGameOver(currentPlayer)){
            break;
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    } while (gameInProgress);
    return 0;
}
