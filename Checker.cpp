#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include "Checker.h"
#include "Player.h"

using namespace std;

Checker::Checker() {
    curr_player = Player();
    i_index = -1;
    j_index = -1;
}
Checker::Checker(const Checker& chkr){
    chkrName.assign(chkr.chkrName);
    curr_player = chkr.getPlayer();
    i_index = chkr.getI();
    j_index = chkr.getJ();

}
void Checker::setI(int i){
    i_index = i;
}
void Checker::setJ(int j){
    j_index = j;
}
int Checker::getI() const{
    return i_index;
}
int Checker::getJ() const{
    return j_index;
}

void Checker::setChkrName(string chkr_name) {
    chkrName = chkr_name;
}

string Checker::getChkrName() const{
    return chkrName;
}

Player Checker::getPlayer() const{
    return curr_player;
}

string Checker::getPlayerName() const {
    return curr_player.getName();
}

void Checker::setPlayer(Player plyr) {
    curr_player = plyr;

}
bool Checker::isEmpty() const{
    return (curr_player.getName() == ".");
}
bool Checker::operator!=(Checker& cmpChk) const{
    return (cmpChk.getI()!=i_index || cmpChk.getJ()!=j_index);
}
Checker& Checker::operator=(const Checker &rhs){
   chkrName=rhs.getChkrName();
   curr_player = rhs.getPlayer();
   i_index = rhs.getI();
   j_index = rhs.getJ();
   return *this;
}

Checker::~Checker() {}

bool legalChkr(const string &Chkr) {
    if (Chkr.length() != 2) {
        return false;
    }
    char first_char, second_char;
    stringstream ss(Chkr);
    first_char = ss.get();
    if (first_char < 'A' || first_char > 'H') {
        return false;
    }
    second_char = ss.get();
    if (second_char < '0' || second_char > '8') {
        return false;
    }
    return true;
}
