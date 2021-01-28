#ifndef CHESSPROJECT_CHECKER_H
#define CHESSPROJECT_CHECKER_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Player.h"

using namespace std;

class Checker {

private:
    string chkrName;
    Player curr_player;
    int i_index;
    int j_index;

public:
    Checker();
    Checker(const Checker& chkr);
    void setChkrName(string chkr_name);
    void setI(int i);
    void setJ(int j);
    int getI() const;
    int getJ() const;

    string getChkrName() const;
    string getPlayerName() const;
    Player getPlayer() const;
    void setPlayer(Player plyr);
    bool isEmpty() const;
    bool operator!=(Checker& cmpChk) const;
    Checker& operator=(const Checker &copyChk);

    virtual ~Checker();

};

bool legalChkr(const string& Chkr);

#endif //CHESSPROJECT_CHECKER_H
