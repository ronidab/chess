#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Player.h"

using namespace std;

Player::Player() {
    player_name = ".";
    white = false;
    num_of_moves = 0;
}
Player::Player(const Player& player){
    player_name=player.player_name;
    white = player.white;
    num_of_moves = player.num_of_moves;
}
Player::Player(string name, bool w) {
    player_name.assign(name);
    white = w;
    num_of_moves = 0;
}
string Player::getName() const {
    return player_name;
}

bool Player::isWhite() const {
    return white;
}

int Player::get_num_of_moves() const{
    return num_of_moves;
}
void Player::add_num_of_moves(){
    //increment num of player's moves
    num_of_moves ++;
}
bool Player::isPawn(){
    return (player_name == "p" || player_name == "P");
}
bool Player::isRook(){
    return (player_name == "r" || player_name == "R");
}
bool Player::isBishop(){
    return (player_name == "b" || player_name == "B");
}
bool Player::isKnight(){
    return (player_name == "n" || player_name == "N");
}
bool Player::isQueen(){
    return (player_name == "q" || player_name == "Q");
}
bool Player::isKing(){
    return (player_name == "k" || player_name == "K");
}
Player& Player::operator=(const Player &copyPlyr){
    player_name.assign(copyPlyr.getName());
    white = copyPlyr.isWhite();
    num_of_moves = copyPlyr.get_num_of_moves();
    return *this;
}

Player::~Player(){}

bool firstKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI - 2 == dstI && srcJ + 1 == dstJ) {
        return true;;
    }
    return false;
}

bool secondKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI - 1 == dstI && srcJ + 2 == dstJ) {
        return true;;
    }
    return false;
}

bool thirdKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI + 1 == dstI && srcJ + 2 == dstJ) {
        return true;;
    }
    return false;
}

bool fourthKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI + 2 == dstI && srcJ + 1 == dstJ) {
        return true;;
    }
    return false;
}

bool fifthKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI + 2 == dstI && srcJ - 1 == dstJ) {
        return true;;
    }
    return false;
}

bool sixthKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI + 1 == dstI && srcJ - 2 == dstJ) {
        return true;;
    }
    return false;
}

bool seventhKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI - 1 == dstI && srcJ - 2 == dstJ) {
        return true;;
    }
    return false;
}

bool eighthKnightChoice(int dstI, int dstJ, int srcI, int srcJ) {
    if (srcI - 2 == dstI && srcJ - 1 == dstJ) {
        return true;;
    }
    return false;
}
