#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include "GameBoard.h"

using namespace std;

GameBoard::GameBoard() {
    last_chkr = Checker();
    whiteKingInChess = false;
    blackKingInChess = false;
    whiteMadeCastling = false;
    blackMadeCastling = false;

    //set checkers names in board:
    char num = '8';
    for (int i = 0; i < 8; i++) {
        char letter = 'A';
        for (int j = 0; j < 8; j++) {
            string curr_name = "";
            curr_name += letter;
            curr_name += num;

            GB[i][j].setChkrName(curr_name);
            GB[i][j].setI(i);
            GB[i][j].setJ(j);
            letter += 1;
        }
        num--;
        cout << endl;

    }
    GB[0][0].setPlayer(Player("R", false));
    GB[0][1].setPlayer(Player("N", false));
    GB[0][2].setPlayer(Player("B", false));
    GB[0][3].setPlayer(Player("Q", false));
    GB[0][4].setPlayer(Player("K", false));
    GB[0][5].setPlayer(Player("B", false));
    GB[0][6].setPlayer(Player("N", false));
    GB[0][7].setPlayer(Player("R", false));

    //Black Pawns Loop:
    for (int i = 0; i < 8; i++) {
        GB[1][i].setPlayer(Player("P", false));
    }

    //Set White Players In Board
    GB[7][0].setPlayer(Player("r", true));
    GB[7][1].setPlayer(Player("n", true));
    GB[7][2].setPlayer(Player("b", true));
    GB[7][3].setPlayer(Player("q", true));
    GB[7][4].setPlayer(Player("k", true));
    GB[7][5].setPlayer(Player("b", true));
    GB[7][6].setPlayer(Player("n", true));
    GB[7][7].setPlayer(Player("r", true));

    //White Pawns Loop:
    for (int i = 0; i < 8; i++) {
        GB[6][i].setPlayer(Player("p", true));
    }
}

Checker &GameBoard::getCheckerByName(string name) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (GB[i][j].getChkrName() == name) {
                return GB[i][j];
            }
        }
    }
    return GB[0][0];
}

bool GameBoard::whiteInChess() const {
    return whiteKingInChess;
}

bool GameBoard::blackInChess() const {
    return blackKingInChess;
}

void GameBoard::setLastChkr(const Checker &chkr) {
    last_chkr = Checker(chkr);
}

Checker &GameBoard::getKingChkr(bool isWhite) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((isWhite && GB[i][j].getPlayerName() == "k") || (!isWhite && GB[i][j].getPlayerName() == "K")) {
                return GB[i][j];
            }
        }
    }
    return GB[0][0];
}

bool GameBoard::isLegalPlayerMove(Checker &sourceChkr, Checker &destChkr, Player &player,
                                  const string &pawn_promotion) {
    if (sourceChkr.isEmpty()) {
        return false;
    }
    if (!destChkr.isEmpty()) {
        if (player.isWhite() == destChkr.getPlayer().isWhite()) {   //player cannot eat himself
            return false;
        }
    }
    if (player.isQueen()) {     //queen case
        if (!isLegalQueenMove(sourceChkr, destChkr)) {
            return false;
        }
    } else if (player.isKnight()) {      //knight case
        if (!isLegalKnightMove(sourceChkr, destChkr)) {
            return false;
        }
    } else if (player.isBishop()) {      //bishop case
        if (!isLegalBishopMove(sourceChkr, destChkr)) {
            return false;
        }
    } else if (player.isRook()) {  //rook case
        if (!isLegalRookMove(sourceChkr, destChkr)) {
            return false;
        }
    } else if (player.isPawn()) {      //pawn case
        if (!isLegalPawnMove(sourceChkr, destChkr, player.isWhite(), pawn_promotion)) {
            return false;
        }
    } else if (player.isKing()) {      //king case
        if (!isLegalKingMove(sourceChkr, destChkr, player.isWhite())) {
            return false;
        }
    }
    if (isNotSelfChessMove(sourceChkr, destChkr, player)) {
        return true;
    }

    return false;
}

bool GameBoard::makeMove(Player &player, Checker &sourceChkr, Checker &destChkr, const string &pawn_promotion) {
    if (!pawn_promotion.empty()) {
        if (!correct_promotion(pawn_promotion)) {
            return false;
        }
        if (!player.isPawn()) { //if there is promotion then it must be pawn player
            return false;
        }
    }

    if (!isLegalPlayerMove(sourceChkr, destChkr, player, pawn_promotion)) {
        return false;
    }

    //if get here, then the move is legal
    player.add_num_of_moves();
    GB[destChkr.getI()][destChkr.getJ()].setPlayer(player);
    GB[sourceChkr.getI()][sourceChkr.getJ()].setPlayer(Player());
    if (!pawn_promotion.empty()) {
        //make white promotion
        if (player.isWhite()) {
            string white_promotion;
            if (pawn_promotion == "Q")white_promotion = "q";
            else if (pawn_promotion == "R")white_promotion = "r";
            else if (pawn_promotion == "N")white_promotion = "n";
            else if (pawn_promotion == "B")white_promotion = "b";
            GB[destChkr.getI()][destChkr.getJ()].setPlayer(Player(white_promotion, true));
        } else {
            //make black promotion
            GB[destChkr.getI()][destChkr.getJ()].setPlayer(Player(pawn_promotion, false));
        }
    }
    setLastChkr(destChkr);
    return true;
}

bool GameBoard::isNotSelfChessMove(Checker &sourceChkr, Checker &destChkr, Player &player,
                                   const string &pawn_promotion) {
    //this func illustrate the asking move before its done and check if it is not caused the player his own chess/not protect the king from chess that it already has
    Player src_sourceChkr = sourceChkr.getPlayer();
    Player src_destChkr = destChkr.getPlayer();
    Checker badChk = Checker();
    //try to make the move:
    GB[destChkr.getI()][destChkr.getJ()].setPlayer(player);
    GB[sourceChkr.getI()][sourceChkr.getJ()].setPlayer(Player());
    if (src_sourceChkr.isWhite()) {        //white turn
        if (InChess(getKingChkr(true), badChk, true)) {     //check if after move it is chess
            //reverse the move
            GB[destChkr.getI()][destChkr.getJ()].setPlayer(src_destChkr);
            GB[sourceChkr.getI()][sourceChkr.getJ()].setPlayer(src_sourceChkr);
            return false;
        }
    } else {      //black turn
        if (InChess(getKingChkr(false), badChk, false)) {     //check if after move it is chess
            //reverse the move
            GB[destChkr.getI()][destChkr.getJ()].setPlayer(src_destChkr);
            GB[sourceChkr.getI()][sourceChkr.getJ()].setPlayer(src_sourceChkr);
            return false;
        }
    }
    //reverse the move
    GB[destChkr.getI()][destChkr.getJ()].setPlayer(src_destChkr);
    GB[sourceChkr.getI()][sourceChkr.getJ()].setPlayer(src_sourceChkr);
    return true;
}

bool correct_promotion(const string &player) {     //check for legal players to promotion
    if (player.length() != 1) {
        return false;
    }
    if (player != "Q" && player != "R" && player != "B" &&
        player != "N") { //set promotion with capital letters
        return false;
    }

    return true;
}

bool GameBoard::isLegalPawnMove(Checker &sourceChkr, Checker &destChkr, bool isWhiteTurn,
                                const string &promotion) {
    //string stream use for separate chars
    stringstream ss(sourceChkr.getChkrName());
    char source_letter = ss.get();
    int source_num = ss.get();
    stringstream s(destChkr.getChkrName());
    char dest_letter = s.get();
    int dest_num = s.get();
    if (isWhiteTurn) {
        if ((!promotion.empty() && dest_num != '8') || (promotion.empty() && dest_num == '8')) {
            //promotion legal only for last step of pawn on the board. and it is the last step then must be a promotion
            return false;
        }
        if (source_num + 1 != dest_num &&
            source_num + 2 != dest_num) {  //can make only one or two steps maximum
            return false;
        } else if (source_num + 2 == dest_num) {
            if (source_num !=
                '2') {          //cane make 2 steps only if its the first step of this pawn in game
                return false;
            }
            if (source_letter != dest_letter) {       //and only in straight way
                return false;
            }
            if (!StraightWayIsEmpty(sourceChkr,
                                    destChkr)) {      //and only if there is no other player forward
                return false;
            }
        }
        if (destChkr.isEmpty() && (source_letter + 1 == dest_letter || source_letter - 1 == dest_letter) &&
            source_num + 1 == '6') {
            //case of En Passant move
            string next_chkr_name;
            int next_chkr_num = source_num;
            char next_chkr_letter = dest_letter;
            next_chkr_name = next_chkr_letter;
            next_chkr_name += next_chkr_num;
            if (last_chkr.getChkrName() == next_chkr_name &&
                getCheckerByName(next_chkr_name).getPlayerName() == "P") {
                getCheckerByName(next_chkr_name).setPlayer(Player());
                return true;
            }
            return false;
        }

        if (!destChkr.isEmpty()) {     //if there is another player in dest checker
            if (source_letter + 1 == dest_letter ||
                source_letter - 1 == dest_letter) {     //then it is a diagonal of 1 step
                if (source_num + 1 == dest_num) {
                    return true;
                }
            }
            return false;                                                               //else: it is an illegal step
        }
        if (source_letter != dest_letter) {       //any other step has to be straight
            return false;
        }

    } else {      //its Black's turn
        if ((!promotion.empty() && dest_num != '1') || (promotion.empty() && dest_num == '1')) {
            //promotion legal only for last step of pawn on the board. and it is the last step then must be a promotion
            return false;
        }
        if (source_num - 1 != dest_num &&
            source_num - 2 != dest_num) {  //can make only one or two steps maximum
            return false;
        } else if (source_num - 2 == dest_num) {
            if (source_num !=
                '7') {          //cane make 2 steps only if its the first step of this pawn in game
                return false;
            }
            if (source_letter != dest_letter) {       //and only in straight way
                return false;
            }
            if (!StraightWayIsEmpty(sourceChkr,
                                    destChkr)) {      //and only if there is no other player forward
                return false;
            }
        }
        if (destChkr.isEmpty() && (source_letter + 1 == dest_letter || source_letter - 1 == dest_letter) &&
            source_num - 1 == '3') {
            //case of En Passant move
            string next_chkr_name;
            int next_chkr_num = source_num;
            char next_chkr_letter = dest_letter;
            next_chkr_name = next_chkr_letter;
            next_chkr_name += next_chkr_num;
            if (last_chkr.getChkrName() == next_chkr_name &&
                getCheckerByName(next_chkr_name).getPlayerName() == "p") {
                getCheckerByName(next_chkr_name).setPlayer(Player());
                return true;
            }
            return false;
        }

        if (!destChkr.isEmpty()) {     //if there is another player in dest checker
            if (source_letter + 1 == dest_letter ||
                source_letter - 1 == dest_letter) {     //then it must be a diagonal of 1 step
                if (source_num - 1 == dest_num) {
                    return true;
                }
            }
            return false;                                                               //else: it is an illegal step
        }
        if (source_letter != dest_letter) {       //any other step has to be straight
            return false;
        }
    }
    return true;
}

bool GameBoard::isLegalQueenMove(Checker &sourceChkr, Checker &destChkr) {
    //string stream use for separate chars
    stringstream ss(sourceChkr.getChkrName());
    char source_letter = ss.get();
    int source_num = ss.get();
    stringstream s(destChkr.getChkrName());
    char dest_letter = s.get();
    int dest_num = s.get();
    if (source_letter == dest_letter || source_num == dest_num) {   //straight move
        if (!StraightWayIsEmpty(sourceChkr, destChkr)) {         //check for empty way
            return false;
        }
    } else if (source_letter != dest_letter && source_num != dest_num) {    //diagonal move
        if (!DiagonalWayIsEmpty(sourceChkr, destChkr)) {             //check for empty way
            return false;
        }
    }
    return true;
}

bool GameBoard::isLegalKingMove(Checker &sourceChkr, Checker &destChkr, bool isWhite) {
    int srcI = sourceChkr.getI();
    int srcJ = sourceChkr.getJ();
    int dstI = destChkr.getI();
    int dstJ = destChkr.getJ();
    //Castling case:
    if ((abs(srcJ - dstJ) == 2)) {        //absolute value = distance from src to dest
        if (srcI != dstI) {
            return false;
        }
        if (!StraightWayIsEmpty(sourceChkr, destChkr)) {
            return false;
        }
        Checker kingChkr, rookChkr, next1, next2;
        if (isWhite) {
            if (whiteKingInChess || whiteMadeCastling) {
                //**cannot make castling while king is threaten
                //**can make castling once in the hole game
                return false;
            }
            kingChkr = getKingChkr(true);
            if (kingChkr.getChkrName() != "E1") {
                return false;
            }
            if (srcJ + 2 == dstJ) {        //right size castling
                rookChkr = Checker(getCheckerByName("H1"));
            } else {             //left side castling
                rookChkr = Checker(getCheckerByName("A1"));
            }
            if (rookChkr.getPlayerName() != "r") {   //check for rook player
                return false;
            }
            if ((kingChkr.getPlayer().get_num_of_moves() != 0) ||
                (rookChkr.getPlayer().get_num_of_moves()) != 0) {
                //legal only if it is rook's and king's first move in game
                return false;
            }
            next1 = getNextChckr(kingChkr, rookChkr);
            next2 = getNextChckr(next1, rookChkr);
            Player king = kingChkr.getPlayer();
            if (!(isNotSelfChessMove(kingChkr, next1, king) && isNotSelfChessMove(kingChkr, next2, king))) {
                //check for next1 and next2 checkers that are not threaten by any other player
                return false;
            }

            whiteMadeCastling = true;

        } else {       //black turn
            if (blackKingInChess || blackMadeCastling) {
                //**cannot make castling while king is threaten
                //**can make castling once in the hole game
                return false;
            }
            kingChkr = getKingChkr(false);
            if (kingChkr.getChkrName() != "E8") {
                return false;
            }
            if (srcJ + 2 == dstJ) {        //right size castling
                rookChkr = Checker(getCheckerByName("H8"));
            } else {             //left side castling
                rookChkr = Checker(getCheckerByName("A8"));
            }

            if (rookChkr.getPlayerName() != "R") {   //check for rook and king players
                return false;
            }
            if (kingChkr.getPlayer().get_num_of_moves() != 0 ||
                rookChkr.getPlayer().get_num_of_moves() != 0) {
                return false;
            }
            next1 = getNextChckr(kingChkr, rookChkr);
            next2 = getNextChckr(next1, rookChkr);
            Player king = kingChkr.getPlayer();
            if (!(isNotSelfChessMove(kingChkr, next1, king) && isNotSelfChessMove(kingChkr, next2, king))) {
                //check for next1 and next2 checkers that are not threaten by any other player
                return false;
            }

            blackMadeCastling = true;
        }

        //make rook move:
        GB[next1.getI()][next1.getJ()].setPlayer(rookChkr.getPlayer());
        GB[rookChkr.getI()][rookChkr.getJ()].setPlayer(Player());
        return true;

    } else if (srcI == dstI) {       //1 step right or left
        if (srcJ + 1 !=
            dstJ && srcJ
                    - 1 != dstJ) {
            return false;
        }
    } else if (srcJ == dstJ) {    //up or down step
        if (srcI + 1 !=
            dstI && srcI
                    - 1 != dstI) {
            return false;
        }
    } else if (!((srcI + 1 == dstI && srcJ + 1 == dstJ) || (srcI + 1 == dstI && srcJ - 1 == dstJ) ||
                 (srcI - 1 == dstI && srcJ + 1 == dstJ) || (srcI - 1 == dstI && srcJ - 1 == dstJ))) {
        return false;
    }
    return true;
}


bool GameBoard::isLegalKnightMove(Checker &sourceChkr, Checker &destChkr) {
    int srcI = sourceChkr.getI();
    int srcJ = sourceChkr.getJ();
    int dstI = destChkr.getI();
    int dstJ = destChkr.getJ();
    if (!(firstKnightChoice(dstI, dstJ, srcI, srcJ) ||
          secondKnightChoice(dstI, dstJ, srcI, srcJ) ||
          thirdKnightChoice(dstI, dstJ, srcI, srcJ) ||
          fourthKnightChoice(dstI, dstJ, srcI, srcJ) ||
          fifthKnightChoice(dstI, dstJ, srcI, srcJ) ||
          sixthKnightChoice(dstI, dstJ, srcI, srcJ) ||
          seventhKnightChoice(dstI, dstJ, srcI, srcJ) ||
          eighthKnightChoice(dstI, dstJ, srcI, srcJ))) {
        return false;
    }
    return true;
}

bool GameBoard::isLegalRookMove(Checker &sourceChkr, Checker &destChkr) {
    if (sourceChkr.getI() != destChkr.getI() && sourceChkr.getJ() != destChkr.getJ()) {
        return false;
    }
    if (!StraightWayIsEmpty(sourceChkr, destChkr)) {
        return false;
    }
    return true;
}

bool GameBoard::isLegalBishopMove(Checker &sourceChkr, Checker &destChkr) {
    stringstream ss(sourceChkr.getChkrName());
    char source_letter = ss.get();
    int source_num = ss.get();
    stringstream s(destChkr.getChkrName());
    char dest_letter = s.get();
    int dest_num = s.get();
    if (source_letter == dest_letter || source_num == dest_num) {  //then it is not diagonal way
        return false;
    }
    if (!DiagonalWayIsEmpty(sourceChkr, destChkr)) {
        return false;
    }
    return true;

}

Checker &GameBoard::getNextChckr(Checker &source, Checker &dest) {
    int srcI = source.getI();
    int srcJ = source.getJ();
    int dstI = dest.getI();
    int dstJ = dest.getJ();

    if (srcI == dstI) {
        //right or lefy way
        if (srcJ > dstJ) {
            return GB[dstI][srcJ - 1];
        } else {
            return GB[dstI][srcJ + 1];
        }
    } else if (srcJ == dstJ) {
        //up or down way
        if (srcI > dstI) {
            return GB[srcI - 1][dstJ];
        } else {
            return GB[srcI + 1][dstJ];
        }
    } else {       //diagonal ways
        if (srcI < dstI && srcJ > dstJ) {
            return GB[srcI + 1][srcJ - 1];
        } else if (srcI > dstI && srcJ < dstJ) {
            return GB[srcI - 1][srcJ + 1];
        } else if (srcI > dstI && srcJ > dstJ) {
            return GB[srcI - 1][srcJ - 1];
        } else {       // srcI<dstI && srcJ<dstJ)
            return GB[srcI + 1][srcJ + 1];
        }
    }

}

bool GameBoard::StraightWayIsEmpty(Checker &sourceChkr, Checker &destChkr) {
    //if source i != dest i then it is straight/back move
    if (sourceChkr.getI() != destChkr.getI() && sourceChkr.getJ() == destChkr.getJ()) {
        int start_i, end_i, this_j;
        if (sourceChkr.getI() > destChkr.getI()) {
            start_i = destChkr.getI();
            end_i = sourceChkr.getI();
        } else {
            start_i = sourceChkr.getI();
            end_i = destChkr.getI();
        }
        this_j = sourceChkr.getJ();
        for (int i = start_i + 1; i < end_i; i++) {
            if (!GB[i][this_j].isEmpty()) {
                return false;
            }
        }

    } else {              //source i == dest i --> it is right or left move
        int start_j, end_j, this_i;
        if (sourceChkr.getJ() > destChkr.getJ()) {
            start_j = destChkr.getJ();
            end_j = sourceChkr.getJ();
        } else {
            start_j = sourceChkr.getJ();
            end_j = destChkr.getJ();
        }
        this_i = sourceChkr.getI();
        for (int j = start_j + 1; j < end_j; j++) {
            if (!GB[this_i][j].isEmpty()) {
                return false;
            }
        }
    }
    return true;

}

bool GameBoard::DiagonalWayIsEmpty(Checker &sourceChkr, Checker &destChkr) {
    int start_i, end_i, start_j, end_j;
    if (sourceChkr.getI() > destChkr.getI()) {   //upper diagonal
        start_i = destChkr.getI();
        end_i = sourceChkr.getI();
        start_j = destChkr.getJ();
        end_j = sourceChkr.getJ();
        if (sourceChkr.getJ() > destChkr.getJ()) {
            for (int i = start_i + 1; i < end_i; i++) {
                if (!GB[i][++start_j].isEmpty()) {
                    return false;
                }
            }
            if (start_j + 1 != end_j) {       //then its not diagonal way
                return false;
            }
        } else {
            for (int i = start_i + 1; i < end_i; i++) {
                if (!GB[i][--start_j].isEmpty()) {
                    return false;
                }
            }
            if (start_j - 1 != end_j) {       //then its not diagonal way
                return false;
            }
        }
    } else {          //down diagonal
        start_i = sourceChkr.getI();
        end_i = destChkr.getI();
        start_j = sourceChkr.getJ();
        end_j = destChkr.getJ();
        if (sourceChkr.getJ() > destChkr.getJ()) {
            for (int i = start_i + 1; i < end_i; i++) { //5
                if (!GB[i][--start_j].isEmpty()) {
                    return false;
                }
            }
            if (start_j - 1 != end_j) {       //then its not diagonal way
                return false;
            }
        } else {
            for (int i = start_i + 1; i < end_i; i++) {
                if (!GB[i][++start_j].isEmpty()) {
                    return false;
                }
            }
            if (start_j + 1 != end_j) {       //then its not diagonal way
                return false;
            }
        }
    }

    return true;
}

bool GameBoard::InChess(Checker &kingChkr, Checker &opponentPlayerChkr, bool whiteKing) {
    //THIS FUNC CHECK IF SPECIFIC KING IS OPPONENT AND INITIALIZED OPPONENT CHECKER
    int kingI = kingChkr.getI();
    int kingJ = kingChkr.getJ();
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {       //left way from king
            if (j == kingJ && i == kingI) {
                continue;
            }

            if (!GB[i][j].isEmpty()) {
                if (GB[i][j].getPlayer().isWhite() != whiteKing) {
                    //if the current checker is not empty and the player belong to opponent team
                    //-->then check if it threaten on current king
                    Player p = GB[i][j].getPlayer();
                    if (isLegalPlayerMove(GB[i][j], kingChkr, p)) {
                        opponentPlayerChkr = GB[i][j];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


bool GameBoard::theKingCanSaveHimself(Checker &badChkr, bool whiteKing) {
    //this func look for legal move for the king to save himself from chess
    int kingI = getKingChkr(whiteKing).getI();
    int kingJ = getKingChkr(whiteKing).getJ();
    for (int i = kingI - 1; i <= kingI + 1; i++) {
        if (i < 0 || i > 7) {
            continue;
        }
        for (int j = kingJ - 1; j <= kingJ + 1; j++) {
            if (j < 0 || j > 7) {
                continue;
            }
            if (i == kingI && j == kingJ) {
                continue;
            }
            Player p = getKingChkr(whiteKing).getPlayer();
            if (isLegalPlayerMove(getKingChkr(whiteKing), GB[i][j], p)) {
                return true;

            }
        }
    }
    return false;
}

bool GameBoard::otherPlayerCanSaveKing(bool whiteKing, Checker &badChkr) {
    for (int i = 0; i < 8; i++) {
        //this loop check if there is some other player who can save his king by making legal step
        // and eat the threaten player
        for (int j = 0; j < 8; j++) {
            if (i == badChkr.getI() && j == badChkr.getJ()) {
                continue;
            }
            Player curr_player = Player(GB[i][j].getPlayer());
            if (isLegalPlayerMove(GB[i][j], badChkr, curr_player)) {
                return true;
            }
        }
    }
    if (badChkr.getPlayer().isKnight()) {
        //if the threaten player is a knight && the king cannot save himself by moving && no other player can eat him ->
        //then it is a chessmate! because the last choice of block the threaten player's way is not available
        return false;
    }
    Checker saveChkr = Checker(getKingChkr(whiteKing));
    while (saveChkr != badChkr) {
        saveChkr = Checker(getNextChckr(saveChkr, badChkr));
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (!GB[i][j].isEmpty()) {
                    Player p = Player(GB[i][j].getPlayer());
                        if (!p.isKing()){
                            if (p.isWhite() == whiteKing) {
                                if (isLegalPlayerMove(GB[i][j], saveChkr, p)) {
                                    return true;
                                }
                            }
                    }
                }
            }
        }
    }
    return false;
}

bool GameBoard::isCheckMate() {
    Checker whiteBadChk;
    Checker blackBadChk;
    whiteKingInChess = InChess(getKingChkr(true), whiteBadChk, true);
    blackKingInChess = InChess(getKingChkr(false), blackBadChk, false);

    if (!whiteKingInChess && !blackKingInChess) {
        return false;       //if there is not chess at all then in particular its not checkmate!
    }
    if (whiteKingInChess) {
        if (theKingCanSaveHimself(whiteBadChk, true)) {
            return false;
        }
        if (otherPlayerCanSaveKing(true, whiteBadChk)) {
            return false;
        }
    }
    if (blackKingInChess) {
        if (theKingCanSaveHimself(blackBadChk, false)) {
            return false;
        }
        if (otherPlayerCanSaveKing(false, blackBadChk)) {
            return false;
        }

    }
    return true;
}

bool GameBoard::isStalemate() {
    bool whiteHasMove = false;
    bool blackHasMove = false;
    //check for each player for each side that is not in chess if it has a legal move to play
    if (whiteKingInChess || blackKingInChess) {
        return false;
    }
//check for white's possible move
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!GB[i][j].isEmpty() && GB[i][j].getPlayer().isWhite()) {
                Player p = GB[i][j].getPlayer();
                if(p.isKing()){
                    continue;
                }
                for (int k = 0; k < 8; k++) {
                    for (int g = 0; g < 8; g++) {
                        if (k == i && g == j) {
                            continue;
                        }
                        if (isLegalPlayerMove(GB[i][j], GB[k][g], p)) {
                            whiteHasMove = true;
                            break;
                        }
                    }
                }
            }
        }
    }
//check for black's possible move
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!GB[i][j].isEmpty() && !GB[i][j].getPlayer().isWhite()) {
                Player p = GB[i][j].getPlayer();
                if(p.isKing()){
                    continue;
                }
                for (int k = 0; k < 8; k++) {
                    for (int g = 0; g < 8; g++) {
                        if (k == i && j == g) {
                            continue;
                        }
                        if (isLegalPlayerMove(GB[i][j], GB[k][g], p)) {
                            blackHasMove = true;
                            break;
                        }
                    }
                }
            }
        }
    }
    if (blackHasMove && whiteHasMove) {
        return false;
    }
    return true;
}

bool GameBoard::isMaterialInsufficient() {
    if (whiteHasOnlyKnightOrBishop() && blackHasOnlyKnightOrBishop()) {
        return true;
    }
    return false;
}

bool GameBoard::whiteHasOnlyKnightOrBishop() {
    bool alreadyFoundWhitePlayer = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (!GB[i][j].isEmpty()) {
                Player p = GB[i][j].getPlayer();
                if (p.isWhite() && !p.isKing()) {
                    if (alreadyFoundWhitePlayer) {
                        //there is more then only two players
                        return false;
                    } else {
                        if (!(p.isBishop() || p.isKnight())) {
                            //player is not bishop or knight
                            return false;
                        } else {
                            alreadyFoundWhitePlayer = true;
                        }
                    }
                }
            }
        }
    }
    return true;}

    bool GameBoard::blackHasOnlyKnightOrBishop() {
        bool alreadyFoundBlackPlayer = false;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (!GB[i][j].isEmpty()) {
                    Player p = GB[i][j].getPlayer();
                    if (!p.isWhite() && !p.isKing()) {
                        if (alreadyFoundBlackPlayer) {
                            //there is more then only two players
                            return false;
                        } else {
                            if (!(p.isBishop() || p.isKnight())) {
                                //player is not bishop or knight
                                return false;
                            } else {
                                alreadyFoundBlackPlayer = true;
                            }
                        }
                    }
                }
            }
        }
            return true;
        }

        void GameBoard::printGB() {
            string boardResult = "";
            char num = '8';

            for (int i = 0; i < 8; i++) {
                boardResult += num;
                boardResult += " ";
                for (int j = 0; j < 8; j++) {
                    boardResult += GB[i][j].getPlayerName();
                    if (j != 7) {
                        boardResult += " ";
                    }
                }
                boardResult += "\n";
                num--;

            }
            boardResult += "  A B C D E F G H";
            cout << boardResult << endl;
        }

        GameBoard::~GameBoard()
        {

        }



