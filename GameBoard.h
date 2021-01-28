#ifndef CHESSPROJECT_GAMEBOARD_H
#define CHESSPROJECT_GAMEBOARD_H
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "Checker.h"
//#include "Player.h"

using namespace std;

class GameBoard {

private:
    Checker GB[8][8];
    Checker last_chkr;
    bool whiteKingInChess;
    bool blackKingInChess;
    bool whiteMadeCastling;
    bool blackMadeCastling;

public:
    GameBoard();
    bool whiteInChess() const;
    bool blackInChess() const;
    Checker& getCheckerByName(string name);
    void setLastChkr(const Checker& chkr);
    Checker& getKingChkr(bool isWhite);
    bool makeMove(Player& player, Checker& sourceChkr, Checker& destChkr, const string& promotion);
    bool isNotSelfChessMove(Checker& sourceChkr, Checker& destChkr, Player& player,const string &pawn_promotion="");
    bool isLegalPlayerMove(Checker& sourceChkr,Checker& destChkr, Player& player, const string &pawn_promotion="");
    bool isLegalPawnMove(Checker& sourceChkr,Checker& destChkr, bool isWhite, const string& promotion);
    bool isLegalQueenMove(Checker& sourceChkr,Checker& destChkr);
    bool isLegalKnightMove(Checker& sourceChkr, Checker& destChkr);
    bool isLegalRookMove(Checker& sourceChkr,Checker& destChkr);
    bool isLegalBishopMove(Checker& sourceChkr,Checker& destChkr);
    bool isLegalKingMove(Checker& sourceChkr,Checker& destChkr, bool isWhite);
    Checker& getNextChckr(Checker& source, Checker& dest);
    bool DiagonalWayIsEmpty(Checker& sourceChkr,Checker& destChkr);
    bool StraightWayIsEmpty(Checker& sourceChkr,Checker& destChkr);
    bool InChess(Checker& kingChkr,Checker& opponentPlayerChkr, bool);
    bool theKingCanSaveHimself(Checker& badChkr, bool whiteKing);
    bool otherPlayerCanSaveKing(bool whiteKing, Checker& badChkr);
    bool whiteHasOnlyKnightOrBishop();
    bool blackHasOnlyKnightOrBishop();
    bool isMaterialInsufficient();
    bool isStalemate();
    bool isCheckMate();
    void printGB();

    virtual ~GameBoard();
};


bool correct_promotion(const string& player);




#endif //CHESSPROJECT_GAMEBOARD_H
