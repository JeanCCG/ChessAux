// #ifndef CHESS_UCSP_IA_H
// #define CHESS_UCSP_IA_H
// #endif

#pragma once

#include "../gameboard/gameboard.h"

using namespace std;
int contador=0;
bool availablepiece;
bool availableeat;
int moves[26][3];
int movesm;
int maxpt,minpt;
int maxmove[2]={0,0};
int eval;
char temp;
int k, l;
void arrr(){
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 2; ++j) {
            moves[i][j]=0;
        }
    }
}


void copy(char m_ori[26][26], char m_copy[26][26]){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            m_copy[i][j]=m_ori[i][j];
        }
    }
}
//void move(int x, int y)
/*
int intelligence(
	Piece slots[26][26], int width, int height,
	int playerBearings[][2], int playerLength,
	int playerPickableBearings[][2], int playerPickableBearings,
	int enemyBearings[][2], int enemyLength,
	int enemyPickableBearings[][2], int enemyPickableBearings,
	int piece[2], int accumulated, int difficulty, int deep, bool A)
{}*/
int max(int a,int b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}
int min(int a,int b){
    if(a<b){
        return a;
    }else{
        return b;
    }
}
int p2validpieces(char piece){
    switch (piece)
    {
        case PiecesChar::charP1_king:
            return 1;
        case PiecesChar::charP1_queen:
            return 2;
        case PiecesChar::charP1_rook:
            return 3;
        case PiecesChar::charP1_knight:
            return 4;
        case PiecesChar::charP1_bishop:
            return 5;
        case PiecesChar::charP1_pawn:
            return 6;
        default:
            return 0;
    }
}
int p1validpieces(char piece){
    switch (piece)
    {
        case PiecesChar::charP2_king:
            return 1;
        case PiecesChar::charP2_queen:
            return 2;
        case PiecesChar::charP2_rook:
            return 3;
        case PiecesChar::charP2_knight:
            return 4;
        case PiecesChar::charP2_bishop:
            return 5;
        case PiecesChar::charP2_pawn:
            return 6;
        default:
            return 0;
    }
}
int minimax(char slots[26][26], int depth, bool player, int points) { //Player = True
    if (depth == 0) {
        cout<<"\nDepth =0 return points\n";
        return points;
    }
    if (player) {
        maxpt = -50;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                switch (slots[i][j]){
                case PiecesChar::charP2_king: {
                    arrr();
                    movesm = 0;
                    availablepiece=true;
                    int p_y = i - 1;
                    int cas;
                    if (i == 6) {
                        cas = 2;
                    } else {
                        cas = 1;
                    }
                    for (k = 0; k < cas; k++) {
                        if ((slots[p_y][j] == PiecesChar::char_free) || (p2validpieces(slots[p_y][j]))) {
                            moves[movesm][0] = p_y;
                            moves[movesm][1] = j;
                            movesm++;
                        } else {
                            break;
                        }
                    }
                    break;
                }
                case PiecesChar::charP2_queen:
                    availablepiece=true;
                    arrr();
                    movesm = 0;
                    for (k = j + 1; k < 8; k++) {
                        if ((slots[i][k] == PiecesChar::char_free) || (p2validpieces(slots[i][k]))) {
                            moves[movesm][0] = i;
                            moves[movesm][1] = k;
                            movesm++;
                        } else
                            break;
                    }
                    //(+y) line
                    for (k = i - 1; -1 < k; k--) {
                        if ((slots[k][j] == PiecesChar::char_free) || (p2validpieces(slots[k][j]))) {
                            moves[movesm][0] = k;
                            moves[movesm][1] = j;
                            movesm++;
                        } else
                            break;
                    }
                    //(-x) line
                    for (k = j - 1; -1 < k; k--) {
                        if ((slots[i][k] == PiecesChar::char_free) || (p2validpieces(slots[i][k]))) {
                            moves[movesm][0] = i;
                            moves[movesm][1] = k;
                            movesm++;
                        } else
                            break;
                    }
                    //(-y) line
                    for (k = i + 1; k < 8; k++) {
                        if ((slots[k][j] == PiecesChar::char_free) || (p2validpieces(slots[k][j]))) {
                            moves[movesm][0] = k;
                            moves[movesm][1] = j;
                            movesm++;
                        } else
                            break;
                    }
                    k = i - 1;
                    l = j + 1;
                    for (; (-1 < k) && (l < 8); (k--) && (l++))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    k = i - 1;
                    l = j - 1;
                    for (; (-1 < k) && (-1 < l); (k--) && (l--))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    //(-x;-y) diagonal
                    k = i + 1;
                    l = j - 1;
                    for (; (k < 8) && (-1 < l); (k++) && (l--))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    //(+x;-y) diagonal
                    k = i + 1;
                    l = j + 1;
                    for (; (k < 8) && (l < 8); (k++) && (l++))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    break;
                case PiecesChar::charP2_rook: {
                    //(+x) line
                    arrr();
                    movesm = 0;
                    availablepiece=true;
                    for (k = j + 1; k < 8; k++) {
                        if ((slots[i][k] == PiecesChar::char_free) || (p2validpieces(slots[i][k]))) {
                            moves[movesm][0] = i;
                            moves[movesm][1] = k;
                            movesm++;
                        } else
                            break;
                    }
                    //(+y) line
                    for (k = i - 1; -1 < k; k--) {
                        if ((slots[k][j] == PiecesChar::char_free) || (p2validpieces(slots[k][j]))) {
                            moves[movesm][0] = k;
                            moves[movesm][1] = j;
                            movesm++;
                        } else
                            break;
                    }
                    //(-x) line
                    for (k = j - 1; -1 < k; k--) {
                        if ((slots[i][k] == PiecesChar::char_free) || (p2validpieces(slots[i][k]))) {
                            moves[movesm][0] = i;
                            moves[movesm][1] = k;
                            movesm++;
                        } else
                            break;
                    }
                    //(-y) line
                    for (k = i + 1; k < 8; k++) {
                        if ((slots[k][j] == PiecesChar::char_free) || (p2validpieces(slots[k][j]))) {
                            moves[movesm][0] = k;
                            moves[movesm][1] = j;
                            movesm++;
                        } else
                            break;
                    }

                    break;
                }
                case PiecesChar::charP2_knight: {
                    arrr();
                    movesm = 0;
                    availablepiece=true;
                    if ((i > 1) && (j > 0) && ((slots[i - 2][j - 1] == PiecesChar::char_free) || (p2validpieces(slots[i - 2][j - 1])))) {
                        moves[movesm][0] = i - 2;
                        moves[movesm][1] = j - 1;
                        movesm++;
                    }
                    if ((i > 1) && (j < 7) && ((slots[i - 2][j + 1] == PiecesChar::char_free) || (p2validpieces(slots[i - 2][j + 1])))) {
                        moves[movesm][0] = i - 2;
                        moves[movesm][1] = j + 1;
                        movesm++;
                    }
                    if ((i > 0) && (j > 1) && ((slots[i - 1][j - 2] == PiecesChar::char_free) || (p2validpieces(slots[i - 1][j - 2])))) {
                        moves[movesm][0] = i - 1;
                        moves[movesm][1] = j - 2;
                        movesm++;
                    }
                    if ((i < 7) && (j > 1) && ((slots[i + 1][j - 2] == PiecesChar::char_free) || (p2validpieces(slots[i + 1][j - 2])))) {
                        moves[movesm][0] = i + 1;
                        moves[movesm][1] = j - 2;
                        movesm++;
                    }
                    if ((i < 6) && (j > 0) && ((slots[i + 2][j - 1] == PiecesChar::char_free) || (p2validpieces(slots[i + 2][j - 1])))) {
                        moves[movesm][0] = i + 2;
                        moves[movesm][1] = j - 1;
                        movesm++;
                    }
                    if ((i < 6) && (j < 7) && ((slots[i + 2][j + 1] == PiecesChar::char_free) || (p2validpieces(slots[i + 2][j + 1])))) {
                        moves[movesm][0] = i + 2;
                        moves[movesm][1] = j + 1;
                        movesm++;
                    }
                    if ((i > 0) && (j < 6) && ((slots[i - 1][j + 2] == PiecesChar::char_free) || (p2validpieces(slots[i - 1][j + 2])))) {
                        moves[movesm][0] = i - 1;
                        moves[movesm][1] = j + 2;
                        movesm++;
                    }
                    if ((i < 7) && (j < 6) && ((slots[i + 1][j + 2] == PiecesChar::char_free) || (p2validpieces(slots[i + 1][j + 2])))) {
                        moves[movesm][0] = i + 1;
                        moves[movesm][1] = j + 2;
                        movesm++;
                    }
                    break;
                }
                case PiecesChar::charP2_bishop:
                    arrr();
                    movesm = 0;
                    availablepiece=true;
                    k = i - 1;
                    l = j + 1;
                    for (; (-1 < k) && (l < 8); (k--) && (l++))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    k = i - 1;
                    l = j - 1;
                    for (; (-1 < k) && (-1 < l); (k--) && (l--))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    //(-x;-y) diagonal
                    k = i + 1;
                    l = j - 1;
                    for (; (k < 8) && (-1 < l); (k++) && (l--))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    //(+x;-y) diagonal
                    k = i + 1;
                    l = j + 1;
                    for (; (k < 8) && (l < 8); (k++) && (l++))
                    {
                        if ((slots[k][l] == PiecesChar::char_free) || (p2validpieces(slots[k][l])))
                        {
                            moves[movesm][0] = k;
                            moves[movesm][1] = l;
                            movesm++;
                        }
                        else
                            break;
                    }
                    break;
                case PiecesChar::charP2_pawn:{
                    arrr();
                    movesm = 0;
                    availablepiece=true;
                    int cas;
                    if (i == 1) {
                        cas = 2;
                    } else {
                        cas = 1;
                    }
                    for (k = 1; k <= cas; k++) {
                        if (cas==1) {
                            if (p2validpieces(slots[i + 1][j + 1])) {
                                moves[movesm][0] = i + 1;
                                moves[movesm][1] = j + 1;
                                movesm++;
                            }
                            if (p2validpieces(slots[i + 1][j - 1])) {
                                moves[movesm][0] = i + 1;
                                moves[movesm][1] = j - 1;
                                movesm++;
                            }
                        }
                        if (slots[i+k][j] == PiecesChar::char_free) {
                            moves[movesm][0] = i+k;
                            moves[movesm][1] = j;
                            movesm++;
                        } else {
                            break;
                        }
                    }
                    break;
                }
                default:
                    availablepiece=false;
                    break;
                }
                if (availablepiece) {
                    availableeat=false;
                    for (k = 0; k < movesm; k++) { // move
                        if (p2validpieces(slots[moves[k][0]][moves[k][1]])) {//points
                            availableeat=true;
                            switch (p2validpieces(slots[moves[k][0]][moves[k][1]])) {
                                case 0://out
                                    moves[k][2] = 0;
                                    break;
                                case 1://king
                                    moves[k][2] = 10;
                                    break;
                                case 2://queen
                                    moves[k][2] = 9;
                                    break;
                                case 3://rook
                                    moves[k][2] = 5;
                                    break;
                                case 4://knight
                                    moves[k][2] = 3;
                                    break;
                                case 5://bishop
                                    moves[k][2] = 3;
                                    break;
                                case 6://pawn
                                    moves[k][2] = 1;
                                    break;
                            }
                        }
                        char slotscopy[26][26];
                        points=moves[k][2];
                        contador++;
                        cout<<contador<<": Profundidad: "<<depth<<" Retornando points:" <<points<<" Player = "<<player<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;
                        for (int m = 0; m < movesm; ++m) {
                            for (int n = 0; n < 2; ++n) {
                                cout<<moves[m][n]<<" ";
                            }
                            cout<<endl;
                        }
                        for (int i = 0; i < 8; ++i) {
                            for (int j = 0; j < 8; ++j) {
                                slotscopy[i][j]=slots[i][j];
                            }
                        }
                        temp = slotscopy[i][j];
                        slotscopy[i][j] = slotscopy[moves[k][0]][moves[k][1]];
                        slotscopy[moves[k][0]][moves[k][1]] = temp;
                        if (availableeat){
                            slotscopy[i][j]=PiecesChar::char_free;
                        }
                        for (int i = 0; i < 8; ++i) {
                            for (int j = 0; j < 8; ++j) {
                                cout<<slotscopy[i][j]<<" ";
                            }
                            cout<<endl;
                        }
                        //eval = minimax(slotscopy, depth - 1, false,points);
                        //maxpt = max(eval, maxpt);
                    }
                    cout<<"End moves"<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;
                }
            }
        }
        cout<<"End Board"<<endl;
        //return  maxpt;
    }else{// MIN
        minpt = 50;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                switch (slots[i][j]) {
                    case PiecesChar::charP1_king: {
                        arrr();
                        movesm = 0;
                        availablepiece = true;
                        int p_y = i - 1;
                        int cas ;
                        if (i == 6) {
                            cas = 2;
                        } else {
                            cas = 1;
                        }
                        for (k = 0; k < cas; k++) {
                            if ((slots[p_y][j] == PiecesChar::char_free) || (p1validpieces(slots[p_y][j]))) {
                                moves[movesm][0] = p_y;
                                moves[movesm][1] = j;
                                movesm++;
                            } else {
                                break;
                            }
                        }
                        break;
                    }
                    case PiecesChar::charP1_queen:
                        arrr();
                        movesm = 0;
                        availablepiece = true;
                        for (k = j + 1; k < 8; k++) {
                            if ((slots[i][k] == PiecesChar::char_free) || (p1validpieces(slots[i][k]))) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else
                                break;
                        }
                        //(+y) line
                        for (k = i - 1; -1 < k; k--) {
                            if ((slots[k][j] == PiecesChar::char_free) || (p1validpieces(slots[k][j]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else
                                break;
                        }
                        //(-x) line
                        for (k = j - 1; -1 < k; k--) {
                            if ((slots[i][k] == PiecesChar::char_free) || (p1validpieces(slots[i][k]))) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else
                                break;
                        }
                        //(-y) line
                        for (k = i + 1; k < 8; k++) {
                            if ((slots[k][j] == PiecesChar::char_free) || (p1validpieces(slots[k][j]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else
                                break;
                        }
                        k = i - 1;
                        l = j + 1;
                        for (; (-1 < k) && (l < 8); (k--) && (l++)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        k = i - 1;
                        l = j - 1;
                        for (; (-1 < k) && (-1 < l); (k--) && (l--)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        //(-x;-y) diagonal
                        k = i + 1;
                        l = j - 1;
                        for (; (k < 8) && (-1 < l); (k++) && (l--)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        //(+x;-y) diagonal
                        k = i + 1;
                        l = j + 1;
                        for (; (k < 8) && (l < 8); (k++) && (l++)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        break;
                    case PiecesChar::charP1_rook: {
                        //(+x) line
                        arrr();
                        movesm = 0;
                        availablepiece = true;
                        for (k = j + 1; k < 8; k++) {
                            if ((slots[i][k] == PiecesChar::char_free) || (p1validpieces(slots[i][k]))) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else
                                break;
                        }
                        //(+y) line
                        for (k = i - 1; -1 < k; k--) {
                            if ((slots[k][j] == PiecesChar::char_free) || (p1validpieces(slots[k][j]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else
                                break;
                        }
                        //(-x) line
                        for (k = j - 1; -1 < k; k--) {
                            if ((slots[i][k] == PiecesChar::char_free) || (p1validpieces(slots[i][k]))) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else
                                break;
                        }
                        //(-y) line
                        for (k = i + 1; k < 8; k++) {
                            if ((slots[k][j] == PiecesChar::char_free) || (p1validpieces(slots[k][j]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else
                                break;
                        }

                        break;
                    }
                    case PiecesChar::charP1_knight: {
                        arrr();
                        movesm = 0;
                        availablepiece = true;
                        if ((i > 1) && (j > 0) &&
                            ((slots[i - 2][j - 1] == PiecesChar::char_free) || (p1validpieces(slots[i - 2][j - 1])))) {
                            moves[movesm][0] = i - 2;
                            moves[movesm][1] = j - 1;
                            movesm++;
                        }
                        if ((i > 1) && (j < 7) &&
                            ((slots[i - 2][j + 1] == PiecesChar::char_free) || (p1validpieces(slots[i - 2][j + 1])))) {
                            moves[movesm][0] = i - 2;
                            moves[movesm][1] = j + 1;
                            movesm++;
                        }
                        if ((i > 0) && (j > 1) &&
                            ((slots[i - 1][j - 2] == PiecesChar::char_free) || (p1validpieces(slots[i - 1][j - 2])))) {
                            moves[movesm][0] = i - 1;
                            moves[movesm][1] = j - 2;
                            movesm++;
                        }
                        if ((i < 7) && (j > 1) &&
                            ((slots[i + 1][j - 2] == PiecesChar::char_free) || (p1validpieces(slots[i + 1][j - 2])))) {
                            moves[movesm][0] = i + 1;
                            moves[movesm][1] = j - 2;
                            movesm++;
                        }
                        if ((i < 6) && (j > 0) &&
                            ((slots[i + 2][j - 1] == PiecesChar::char_free) || (p1validpieces(slots[i + 2][j - 1])))) {
                            moves[movesm][0] = i + 2;
                            moves[movesm][1] = j - 1;
                            movesm++;
                        }
                        if ((i < 6) && (j < 7) &&
                            ((slots[i + 2][j + 1] == PiecesChar::char_free) || (p1validpieces(slots[i + 2][j + 1])))) {
                            moves[movesm][0] = i + 2;
                            moves[movesm][1] = j + 1;
                            movesm++;
                        }
                        if ((i > 0) && (j < 6) &&
                            ((slots[i - 1][j + 2] == PiecesChar::char_free) || (p1validpieces(slots[i - 1][j + 2])))) {
                            moves[movesm][0] = i - 1;
                            moves[movesm][1] = j + 2;
                            movesm++;
                        }
                        if ((i < 7) && (j < 6) &&
                            ((slots[i + 1][j + 2] == PiecesChar::char_free) || (p1validpieces(slots[i + 1][j + 2])))) {
                            moves[movesm][0] = i + 1;
                            moves[movesm][1] = j + 2;
                            movesm++;
                        }
                        break;
                    }
                    case PiecesChar::charP1_bishop:
                        arrr();
                        movesm = 0;
                        availablepiece = true;
                        k = i - 1;
                        l = j + 1;
                        for (; (-1 < k) && (l < 8); (k--) && (l++)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        k = i - 1;
                        l = j - 1;
                        for (; (-1 < k) && (-1 < l); (k--) && (l--)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        //(-x;-y) diagonal
                        k = i + 1;
                        l = j - 1;
                        for (; (k < 8) && (-1 < l); (k++) && (l--)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        //(+x;-y) diagonal
                        k = i + 1;
                        l = j + 1;
                        for (; (k < 8) && (l < 8); (k++) && (l++)) {
                            if ((slots[k][l] == PiecesChar::char_free) || (p1validpieces(slots[k][l]))) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            } else
                                break;
                        }
                        break;
                    case PiecesChar::charP1_pawn: {
                        arrr();
                        movesm = 0;
                        availablepiece = true;
                        int cas;
                        if (i == 6) {
                            cas = 2;
                        } else {
                            cas = 1;
                        }
                        for (k = 1; k <= cas; k++) {
                            if (cas==1){
                                if(p1validpieces(slots[i-1][j+1])){
                                    moves[movesm][0] = i-1;
                                    moves[movesm][1] = j+1;
                                    movesm++;
                                }
                                if(p1validpieces(slots[i-1][j-1])){
                                    moves[movesm][0] = i-1;
                                    moves[movesm][1] = j-1;
                                    movesm++;
                                }
                            }
                            if (slots[i-k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = i-k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else {
                                break;
                            }
                        }
                        break;
                    }
                    default:
                        availablepiece = false;
                        break;
                }
                if (availablepiece) {
                    availableeat=false;
                    for (k = 0; k < movesm; k++) { // move
                        if (p1validpieces(slots[moves[k][0]][moves[k][1]])) {//points
                            availableeat=true;
                            switch (p1validpieces(slots[moves[k][0]][moves[k][1]])) {
                                case 0://out
                                    moves[k][2] = 0;
                                    break;
                                case 1://king
                                    moves[k][2] = -10;
                                    break;
                                case 2://queen
                                    moves[k][2] = -9;
                                    break;
                                case 3://rook
                                    moves[k][2] = -5;
                                    break;
                                case 4://knight
                                    moves[k][2] = -3;
                                    break;
                                case 5://bishop
                                    moves[k][2] = -3;
                                    break;
                                case 6://pawn
                                    moves[k][2] = -1;
                                    break;
                            }
                        }
                        char slotscopy[26][26];
                        points=moves[k][2];
                        cout<<"Profundidad: "<<depth<<" Retornando points:" <<points<<" Player = "<<player<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;
                        for (int m = 0; m < movesm; ++m) {
                            for (int n = 0; n < 2; ++n) {
                                cout<<moves[m][n]<<" ";
                            }
                            cout<<endl;
                        }
                        copy(slots,slotscopy);
                        temp = slotscopy[i][j];
                        slotscopy[i][j] = slotscopy[moves[k][0]][moves[k][1]];
                        slotscopy[moves[k][0]][moves[k][1]] = temp;
                        if(availableeat){
                            slotscopy[i][j]=PiecesChar::char_free;
                        }
                        for (int i = 0; i < 8; ++i) {
                            for (int j = 0; j < 8; ++j) {
                                cout<<slotscopy[i][j]<<" ";
                            }
                            cout<<endl;
                        }
                        minimax(slotscopy, depth - 1, true, points);
                        cout<<"End minimax"<<endl;
                        //minpt = min(eval, minpt);
                    }
                    cout<<"End moves"<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;
                }
            }
        }
        cout<<"End board"<<endl;
        //return  minpt;
    }
}

