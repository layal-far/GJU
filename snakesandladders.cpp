#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int WINNING_POSITION = 100;

//board setup
map<int, int> snakes;
map<int, int> ladders;

//function to roll a dice (6-sided)
int rollDice() {
    return rand() % 6 + 1;
}

//function to generate random snakes and ladders
void generateSnakesAndLadders(int numSnakes, int numLadders) {
    snakes.clear();
    ladders.clear();
    vector<int> usedPositions;

    auto isPositionUsed = [&](int pos) {
        return find(usedPositions.begin(), usedPositions.end(), pos) != usedPositions.end();
    };

    //generate snakes
    for (int i = 0; i < numSnakes; i++) {
        int head, tail;
        do {
            head = rand() % (WINNING_POSITION - 10) + 11; // heads between 11 and 99
            tail = rand() % (head - 1) + 1;               // tails below heads
        } while (isPositionUsed(head) || isPositionUsed(tail) || snakes.count(head) > 0);

        snakes[head] = tail;
        usedPositions.push_back(head);
        usedPositions.push_back(tail);
    }

    //generate ladders
    for (int i = 0; i < numLadders; i++) {
        int start, end;
        do {
            start = rand() % (WINNING_POSITION - 20) + 1;          // starts between 1 and 80
            end = rand() % (WINNING_POSITION - start) + start + 1; // ends above start
        } while (isPositionUsed(start) || isPositionUsed(end) || ladders.count(start) > 0);

        ladders[start] = end;
        usedPositions.push_back(start);
        usedPositions.push_back(end);
    }
}

//function to check for snakes or ladders
int checkForSnakesOrLadders(int position) {
    if (snakes.find(position) != snakes.end()) {
        cout << "Uh-Oh! You got bitten by a snake. Going down from " << position << " to " << snakes[position] << endl;
        return snakes[position];
    }
    if (ladders.find(position) != ladders.end()) {
        cout << "Yay! You climbed a ladder from " << position << " to " << ladders[position] << endl;
        return ladders[position];
    }

    return position;
}

//players
class Player {
public:
    string name;
    int position;

    Player(string name) : name(name), position(0) {}

    //function to make a move
    void move() {
        cout << name << ", press Enter to roll the dice...";
        cin.ignore(); //wait
        int diceRoll = rollDice();
        cout << name << " rolled a " << diceRoll << endl;
        position += diceRoll;

        if (position > WINNING_POSITION) {
            position -= diceRoll;  //don't move if it exceeds board
            cout << name << " stays at " << position << ". Can't move beyond " << WINNING_POSITION << "." << endl;
            return;
        }

        position = checkForSnakesOrLadders(position);
        cout << name << " is now on position " << position << endl;
    }

    bool hasWon() const {
        return position == WINNING_POSITION;
    }
};

//main function
void playGame() {
    cout << "Welcome to Snakes and Ladders!\n";

    //get player names
    string name1, name2;
    cout << "Player 1, state your name: ";
    cin >> name1;
    cout << "Player 2, state your name: ";
    cin >> name2;
    cin.ignore(); // clear buffer
    Player player1(name1), player2(name2);
    Player* currentPlayer = &player1;

    //generate snakes and ladders
    int numSnakes = rand() % 5 + 1;   //random number of snakes between 1 and 5
    int numLadders = rand() % 5 + 1;  //random number of ladders between 1 and 5
    generateSnakesAndLadders(numSnakes, numLadders);
    cout << "\nThis game has " << numSnakes << " snakes and " << numLadders << " ladders.\n";
    cout << "The positions of snakes and ladders have been randomly selected.\n";
    cout << "---------------------------------\n";

    //main game loop
    while (true) {
        cout << currentPlayer->name << "'s turn.\n";
        currentPlayer->move();

        if (currentPlayer->hasWon()) {
            cout << currentPlayer->name << " has won the game!\n";
            break;
        }

        //switch player
        currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
        cout << "---------------------------------\n";
    }
}

//main program loop (replay or quit?)
int main() {
    srand(static_cast<unsigned int>(time(0)));  //randomness seed

    while (true) {
        playGame();

        char choice;
        cout << "Would you like to play again? (y/n): ";
        cin >> choice;

        if (choice == 'n' || choice == 'N') {
            cout << "Thank you for playing! Goodbye!\n";
            break;
        }

        cin.ignore(); //clear buffer
        cout << "\nStarting new game...\n";
    }

    return 0;
}
