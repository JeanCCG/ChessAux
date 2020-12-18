#pragma once

#include "../enums/enums.h"



class Piece {
public:
    char symbol;
    bool isFree;
    bool player;
    int points;
    int movements = 0;

    Piece();

    // Piece(char, bool, bool, int);
    Piece(char, bool, bool, int);

    // void moveTo(int[2]);
    void moveTo(int [2]);

    // void setFree();
    void setFree();

    void setPiece(Piece);
};
