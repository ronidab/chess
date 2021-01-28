#ifndef CHESSPROJECT_PLAYER_H
#define CHESSPROJECT_PLAYER_H
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


using namespace std;

class Player{

private:
    string player_name;
    bool white;
    int num_of_moves;

public:
    Player();
    Player(const Player& player);       //copy constructor
    Player(string name, bool w);
    string getName() const;
    bool isWhite() const;
    int get_num_of_moves() const;
    void add_num_of_moves();
    bool isPawn();
    bool isRook();
    bool isBishop();
    bool isKnight();
    bool isKing();
    bool isQueen();

    Player& operator=(const Player &copyPlyr);

    virtual ~Player();

};

bool firstKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool secondKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool thirdKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool fourthKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool fifthKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool sixthKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool seventhKnightChoice(int dstI, int dstJ, int srcI, int srcJ);
bool eighthKnightChoice(int dstI, int dstJ, int srcI, int srcJ);


#endif //CHESSPROJECT_PLAYER_H
