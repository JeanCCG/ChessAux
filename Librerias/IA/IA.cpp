#include "IA.h"
#include <iostream>
using namespace std;
int contador=0,contadora=0;
bool availablepiece;
bool availableeat;
bool kingside_castling=false;
bool queenside_castling=false;
bool kingside_castling_P1=false;
bool queenside_castling_P1=false;
bool pawn_in_passing=true;
bool movesvalidantpawn=false;
bool checkmate;
bool check;
char temp;
int k, l;

void copy(char m_ori[8][8], char m_copy[8][8]){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            m_copy[i][j]=m_ori[i][j];
        }
    }
}
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
int minimax(char slots[8][8], int depth, bool player, int points,int arrr[1000][5],int& arrrms) { //Player = True
    if (depth == 0) {
        return points;
    }
    if (player) {
        int maxpt = -50;
        int pts=points;
        int arrrm;
        if(depth==3) {
            arrrm = 0;
        }
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j) {
                int moves[26][3];
                int movesm;
                switch (slots[i][j]){
                    case PiecesChar::charP2_king: {
                        checkmate = true;
                        check = false;
                        availablepiece = true;
                        for (int r = 0; r < 26; ++r)
                        {
                            for (int s = 0; s < 3; ++s)
                            {
                                moves[r][s] = 0;
                            }
                        }
                        movesm = 0;
                        int movesp1[121][2];
                        int movesp1m=0;
                        int ct=0;
                        for (int p = 0; p < 8; ++p)
                        {
                            for (int o = 0; o < 8; ++o)
                            {
                                switch (slots[p][o])
                                {
                                    case PiecesChar::charP1_king:
                                    {
                                        movesp1[movesp1m][0]=p+1;
                                        movesp1[movesp1m][1]=o;
                                        movesp1[movesp1m+1][0]=p+1;
                                        movesp1[movesp1m+1][1]=o+1;
                                        movesp1[movesp1m+2][0]=p;
                                        movesp1[movesp1m+2][1]=o+1;
                                        movesp1[movesp1m+3][0]=p-1;
                                        movesp1[movesp1m+3][1]=o;
                                        movesp1[movesp1m+4][0]=p-1;
                                        movesp1[movesp1m+4][1]=o-1;
                                        movesp1[movesp1m+5][0]=p;
                                        movesp1[movesp1m+5][1]=o-1;
                                        movesp1[movesp1m+6][0]=p+1;
                                        movesp1[movesp1m+6][1]=o-1;
                                        movesp1[movesp1m+7][0]=p-1;
                                        movesp1[movesp1m+7][1]=o+1;
                                        movesp1m=movesp1m+8;
                                        break;
                                    }
                                    case PiecesChar::charP1_queen:
                                        for (k = o + 1; k < 8; k++)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+y) line
                                        for (k = p - 1; - 1 < k; k--)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x) line
                                        for (k = o - 1; - 1 < k; k--)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-y) line
                                        for (k = p + 1; k < 8; k++)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        k = p - 1;
                                        l = o + 1;
                                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        k = p - 1;
                                        l = o - 1;
                                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x;-y) diagonal
                                        k = p + 1;
                                        l = o - 1;
                                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+x;-y) diagonal
                                        k = p + 1;
                                        l = o + 1;
                                        for (; (k < 8) && (l < 8); (k++) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        break;
                                    case PiecesChar::charP1_rook:
                                    {

                                        //(+x) line
                                        for (k = o + 1; k < 8; k++)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+y) line
                                        for (k = p - 1; - 1 < k; k--)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x) line
                                        for (k = o - 1; - 1 < k; k--)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-y) line
                                        for (k = p + 1; k < 8; k++)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    case PiecesChar::charP1_knight:
                                    {
                                        if ((p > 1) && (o > 0) &&
                                            ((slots[p - 2][o - 1] == PiecesChar::char_free) || (p2validpieces(slots[p - 2][o - 1]))))
                                        {
                                            movesp1[movesp1m][0] = p - 2;
                                            movesp1[movesp1m][1] = o - 1;
                                            movesp1m++;
                                        }
                                        if ((p > 1) && (o < 7) &&
                                            ((slots[p - 2][o + 1] == PiecesChar::char_free) || (p2validpieces(slots[p - 2][o + 1]))))
                                        {
                                            movesp1[movesp1m][0] = p - 2;
                                            movesp1[movesp1m][1] = o + 1;
                                            movesp1m++;
                                        }
                                        if ((p > 0) && (o > 1) &&
                                            ((slots[p - 1][o - 2] == PiecesChar::char_free) || (p2validpieces(slots[p - 1][o - 2]))))
                                        {
                                            movesp1[movesp1m][0] = p - 1;
                                            movesp1[movesp1m][1] = o - 2;
                                            movesp1m++;
                                        }
                                        if ((p < 7) && (o > 1) &&
                                            ((slots[p + 1][o - 2] == PiecesChar::char_free) || (p2validpieces(slots[p + 1][o - 2]))))
                                        {
                                            movesp1[movesp1m][0] = p + 1;
                                            movesp1[movesp1m][1] = o - 2;
                                            movesp1m++;
                                        }
                                        if ((p < 6) && (o > 0) &&
                                            ((slots[p + 2][o - 1] == PiecesChar::char_free) || (p2validpieces(slots[p + 2][o - 1]))))
                                        {
                                            movesp1[movesp1m][0] = p + 2;
                                            movesp1[movesp1m][1] = o - 1;
                                            movesp1m++;
                                        }
                                        if ((p < 6) && (o < 7) &&
                                            ((slots[p + 2][o + 1] == PiecesChar::char_free) || (p2validpieces(slots[p + 2][o + 1]))))
                                        {
                                            movesp1[movesp1m][0] = p + 2;
                                            movesp1[movesp1m][1] = o + 1;
                                            movesp1m++;
                                        }
                                        if ((p > 0) && (o < 6) &&
                                            ((slots[p - 1][o + 2] == PiecesChar::char_free) || (p2validpieces(slots[p - 1][o + 2]))))
                                        {
                                            movesp1[movesp1m][0] = p - 1;
                                            movesp1[movesp1m][1] = o + 2;
                                            movesp1m++;
                                        }
                                        if ((p < 7) && (o < 6) &&
                                            ((slots[p + 1][o + 2] == PiecesChar::char_free) || (p2validpieces(slots[p + 1][o + 2]))))
                                        {
                                            movesp1[movesp1m][0] = p + 1;
                                            movesp1[movesp1m][1] = o + 2;
                                            movesp1m++;
                                        }
                                        break;
                                    }
                                    case PiecesChar::charP1_bishop:
                                        k = p - 1;
                                        l = o + 1;
                                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        k = p - 1;
                                        l = o - 1;
                                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x;-y) diagonal
                                        k = p + 1;
                                        l = o - 1;
                                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+x;-y) diagonal
                                        k = p + 1;
                                        l = o + 1;
                                        for (; (k < 8) && (l < 8); (k++) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP2_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p2validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        break;
                                    case PiecesChar::charP1_pawn:
                                    {
                                        int cas;
                                        if (p == 6)
                                        {
                                            cas = 2;
                                        }
                                        else
                                        {
                                            cas = 1;
                                        }
                                        for (k = 1; k <= cas; k++)
                                        {
                                            if (k == 1)
                                            {
                                                if ((p>0)&&(o<7)&&((p2validpieces(slots[p - 1][o + 1])) || (slots[p - 1][o + 1] == PiecesChar::char_free)))
                                                {
                                                    movesp1[movesp1m][0] = p - 1;
                                                    movesp1[movesp1m][1] = o + 1;
                                                    movesp1m++;
                                                }
                                                if ((p>0)&&(o>0)&&((p2validpieces(slots[p - 1][o - 1])) || (slots[p - 1][o - 1] == PiecesChar::char_free)))
                                                {
                                                    movesp1[movesp1m][0] = p - 1;
                                                    movesp1[movesp1m][1] = o - 1;
                                                    movesp1m++;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                        }
                        int movesking_m=0;
                        int movesking[8][2];
                        if (i>0) {
                            if ((j>0)&&((slots[i-1][j-1] == PiecesChar::char_free)||(p2validpieces(slots[i-1][j-1])))){
                                movesking[movesking_m][0]=i-1;
                                movesking[movesking_m][1]=j-1;
                                movesking_m++;
                            }
                            if ((j<7)&&((slots[i-1][j+1] == PiecesChar::char_free)||(p2validpieces(slots[i-1][j+1])))){
                                movesking[movesking_m][0]=i-1;
                                movesking[movesking_m][1]=j+1;
                                movesking_m++;
                            }
                            if((slots[i-1][j] == PiecesChar::char_free)||(p2validpieces(slots[i-1][j]))){
                                movesking[movesking_m][0]=i-1;
                                movesking[movesking_m][1]=j;
                                movesking_m++;
                            }
                        }
                        if (i<7) {
                            if ((j>0)&&((slots[i+1][j-1] == PiecesChar::char_free)||(p2validpieces(slots[i+1][j-1])))){
                                movesking[movesking_m][0]=i+1;
                                movesking[movesking_m][1]=j-1;
                                movesking_m++;
                            }
                            if ((j<7)&&((slots[i+1][j+1] == PiecesChar::char_free)||(p2validpieces(slots[i+1][j+1])))){
                                movesking[movesking_m][0]=i+1;
                                movesking[movesking_m][1]=j+1;
                                movesking_m++;
                            }
                            if((slots[i+1][j] == PiecesChar::char_free)||(p2validpieces(slots[i+1][j]))){
                                movesking[movesking_m][0]=i+1;
                                movesking[movesking_m][1]=j;
                                movesking_m++;
                            }
                        }
                        if ((j<7)&&((slots[i][j+1] == PiecesChar::char_free)||(p2validpieces(slots[i][j+1])))){
                            movesking[movesking_m][0]=i;
                            movesking[movesking_m][1]=j+1;
                            movesking_m++;
                        }
                        if ((j>0)&&((slots[i][j-1] == PiecesChar::char_free)||(p2validpieces(slots[i][j-1])))){
                            movesking[movesking_m][0]=i;
                            movesking[movesking_m][1]=j-1;
                            movesking_m++;
                        }
                        if((i == 0)&&(j==4)){
                            if((slots[0][7]==PiecesChar::charP2_rook)&&(slots[0][5]==PiecesChar::char_free)&&(slots[0][6]==PiecesChar::char_free)){
                                movesking[movesking_m][0] = 0;
                                movesking[movesking_m][1] = 6;
                                movesking_m++;
                                kingside_castling=true;
                            }
                            if((slots[0][0]==PiecesChar::charP2_rook)&&(slots[0][3]==PiecesChar::char_free)&&(slots[0][2]==PiecesChar::char_free)&&(slots[0][1]==PiecesChar::char_free)){
                                movesking[movesking_m][0] = 0;
                                movesking[movesking_m][1] = 2;
                                movesking_m++;
                                queenside_castling=true;
                            }
                        }
                        bool valid_move_king;
                        for (int m = 0; m < movesking_m; ++m) {
                            valid_move_king=true;
                            for (int n = 0; n < movesp1m; ++n) {
                                if((movesking[m][0]==movesp1[n][0])&&(movesking[m][1]==movesp1[n][1])){
                                    valid_move_king=false;
                                    break;
                                }
                                if ((kingside_castling)&&(movesp1[n][0]==0)&&(movesp1[n][1]==5)){
                                    cout<<"if kingside_castling not work"<<endl;
                                    valid_move_king=false;
                                    kingside_castling=false;
                                    break;
                                }
                                if ((queenside_castling)&&(movesp1[n][0]==0)&&(movesp1[n][1]==3)){
                                    valid_move_king=false;
                                    queenside_castling=false;
                                    break;
                                }
                                if((i==movesp1[n][0])&&(j==movesp1[n][1])){
                                    check=true;
                                }
                            }
                            if(valid_move_king){
                                if(slots[movesking[m][0]][movesking[m][1]]==PiecesChar::char_free || (p2validpieces(slots[movesking[m][0]][movesking[m][1]]))) {
                                    moves[movesm][0] = movesking[m][0];
                                    moves[movesm][1] = movesking[m][1];
                                    movesm++;
                                    checkmate=false;
                                }
                            }
                        }
                        if ((check)&&(checkmate)){
                            return -100;
                        }
                        break;
                    }
                    case PiecesChar::charP2_queen:
                        availablepiece=true;
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        for (k = j + 1; k < 8; k++) {
                            if (slots[i][k] == PiecesChar::char_free) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p2validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(+y) line
                        for (k = i - 1; -1 < k; k--) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p2validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        //(-x) line
                        for (k = j - 1; -1 < k; k--) {
                            if (slots[i][k] == PiecesChar::char_free)  {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p2validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(-y) line
                        for (k = i + 1; k < 8; k++) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p2validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        k = i - 1;
                        l = j + 1;
                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if(p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        k = i - 1;
                        l = j - 1;
                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else {
                                break;
                            }
                        }
                        //(-x;-y) diagonal
                        k = i + 1;
                        l = j - 1;
                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        //(+x;-y) diagonal
                        k = i + 1;
                        l = j + 1;
                        for (; (k < 8) && (l < 8); (k++) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        break;
                    case PiecesChar::charP2_rook: {
                        //(+x) line
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece = true;
                        for (k = j + 1; k < 8; k++) {
                            if (slots[i][k] == PiecesChar::char_free) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p2validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(+y) line
                        for (k = i - 1; -1 < k; k--) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p2validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        //(-x) line
                        for (k = j - 1; -1 < k; k--) {
                            if (slots[i][k] == PiecesChar::char_free)  {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p2validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(-y) line
                        for (k = i + 1; k < 8; k++) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p2validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        break;
                    }
                    case PiecesChar::charP2_knight: {
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
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
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece=true;
                        k = i - 1;
                        l = j + 1;
                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if(p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        k = i - 1;
                        l = j - 1;
                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else {
                                break;
                            }
                        }
                        //(-x;-y) diagonal
                        k = i + 1;
                        l = j - 1;
                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        //(+x;-y) diagonal
                        k = i + 1;
                        l = j + 1;
                        for (; (k < 8) && (l < 8); (k++) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p2validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        break;
                    case PiecesChar::charP2_pawn:{
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece=true;
                        int cas;
                        if (i == 1) {
                            cas = 2;
                        } else {
                            cas = 1;
                        }
                        for (k = 1; k <= cas; k++) {
                            if (k==1) {
                                if ((i<7)&&(j<7)&&(p2validpieces(slots[i + 1][j + 1]))) {
                                    moves[movesm][0] = i + 1;
                                    moves[movesm][1] = j + 1;
                                    movesm++;
                                }
                                if ((i<7)&&(j>0)&&(p2validpieces(slots[i + 1][j - 1]))) {
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
                    for (int p = 0; p < movesm; p++) { // move
                        if (p2validpieces(slots[moves[p][0]][moves[p][1]])) {//points
                            availableeat=true;
                            switch (p2validpieces(slots[moves[p][0]][moves[p][1]])) {
                                case 0://out
                                    moves[p][2] = 0;
                                    break;
                                case 1://king
                                    moves[p][2] = 20;
                                    break;
                                case 2://queen
                                    moves[p][2] = 9;
                                    break;
                                case 3://rook
                                    moves[p][2] = 5;
                                    break;
                                case 4://knight
                                    moves[p][2] = 3;
                                    break;
                                case 5://bishop
                                    moves[p][2] = 3;
                                    break;
                                case 6://pawn
                                    moves[p][2] = 1;
                                    break;
                            }
                        }
                        char slotscopy[8][8];
                        points =pts+moves[p][2];
                        /*//DEBUGGING---------------------
                        contadora++;
                        cout<<contadora<<": Profundidad: "<<depth<<" Retornando points:" <<points<<" Player = "<<player<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;
                        contador=0;
                        for (int m = 0; m < movesm; ++m) {
                            for (int n = 0; n < 3; ++n) {
                                cout<<moves[m][n]<<" ";
                            }
                            cout<<endl;
                        }
                        //DEBUGGING---------------------*/

                        for (int i = 0; i < 8; ++i) {
                            for (int j = 0; j < 8; ++j) {
                                slotscopy[i][j]=slots[i][j];
                            }
                        }
                        /*cout<<"kingside_castling: "<<kingside_castling<<"move :"<<moves[p][0]<<moves[p][1]<<endl;*/
                        if((slots[i][j]==PiecesChar::charP2_king)&&(kingside_castling)&&(moves[p][0]==0)&&(moves[p][1]==6)){
                            slotscopy[0][7]=PiecesChar::char_free;
                            slotscopy[0][5]=PiecesChar::charP2_rook;
                        }
                        if((slots[i][j]==PiecesChar::charP2_king)&&(queenside_castling)&&(moves[p][0]==0)&&(moves[p][1]==2)){
                            slotscopy[0][0]=PiecesChar::char_free;
                            slotscopy[0][3]=PiecesChar::charP2_rook;
                        }
                        //Pawn in passing
                        if((pawn_in_passing)&&(movesvalidantpawn)&&(slots[i][j]==PiecesChar::char_free)){
                            slotscopy[i][j]=PiecesChar::char_free;
                            slotscopy[i+1][j-1]=PiecesChar::charP2_pawn;
                        }
                        temp = slotscopy[i][j];
                        slotscopy[i][j] = slotscopy[moves[p][0]][moves[p][1]];
                        slotscopy[moves[p][0]][moves[p][1]] = temp;
                        if (availableeat){
                            slotscopy[i][j]=PiecesChar::char_free;
                        }
                        /*//DEBUGGING---------------------
                        for (int i = 0; i < 8; ++i) {
                            for (int j = 0; j < 8; ++j) {
                                cout<<slotscopy[i][j]<<" ";
                            }
                            cout<<endl;
                        }
                        //DEBUGGING---------------------*/
                        int eval =minimax(slotscopy, depth - 1, false,points,arrr, arrrms);
                        maxpt = max(eval, maxpt);
                        if(depth==Player::Difficulty){
                            arrr[arrrm][0]=moves[p][0];
                            arrr[arrrm][1]=moves[p][1];
                            arrr[arrrm][2]=eval;
                            arrr[arrrm][3]=i;
                            arrr[arrrm][4]=j;
                            arrrm++;
                        }
                    }
                    /*cout<<"End moves"<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;//DEBBUGING*/
                }
            }
        }
        /*cout<<"End Board"<<endl;
        cout<<"\nMaximo valor eval : "<<maxpt<<endl;*/
        if(depth == Player::Difficulty){
            arrrms=arrrm;
        }
        return  maxpt;
    }else{// MIN
        int minpt = 50;
        int pts=points;
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int moves[26][3];
                int movesm;
                switch (slots[i][j]) {
                    case PiecesChar::charP1_king: {
                        availablepiece = true;
                        check=false;
                        checkmate=true;
                        for (int r = 0; r < 26; ++r)
                        {
                            for (int s = 0; s < 3; ++s)
                            {
                                moves[r][s] = 0;
                            }
                        }
                        movesm = 0;
                        int movesp1[121][2];
                        int movesp1m=0;
                        int ct=0;
                        for (int p = 0; p < 8; ++p)
                        {
                            for (int o = 0; o < 8; ++o)
                            {
                                switch (slots[p][o])
                                {
                                    case PiecesChar::charP2_king:
                                    {
                                        movesp1[movesp1m][0]=p+1;
                                        movesp1[movesp1m][1]=o;
                                        movesp1[movesp1m+1][0]=p+1;
                                        movesp1[movesp1m+1][1]=o+1;
                                        movesp1[movesp1m+2][0]=p;
                                        movesp1[movesp1m+2][1]=o+1;
                                        movesp1[movesp1m+3][0]=p-1;
                                        movesp1[movesp1m+3][1]=o;
                                        movesp1[movesp1m+4][0]=p-1;
                                        movesp1[movesp1m+4][1]=o-1;
                                        movesp1[movesp1m+5][0]=p;
                                        movesp1[movesp1m+5][1]=o-1;
                                        movesp1[movesp1m+6][0]=p+1;
                                        movesp1[movesp1m+6][1]=o-1;
                                        movesp1[movesp1m+7][0]=p-1;
                                        movesp1[movesp1m+7][1]=o+1;
                                        movesp1m=movesp1m+8;
                                        break;
                                    }
                                    case PiecesChar::charP2_queen:
                                        for (k = o + 1; k < 8; k++)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+y) line
                                        for (k = p - 1; - 1 < k; k--)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x) line
                                        for (k = o - 1; - 1 < k; k--)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-y) line
                                        for (k = p + 1; k < 8; k++)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        k = p - 1;
                                        l = o + 1;
                                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        k = p - 1;
                                        l = o - 1;
                                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x;-y) diagonal
                                        k = p + 1;
                                        l = o - 1;
                                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+x;-y) diagonal
                                        k = p + 1;
                                        l = o + 1;
                                        for (; (k < 8) && (l < 8); (k++) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        break;
                                    case PiecesChar::charP2_rook:
                                    {

                                        //(+x) line
                                        for (k = o + 1; k < 8; k++)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+y) line
                                        for (k = p - 1; - 1 < k; k--)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x) line
                                        for (k = o - 1; - 1 < k; k--)
                                        {
                                            if (slots[p][k] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (slots[p][k]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[p][k]))
                                            {
                                                movesp1[movesp1m][0] = p;
                                                movesp1[movesp1m][1] = k;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-y) line
                                        for (k = p + 1; k < 8; k++)
                                        {
                                            if (slots[k][o] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (slots[k][o]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][o]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = o;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        break;
                                    }
                                    case PiecesChar::charP2_knight:
                                    {
                                        if ((p > 1) && (o > 0) &&
                                            ((slots[p - 2][o - 1] == PiecesChar::char_free) || (p1validpieces(slots[p - 2][o - 1]))))
                                        {
                                            movesp1[movesp1m][0] = p - 2;
                                            movesp1[movesp1m][1] = o - 1;
                                            movesp1m++;
                                        }
                                        if ((p > 1) && (o < 7) &&
                                            ((slots[p - 2][o + 1] == PiecesChar::char_free) || (p1validpieces(slots[p - 2][o + 1]))))
                                        {
                                            movesp1[movesp1m][0] = p - 2;
                                            movesp1[movesp1m][1] = o + 1;
                                            movesp1m++;
                                        }
                                        if ((p > 0) && (o > 1) &&
                                            ((slots[p - 1][o - 2] == PiecesChar::char_free) || (p1validpieces(slots[p - 1][o - 2]))))
                                        {
                                            movesp1[movesp1m][0] = p - 1;
                                            movesp1[movesp1m][1] = o - 2;
                                            movesp1m++;
                                        }
                                        if ((p < 7) && (o > 1) &&
                                            ((slots[p + 1][o - 2] == PiecesChar::char_free) || (p1validpieces(slots[p + 1][o - 2]))))
                                        {
                                            movesp1[movesp1m][0] = p + 1;
                                            movesp1[movesp1m][1] = o - 2;
                                            movesp1m++;
                                        }
                                        if ((p < 6) && (o > 0) &&
                                            ((slots[p + 2][o - 1] == PiecesChar::char_free) || (p1validpieces(slots[p + 2][o - 1]))))
                                        {
                                            movesp1[movesp1m][0] = p + 2;
                                            movesp1[movesp1m][1] = o - 1;
                                            movesp1m++;
                                        }
                                        if ((p < 6) && (o < 7) &&
                                            ((slots[p + 2][o + 1] == PiecesChar::char_free) || (p1validpieces(slots[p + 2][o + 1]))))
                                        {
                                            movesp1[movesp1m][0] = p + 2;
                                            movesp1[movesp1m][1] = o + 1;
                                            movesp1m++;
                                        }
                                        if ((p > 0) && (o < 6) &&
                                            ((slots[p - 1][o + 2] == PiecesChar::char_free) || (p1validpieces(slots[p - 1][o + 2]))))
                                        {
                                            movesp1[movesp1m][0] = p - 1;
                                            movesp1[movesp1m][1] = o + 2;
                                            movesp1m++;
                                        }
                                        if ((p < 7) && (o < 6) &&
                                            ((slots[p + 1][o + 2] == PiecesChar::char_free) || (p1validpieces(slots[p + 1][o + 2]))))
                                        {
                                            movesp1[movesp1m][0] = p + 1;
                                            movesp1[movesp1m][1] = o + 2;
                                            movesp1m++;
                                        }
                                        break;
                                    }
                                    case PiecesChar::charP2_bishop:
                                        k = p - 1;
                                        l = o + 1;
                                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        k = p - 1;
                                        l = o - 1;
                                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(-x;-y) diagonal
                                        k = p + 1;
                                        l = o - 1;
                                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        //(+x;-y) diagonal
                                        k = p + 1;
                                        l = o + 1;
                                        for (; (k < 8) && (l < 8); (k++) && (l++))
                                        {
                                            if (slots[k][l] == PiecesChar::char_free)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (slots[k][l]== PiecesChar::charP1_king)
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                            }
                                            else if (p1validpieces(slots[k][l]))
                                            {
                                                movesp1[movesp1m][0] = k;
                                                movesp1[movesp1m][1] = l;
                                                movesp1m++;
                                                break;
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                        break;
                                    case PiecesChar::charP2_pawn:
                                    {
                                        int cas;
                                        if (p == 6)
                                        {
                                            cas = 2;
                                        }
                                        else
                                        {
                                            cas = 1;
                                        }
                                        for (k = 1; k <= cas; k++)
                                        {
                                            if (k == 1)
                                            {
                                                if ((p<7)&&(o<7)&&((p1validpieces(slots[p + 1][o + 1])) || (slots[p + 1][o + 1] == PiecesChar::char_free)))
                                                {
                                                    movesp1[movesp1m][0] = p + 1;
                                                    movesp1[movesp1m][1] = o + 1;
                                                    movesp1m++;
                                                }
                                                if ((p<7)&&(o>0)&&((p1validpieces(slots[p + 1][o - 1])) || (slots[p + 1][o - 1] == PiecesChar::char_free)))
                                                {
                                                    movesp1[movesp1m][0] = p + 1;
                                                    movesp1[movesp1m][1] = o - 1;
                                                    movesp1m++;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                        }
                        int movesking_m=0;
                        int movesking[8][2];
                        if (i>0) {
                            if ((j>0)&&((slots[i-1][j-1] == PiecesChar::char_free)||(p1validpieces(slots[i-1][j-1])))){
                                movesking[movesking_m][0]=i-1;
                                movesking[movesking_m][1]=j-1;
                                movesking_m++;
                            }
                            if ((j<7)&&((slots[i-1][j+1] == PiecesChar::char_free)||(p1validpieces(slots[i-1][j+1])))){
                                movesking[movesking_m][0]=i-1;
                                movesking[movesking_m][1]=j+1;
                                movesking_m++;
                            }
                            if((slots[i-1][j] == PiecesChar::char_free)||(p1validpieces(slots[i-1][j]))){
                                movesking[movesking_m][0]=i-1;
                                movesking[movesking_m][1]=j;
                                movesking_m++;
                            }
                        }
                        if (i<7) {
                            if ((j>0)&&((slots[i+1][j-1] == PiecesChar::char_free)||(p1validpieces(slots[i+1][j-1])))){
                                movesking[movesking_m][0]=i+1;
                                movesking[movesking_m][1]=j-1;
                                movesking_m++;
                            }
                            if ((j<7)&&((slots[i+1][j+1] == PiecesChar::char_free)||(p1validpieces(slots[i+1][j+1])))){
                                movesking[movesking_m][0]=i+1;
                                movesking[movesking_m][1]=j+1;
                                movesking_m++;
                            }
                            if((slots[i+1][j] == PiecesChar::char_free)||(p1validpieces(slots[i+1][j]))){
                                movesking[movesking_m][0]=i+1;
                                movesking[movesking_m][1]=j;
                                movesking_m++;
                            }
                        }
                        if ((j<7)&&((slots[i][j+1] == PiecesChar::char_free)||(p1validpieces(slots[i][j+1])))){
                            cout<<"valid king move"<<endl;
                            movesking[movesking_m][0]=i;
                            movesking[movesking_m][1]=j+1;
                            movesking_m++;
                        }
                        if ((j>0)&&((slots[i][j-1] == PiecesChar::char_free)||(p1validpieces(slots[i][j-1])))){
                            movesking[movesking_m][0]=i;
                            movesking[movesking_m][1]=j-1;
                            movesking_m++;
                        }
                        if((i == 7)&&(j==4)){
                            if((slots[7][7]==PiecesChar::charP1_rook)&&(slots[7][5]==PiecesChar::char_free)&&(slots[7][6]==PiecesChar::char_free)){
                                movesking[movesking_m][0]=7;
                                movesking[movesking_m][1]=6;
                                movesking_m++;
                                kingside_castling_P1=true;
                            }
                            if((slots[7][0]==PiecesChar::charP1_rook)&&(slots[7][3]==PiecesChar::char_free)&&(slots[7][2]==PiecesChar::char_free)&&(slots[7][1]==PiecesChar::char_free)){
                                movesking[movesking_m][0]=7;
                                movesking[movesking_m][1]=2;
                                movesking_m++;
                                queenside_castling_P1=true;
                            }
                        }
                        bool valid_move_king;
                        /*cout<<"checkmate: "<<checkmate<<" , King moves before:"<<endl;
                        for (int m = 0; m < movesking_m; ++m) {
                            cout<<"["<<movesking[m][0]<<"]["<<movesking[m][1]<<"]"<<endl;
                        }*/
                        for (int m = 0; m < movesking_m; ++m) {
                            valid_move_king=true;
                            for (int n = 0; n < movesp1m; ++n) {
                                if((movesp1[n][0]==7)&&(movesp1[n][1]==3)){
                                }
                                if((movesking[m][0]==movesp1[n][0])&&(movesking[m][1]==movesp1[n][1])){
                                    valid_move_king=false;
                                    break;
                                }
                                if ((kingside_castling_P1)&&(movesp1[n][0]==7)&&(movesp1[n][1]==5)){
                                    valid_move_king=false;
                                    kingside_castling_P1=false;
                                    break;
                                }
                                if ((queenside_castling_P1)&&(movesp1[n][0]==7)&&(movesp1[n][1]==3)){
                                    valid_move_king=false;
                                    queenside_castling_P1=false;
                                    break;
                                }
                                if((i==movesp1[n][0])&&(j==movesp1[n][1])){
                                    check=true;
                                }
                            }
                            if(valid_move_king){
                                if((slots[movesking[m][0]][movesking[m][1]]==PiecesChar::char_free) || (p1validpieces(slots[movesking[m][0]][movesking[m][1]]))) {
                                    moves[movesm][0] = movesking[m][0];
                                    moves[movesm][1] = movesking[m][1];
                                    movesm++;
                                    checkmate=false;
                                }
                            }
                        }
                        /*cout<<"checkmate: "<<checkmate<<" , King moves after :"<<endl;
                        for (int m = 0; m < movesking_m; ++m) {
                            cout<<"["<<movesking[m][0]<<"]["<<movesking[m][1]<<"]"<<endl;
                        }*/
                        if ((check)&&(checkmate)){
                            return 100;
                        }
                        break;
                    }
                    case PiecesChar::charP1_queen:
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece = true;
                        for (k = j + 1; k < 8; k++) {
                            if (slots[i][k] == PiecesChar::char_free) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p1validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(+y) line
                        for (k = i - 1; -1 < k; k--) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p1validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        //(-x) line
                        for (k = j - 1; -1 < k; k--) {
                            if (slots[i][k] == PiecesChar::char_free)  {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p1validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(-y) line
                        for (k = i + 1; k < 8; k++) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p1validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        k = i - 1;
                        l = j + 1;
                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if(p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        k = i - 1;
                        l = j - 1;
                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else {
                                break;
                            }
                        }
                        //(-x;-y) diagonal
                        k = i + 1;
                        l = j - 1;
                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        //(+x;-y) diagonal
                        k = i + 1;
                        l = j + 1;
                        for (; (k < 8) && (l < 8); (k++) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        break;
                    case PiecesChar::charP1_rook: {
                        //(+x) line
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece = true;
                        for (k = j + 1; k < 8; k++) {
                            if (slots[i][k] == PiecesChar::char_free) {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p1validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(+y) line
                        for (k = i - 1; -1 < k; k--) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p1validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        //(-x) line
                        for (k = j - 1; -1 < k; k--) {
                            if (slots[i][k] == PiecesChar::char_free)  {
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                            } else if(p1validpieces(slots[i][k])){
                                moves[movesm][0] = i;
                                moves[movesm][1] = k;
                                movesm++;
                                break;
                            } else{
                                break;
                            }
                        }
                        //(-y) line
                        for (k = i + 1; k < 8; k++) {
                            if (slots[k][j] == PiecesChar::char_free) {
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                            } else if (p1validpieces(slots[k][j])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = j;
                                movesm++;
                                break;
                            } else {
                                break;
                            }
                        }
                        break;
                    }
                    case PiecesChar::charP1_knight: {
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
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
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece = true;
                        k = i - 1;
                        l = j + 1;
                        for (; (-1 < k) && (l < 8); (k--) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if(p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        k = i - 1;
                        l = j - 1;
                        for (; (-1 < k) && (-1 < l); (k--) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else {
                                break;
                            }
                        }
                        //(-x;-y) diagonal
                        k = i + 1;
                        l = j - 1;
                        for (; (k < 8) && (-1 < l); (k++) && (l--))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        //(+x;-y) diagonal
                        k = i + 1;
                        l = j + 1;
                        for (; (k < 8) && (l < 8); (k++) && (l++))
                        {
                            if (slots[k][l] == PiecesChar::char_free)
                            {
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                            }
                            else if (p1validpieces(slots[k][l])){
                                moves[movesm][0] = k;
                                moves[movesm][1] = l;
                                movesm++;
                                break;
                            }else{
                                break;
                            }
                        }
                        break;
                    case PiecesChar::charP1_pawn: {
                        for (int r = 0; r < 26; ++r) {
                            for (int s = 0; s < 3; ++s) {
                                moves[r][s]=0;
                            }
                        }
                        movesm = 0;
                        availablepiece = true;
                        int cas;
                        if (i == 6) {
                            cas = 2;
                        } else {
                            cas = 1;
                        }
                        for (k = 1; k <= cas; k++) {
                            if (k==1){
                                if((i>0)&&(j<7)&&(p1validpieces(slots[i-1][j+1]))){
                                    moves[movesm][0] = i-1;
                                    moves[movesm][1] = j+1;
                                    movesm++;
                                }
                                if((i>0)&&(j>0)&&(p1validpieces(slots[i-1][j-1]))){
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
                    for (int p = 0; p < movesm; p++) { // move
                        if (p1validpieces(slots[moves[p][0]][moves[p][1]])) {//points
                            availableeat=true;
                            switch (p1validpieces(slots[moves[p][0]][moves[p][1]])) {
                                case 0://out
                                    moves[p][2] = 0;
                                    break;
                                case 1://king
                                    moves[p][2] = 20;
                                    break;
                                case 2://queen
                                    moves[p][2] = 9;
                                    break;
                                case 3://rook
                                    moves[p][2] = 5;
                                    break;
                                case 4://knight
                                    moves[p][2] = 3;
                                    break;
                                case 5://bishop
                                    moves[p][2] = 3;
                                    break;
                                case 6://pawn
                                    moves[p][2] = 1;
                                    break;
                            }
                        }
                        char slotscopy[8][8];
                        points =pts-moves[p][2];
                        /*//DEBUGGING---------------------
                        contador++;
                        cout<<contador<<" Profundidad: "<<depth<<" Retornando points:" <<points<<" Player = "<<player<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;
                        for (int m = 0; m < movesm; ++m) {
                            for (int n = 0; n < 3; ++n) {
                                cout<<moves[m][n]<<" ";
                            }
                            cout<<endl;
                        }
                        //DEBUGGING---------------------*/
                        copy(slots,slotscopy);
                        if((slots[i][j]==PiecesChar::charP1_king)&&(kingside_castling_P1)&&(moves[p][0]==7)&&(moves[p][1]==6)){
                            slotscopy[7][7]=PiecesChar::char_free;
                            slotscopy[7][5]=PiecesChar::charP1_rook;
                        }
                        if((slots[i][j]==PiecesChar::charP1_king)&&(kingside_castling_P1)&&(moves[p][0]==7)&&(moves[p][1]==2)){
                            slotscopy[7][0]=PiecesChar::char_free;
                            slotscopy[7][3]=PiecesChar::charP1_rook;
                        }
                        temp = slotscopy[i][j];
                        slotscopy[i][j] = slotscopy[moves[p][0]][moves[p][1]];
                        slotscopy[moves[p][0]][moves[p][1]] = temp;
                        if(availableeat){
                            slotscopy[i][j]=PiecesChar::char_free;
                        }
                        /*//DEBUGGING---------------------
                        for (int i = 0; i < 8; ++i) {
                            for (int j = 0; j < 8; ++j) {
                                cout<<slotscopy[i][j]<<" ";
                            }
                            cout<<endl;
                        }
                        //DEBUGGING---------------------*/
                        int eval=minimax(slotscopy, depth - 1, true, points,arrr,arrrms);
                        minpt = min(eval, minpt);
                    }
                    /*cout<<"End moves"<<" Pieza["<<i<<"]["<<j<<"] : "<<slots[i][j] <<endl;*/
                }
            }
        }
        /*cout<<"End board"<<endl;
        cout<<"\nMainimo valor eval : "<<minpt<<endl;*/
        return  minpt;
    }
}

