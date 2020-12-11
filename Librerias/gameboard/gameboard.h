#include "../pieces/pieces.h"

void cleanConsole();

class Gameboard {
public:
    // 2D ptrptr?
    Piece slots[26][26];
    int width;
    int height;

    // Gameboard() {}
    Gameboard(int [][3], int, int [][3], int, int, int);

    void move(int [2], int [2]); // end MUST BE FREE
    void capture(int [2], int [2]);

    void eat(int [2], int [2]);

    int goDiagonal(int [2], int [2]); // Diagonal
    bool goStraight(int [2], int [2]); // Línea recta
    bool jump(int [2], int [2]); // Salto de caballo
    bool validKingMovement(int [2], int [2]);

    bool validEnPassant(int [2], int [2]); // Peón al paso
    bool isMenaced(int [2]);

    void drawDot(int [2]);
    // void undrawKingDots(int place[2]);
    // {
    // 	place[0] = place[0];
    // }

    bool availableKingMovement(int [2]);

    bool piecePossibilities(int [2]); // ONLY P1 need visual reference
    void show();

private:
    int slotWidth = 7;
    int equatorFrameWidth = 3;
    int meridianFrameWidth = 1;
    char blankChar = ' ';
    char equatorChar = '_';
    char meridianChar = '|';
    char letter = 'A';
    int number = 1;
    char *equatorFrame;
    char *equatorSlot;
    char *equatorBlank;
    char *meridianFrame;
    char *meridianBlank;
    char *slotPiece;
    char *slotBlank;

    void initShowVars();

    bool drawLines_P(int [2]);

    void undrawLines_P(int [2]);

    void undrawLines(int [2]);

    bool drawJumps(int [2]); //Knight Jumps
    void undrawJumps(int [2]);

    bool drawLines(int [2]);

    void undrawDiagonals(int [2]);

    bool drawDiagonals(int [2]);

    void initGameboard(int [][3], int, int [][3], int);

};
