#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "string.h"
#include "GameBoard.h"

using namespace std;

int main() {
    //starting game:
    GameBoard Gb = GameBoard();
    bool Checkmate = false;
    bool white_turn = true;
    bool again_loop = false;
    int counter = 1;

    while (!Checkmate) {
        if (!again_loop) {
            Gb.printGB();      //prints board
            if (white_turn) {
                cout << counter << ") White's turn, please enter a move:" << endl;
            } else {
                cout << counter << ") Black's turn, please enter a move:" << endl;
            }
        }
        string curr_move;
        string startChk;
        string endChk;
        string pawn_promotion;
        getline(cin, curr_move);
        if(!cin){ return 0; }
        stringstream ss(curr_move);
        ss >> startChk;
        ss >> endChk;
        ss >> pawn_promotion;
        //legal input checks:
        if (!legalChkr(startChk) || !legalChkr(endChk)) {
            cerr << counter << ") Invalid input; please enter a move:" << endl;
            again_loop = true;
            continue;
        }
        Player currPlayer = Gb.getCheckerByName(startChk).getPlayer();
        if (currPlayer.isWhite() != white_turn) {
            cerr << counter << ") Illegal move; please enter a move:" << endl;
            again_loop = true;
            continue;
        }
        if (!Gb.makeMove(currPlayer, Gb.getCheckerByName(startChk), Gb.getCheckerByName(endChk), pawn_promotion)) {
            cerr << counter << ") Illegal move; please enter a move:" << endl;
            again_loop = true;
            continue;
        }

        if (Gb.isCheckMate()) {
            Checkmate = true;
            Gb.printGB();
            if (Gb.blackInChess()) {
                cout << "White wins with checkmate!" << endl;
            } else {
                cout << "Black wins with checkmate!" << endl;
            }
        }
        else if (Gb.isStalemate()) {
            Checkmate = true;
            Gb.printGB();
            cout << "The game is drawn due to stalemate!" << endl;
        }
        else if (Gb.isMaterialInsufficient()) {
            Checkmate = true;
            Gb.printGB();
            cout << "The game is drawn due to insufficient material!" << endl;
        }

        again_loop = false;
        white_turn = !white_turn;
        counter++;
        //Making change
        //Double change
    }
    return 0;
}
