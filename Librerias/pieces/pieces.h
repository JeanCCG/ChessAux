#pragma once

#include "../enums/enums.h"

class Piece {
public:
    char symbol;
    bool isFree;
    bool player;
    int points;
    int movements = 0;
    int enemyMenacing = 0;
    int playerMenacing = 0;
    int winPotential = 0;  // Lo que se puede ganar si se mueve a una posición.
    int lostPotential = 0; // Lo que se puede perder si no se defiende una posición

    Piece();

    // Piece(char, bool, bool, int);
    Piece(char, bool, bool, int);

    // void moveTo(int[2]);
    void moveTo(int place[2]);

    // void setFree();
    void setFree();

    void setPiece(Piece piece);
    // void setPiece(Piece);
    // bool inCheck();
    // void eat(Piece);
    // void capture(Piece);
protected:
private:
};
