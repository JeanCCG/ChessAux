#include <iostream>
#include "../interface/interface.h"
#include "gameboard.h"

using namespace std;
int P1_score = 0;
int P2_score = 0;

void cleanConsole() {
    system("cls");
    OnceAnnouncement title(5);
}

Gameboard::Gameboard(int P1PiecesInit[][3], int nP1Pieces, int P2PiecesInit[][3], int nP2Pieces, int iWidth,
                     int iHeight) {
    cout << "Inside the Gameboard" << endl;
    width = iWidth;
    height = iHeight;

    initGameboard(P1PiecesInit, nP1Pieces, P2PiecesInit, nP2Pieces);
    initShowVars();
}

void Gameboard::move(int start[2], int end[2]) // end MUST BE FREE
{
    slots[end[0]][end[1]].setPiece(slots[start[0]][start[1]]);
    slots[start[0]][start[1]].setFree();
}

void Gameboard::capture(int start[2], int end[2]) {

    if (slots[start[0]][start[1]].player) //P1
    {
        P1_score += slots[end[0]][end[1]].points;
    } else {
        P2_score += slots[end[0]][end[1]].points;
    }
    slots[end[0]][end[1]].setFree();
    move(start, end);
}

void Gameboard::eat(int start[2], int end[2]) {
    capture(start, end);
}

int Gameboard::goDiagonal(int start[2], int end[2]) // Diagonal
{
    int availableSlots = 1;
    if ((abs(start[0] - end[0]) == 1) && (abs(start[1] - end[1]) == 1))
        return 1;
    else if (abs(start[0] - end[0]) == abs(start[1] - end[1])) {
        bool isUp = ((start[0] - end[0]) > 0) ? true : false;
        bool isLeft = ((start[1] - end[1]) > 0) ? true : false;
        int i, it, iLimit;
        int j, jt, jLimit;
        if (isUp) {
            iLimit = -1;
            i = start[0] - 1;
            it = -1;
        } else {
            iLimit = height;
            i = start[0] + 1;
            it = 1;
        }
        if (isLeft) {
            jLimit = -1;
            j = start[1] - 1;
            jt = -1;
        } else {
            jLimit = width;
            j = start[1] + 1;
            jt = 1;
        }

        for (; (i != iLimit) && (j != jLimit); (i += it) && (j += jt)) {
            if (slots[i][j].symbol != PiecesChar::char_free)
                return availableSlots;
            availableSlots++;
        }
        return availableSlots;
    } else
        return 0;
}

bool Gameboard::goStraight(int start[2], int end[2]) // Línea recta
{
    if ((abs(start[0] - end[0]) == 0) && (abs(start[1] - end[1]) == 0))
        return true;
    else if (start[0] == end[0]) {
        bool isLeft = ((start[1] - end[1]) > 0) ? true : false;
        bool isAPieceInMiddle = false;
        int j, jt;
        int jLimit = end[1];
        if (isLeft) {
            j = start[1] - 1;
            jt = -1;
        } else {
            j = start[1] + 1;
            jt = 1;
        }
        for (; (j != jLimit); (j += jt))
            if (slots[start[0]][j].symbol != PiecesChar::char_free)
                return false;
        if (slots[end[0]][end[1]].isFree)
            move(start, end);
        else
            eat(start, end);

        return true;
    } else if (start[1] == end[1]) {
        bool isUp = ((start[0] - end[0]) > 0) ? true : false;
        int i, it;
        int iLimit = end[0];
        if (isUp) {
            i = start[0] - 1;
            it = -1;
        } else {
            i = start[0] + 1;
            it = 1;
        }
        for (; (i != iLimit); (i += it))
            if (slots[i][start[1]].symbol != PiecesChar::char_free)
                return false;
        if (slots[end[0]][end[1]].isFree)
            move(start, end);
        else
            eat(start, end);
        return true;
    } else
        return false;
}

bool Gameboard::jump(int start[2], int end[2]) // Salto de caballo
{
    int xDistance = abs(start[0] - end[0]);
    int yDistance = abs(start[1] - end[1]);
    if (
            ((xDistance == 1) && (yDistance == 2)) ||
            ((xDistance == 2) && (yDistance == 1))) {

        if (slots[end[0]][end[1]].isFree)
            move(start, end);
        else
            eat(start, end);
        return true;
    } else
        return false;
}

bool Gameboard::validKingMovement(int start[2], int end[2]) {
    return true;
}

bool Gameboard::validEnPassant(int start[2], int end[2]) // Peón al paso
{
    return true;
}

bool Gameboard::isMenaced(int place[2]) {
    return true;
}

void Gameboard::drawDot(int place[2]) {
    slots[place[0]][place[1]].symbol = PiecesChar::char_dot;
}
// void undrawKingDots(int place[2]);
// {
// 	place[0] = place[0];
// }

bool Gameboard::availableKingMovement(int kingBearings[2]) {
    bool availableMovement = false;
    bool inTheUpperBorder = (kingBearings[0] == 0) ? true : false;
    bool inTheBottomBorder = (kingBearings[0] == height - 1) ? true : false;
    bool inTheLeftBorder = (kingBearings[1] == 0) ? true : false;
    bool inTheRightBorder = (kingBearings[1] == width - 1) ? true : false;
    bool slotMenaced;
    bool player = (slots[kingBearings[0]][kingBearings[1]].player == Player::P1) ? Player::P1 : Player::P2;
    bool availableQueensideCastling = true;
    bool availableKingsideCastling = true;

    int end[2];
    // int i = 0;
    int iLimit = 4;
    // int j = 0;
    int jLimit = 4;
    // if (inTheUpperBorder)
    // 	i = 1;
    if (inTheBottomBorder)
        iLimit = 3;
    if (inTheRightBorder)
        jLimit = 3;
    // if (inTheLeftBorder)
    // 	j = 1;

    for (int i = (inTheUpperBorder) ? 1 : 0; i < iLimit; i++)
        for (int j = (inTheLeftBorder) ? 1 : 0; j < jLimit; j++) {
            end[0] = kingBearings[0] + (-1 + i);
            end[1] = kingBearings[1] + (-1 + j);
            slotMenaced = isMenaced(end);
            if ((slotMenaced == false) &&
                (slots[end[0]][end[1]].isFree)) {
                drawDot(end);
                availableMovement = true;
            } else if (slots[end[0]][end[1]].player != player)
                availableMovement = true;
        }
    if (slots[kingBearings[0]][kingBearings[1]].movements == 0) {
        if (slots[kingBearings[0]][0].movements == 0) {
            for (int j = kingBearings[1] - 1; 0 < j; j--)
                if (slots[kingBearings[0]][j].isFree == false) {
                    availableQueensideCastling = false;
                    break;
                }
        }
        if (slots[kingBearings[0]][width - 1].movements == 0) {
            for (int j = kingBearings[1] + 1; j < width - 1; j++)
                if (slots[kingBearings[0]][j].isFree == false) {
                    availableKingsideCastling = false;
                    break;
                }
        }
    }
    if (availableKingsideCastling) {
        availableMovement = true;
        end[0] = kingBearings[0];
        end[1] = 2;
        drawDot(end);
    }
    if (availableQueensideCastling) {
        availableMovement = true;
        end[0] = kingBearings[0];
        end[1] = width - 3;
        drawDot(end);
    }
    return availableMovement;
}

bool Gameboard::piecePossibilities(int place[2]) // ONLY P1 need visual reference
{
    cout << "piecePossibilities" << endl;
    int piecePossibilities;
    bool availableMovement = false;
    switch (slots[place[0]][place[1]].symbol) {
        case PiecesChar::charP1_king:
            piecePossibilities = 0;
            availableMovement = availableKingMovement(place);
            break;
        case PiecesChar::charP1_queen:
            piecePossibilities = 1;
            if (drawDiagonals(place))
                availableMovement = true;
            if (drawLines(place))
                availableMovement = true;
            break;
        case PiecesChar::charP1_rook:
            piecePossibilities = 2;
            availableMovement = drawLines(place);
            break;
        case PiecesChar::charP1_knight:
            piecePossibilities = 3;
            availableMovement = drawJumps(place);
            break;
        case PiecesChar::charP1_bishop:
            piecePossibilities = 4;
            availableMovement = drawDiagonals(place);
            break;
        case PiecesChar::charP1_pawn:
            availableMovement = drawLines_P(place);
            piecePossibilities = 5;
            break;

        default:
            break;
    }
    if (availableMovement) {
        show();
        switch (piecePossibilities) {
            case 0:
                // undrawKingDots(place);
                break;
            case 1:
                undrawDiagonals(place);
                undrawLines(place);
                break;
            case 2:
                undrawLines(place);
                break;
            case 3:
                undrawJumps(place);
                break;
            case 4:
                undrawDiagonals(place);
                break;
            case 5:
                undrawLines_P(place);
                break;

            default:
                break;
        }
        return true;
    } else
        return false;
}

void Gameboard::show() {
    cleanConsole();
    // Letters, upper gameframe
    cout << equatorBlank << meridianChar;
    for (int i = 0; i < width; i++) {
        slotPiece[slotWidth / 2] = ((char) letter + i);
        cout << slotPiece << meridianChar;
    }
    cout << endl;
    // Gameboard
    for (int i = 0; i < height; i++) {
        //first frame
        cout << equatorFrame << meridianChar;
        for (int j = 0; j < width; j++) {
            cout << equatorSlot << meridianChar;
        }
        cout << equatorFrame << endl;

        for (int j = 0; j < 2; j++) {
            // Left frame (equator)
            if (j == 1) {
                cout << blankChar << number + i << blankChar << meridianChar;
            } else {
                cout << equatorBlank << meridianChar;
            }
            // Slot
            for (int o = 0; o < slotWidth + 1; o++) {
                if (j == 1) {
                    slotPiece[slotWidth / 2] = slots[i][o].symbol;
                    cout << slotPiece << meridianChar;
                } else {
                    cout << slotBlank << meridianChar;
                }
            }
            //Right Frame (equator)
            cout << equatorBlank << endl;
        }
    }
    // Letters, bottom gameframe
    cout << equatorFrame << meridianChar;
    for (int j = 0; j < width; j++) {
        cout << equatorSlot << meridianChar;
    }
    cout << equatorFrame << endl;
    cout << equatorBlank << meridianChar;
    for (int i = 0; i < width; i++) {
        slotPiece[slotWidth / 2] = ((char) letter + i);
        cout << slotPiece << meridianChar;
    }
    cout << endl;
}


void Gameboard::initShowVars() {
    equatorFrame = new char[equatorFrameWidth];
    equatorSlot = new char[slotWidth];
    equatorBlank = new char[equatorFrameWidth];
    meridianFrame = new char[meridianFrameWidth];
    meridianBlank = new char[meridianFrameWidth];
    slotPiece = new char[slotWidth];
    slotBlank = new char[slotWidth];

    for (int i = 0; i < equatorFrameWidth; i++) {
        equatorFrame[i] = equatorChar;
        equatorBlank[i] = blankChar;
    }
    for (int i = 0; i < meridianFrameWidth; i++) {
        meridianFrame[i] = meridianChar;
        meridianBlank[i] = blankChar;
    }
    for (int i = 0; i < slotWidth; i++) {
        equatorSlot[i] = equatorChar;
        slotBlank[i] = blankChar;
        slotPiece[i] = blankChar;
    }
}

bool Gameboard::drawLines_P(int place[2]) {
    bool availableMovement = false;
    int p_y = place[0] - 1;
    int cas = 0;
    if (place[0] == 6) {
        cas = 2;
    } else {
        cas = 1;
    }
    for (int i = 0; i < cas; i++) {
        if (slots[p_y][place[1]].symbol == PiecesChar::char_free) {
            slots[p_y][place[1]].symbol = '*';
            availableMovement = true;
            p_y--;
        } else {
            break;
        }
    }
    return availableMovement;
}

void Gameboard::undrawLines_P(int place[2]) {
    int p_y = place[0] - 1;
    int cas = 0;
    if (place[0] == 6) {
        cas = 2;
    } else {
        cas = 1;
    }
    for (int j = 0; j < cas; j++) {
        if (slots[p_y][place[1]].symbol == '*') {
            slots[p_y][place[1]].symbol = PiecesChar::char_free;
            p_y--;
        } else {
            break;
        }
    }
}

void Gameboard::undrawLines(int place[2]) {
    //(+x) line
    for (int j = place[1] + 1; j < width; j++) {
        if (slots[place[0]][j].symbol == '*')
            slots[place[0]][j].symbol = PiecesChar::char_free;
        else
            break;
    }
    //(+y) line
    for (int i = place[0] - 1; -1 < i; i--) {
        if (slots[i][place[1]].symbol == '*')
            slots[i][place[1]].symbol = PiecesChar::char_free;
        else
            break;
    }
    //(-x) line
    for (int j = place[1] - 1; -1 < j; j--) {
        if (slots[place[0]][j].symbol == '*')
            slots[place[0]][j].symbol = PiecesChar::char_free;
        else
            break;
    }
    //(-y) line
    for (int i = place[0] + 1; i < height; i++) {
        if (slots[i][place[1]].symbol == '*')
            slots[i][place[1]].symbol = PiecesChar::char_free;
        else
            break;
    }
}

bool Gameboard::drawJumps(int place[2]) //Knight Jumps
{
    bool availableMovement = false;
    if ((place[0] > 1) && (place[1] > 0) && (slots[place[0] - 2][place[1] - 1].symbol == PiecesChar::char_free)) {
        slots[place[0] - 2][place[1] - 1].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] > 1) && (place[1] < 7) && (slots[place[0] - 2][place[1] + 1].symbol == PiecesChar::char_free)) {
        slots[place[0] - 2][place[1] + 1].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] > 0) && (place[1] > 1) && (slots[place[0] - 1][place[1] - 2].symbol == PiecesChar::char_free)) {
        slots[place[0] - 1][place[1] - 2].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] < 7) && (place[1] > 1) && (slots[place[0] + 1][place[1] - 2].symbol == PiecesChar::char_free)) {
        slots[place[0] + 1][place[1] - 2].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] < 6) && (place[1] > 0) && (slots[place[0] + 2][place[1] - 1].symbol == PiecesChar::char_free)) {
        slots[place[0] + 2][place[1] - 1].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] < 6) && (place[1] < 7) && (slots[place[0] + 2][place[1] + 1].symbol == PiecesChar::char_free)) {
        slots[place[0] + 2][place[1] + 1].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] > 0) && (place[1] < 6) && (slots[place[0] - 1][place[1] + 2].symbol == PiecesChar::char_free)) {
        slots[place[0] - 1][place[1] + 2].symbol = '*';
        availableMovement = true;
    }
    if ((place[0] < 7) && (place[1] < 6) && (slots[place[0] + 1][place[1] + 2].symbol == PiecesChar::char_free)) {
        slots[place[0] + 1][place[1] + 2].symbol = '*';
        availableMovement = true;
    }
    return availableMovement;
}

void Gameboard::undrawJumps(int place[2]) {
    if ((place[0] > 1) && (place[1] > 0) && (slots[place[0] - 2][place[1] - 1].symbol == '*')) {
        slots[place[0] - 2][place[1] - 1].symbol = PiecesChar::char_free;
    }
    if ((place[0] > 1) && (place[1] < 7) && (slots[place[0] - 2][place[1] + 1].symbol == '*')) {
        slots[place[0] - 2][place[1] + 1].symbol = PiecesChar::char_free;
    }
    if ((place[0] > 0) && (place[1] > 1) && (slots[place[0] - 1][place[1] - 2].symbol == '*')) {
        slots[place[0] - 1][place[1] - 2].symbol = PiecesChar::char_free;
    }
    if ((place[0] < 7) && (place[1] > 1) && (slots[place[0] + 1][place[1] - 2].symbol == '*')) {
        slots[place[0] + 1][place[1] - 2].symbol = PiecesChar::char_free;
    }
    if ((place[0] < 6) && (place[1] > 0) && (slots[place[0] + 2][place[1] - 1].symbol == '*')) {
        slots[place[0] + 2][place[1] - 1].symbol = PiecesChar::char_free;
    }
    if ((place[0] < 6) && (place[1] < 7) && (slots[place[0] + 2][place[1] + 1].symbol == '*')) {
        slots[place[0] + 2][place[1] + 1].symbol = PiecesChar::char_free;
    }
    if ((place[0] > 0) && (place[1] < 6) && (slots[place[0] - 1][place[1] + 2].symbol == '*')) {
        slots[place[0] - 1][place[1] + 2].symbol = PiecesChar::char_free;
    }
    if ((place[0] < 7) && (place[1] < 6) && (slots[place[0] + 1][place[1] + 2].symbol == '*')) {
        slots[place[0] + 1][place[1] + 2].symbol = PiecesChar::char_free;
    }
}

bool Gameboard::drawLines(int place[2]) {
    bool availableMovement = false;
    //(+x) line
    for (int j = place[1] + 1; j < width; j++) {
        if (slots[place[0]][j].symbol == PiecesChar::char_free) {
            slots[place[0]][j].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    //(+y) line
    for (int i = place[0] - 1; -1 < i; i--) {
        if (slots[i][place[1]].symbol == PiecesChar::char_free) {
            slots[i][place[1]].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    //(-x) line
    for (int j = place[1] - 1; -1 < j; j--) {
        if (slots[place[0]][j].symbol == PiecesChar::char_free) {
            slots[place[0]][j].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    //(-y) line
    for (int i = place[0] + 1; i < height; i++) {
        if (slots[i][place[1]].symbol == PiecesChar::char_free) {
            slots[i][place[1]].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    return availableMovement;
}

void Gameboard::undrawDiagonals(int place[2]) {
    int i, j;
    //(+x;+y) diagonal
    i = place[0] - 1;
    j = place[1] + 1;
    for (; (-1 < i) && (j < width); (i--) && (j++)) {
        if (slots[i][j].symbol == '*') {
            slots[i][j].symbol = PiecesChar::char_free;
        } else
            break;
    }
    //(-x;+y) diagonal
    i = place[0] - 1;
    j = place[1] - 1;
    for (; (-1 < i) && (-1 < j); (i--) && (j--)) {
        if (slots[i][j].symbol == '*') {
            slots[i][j].symbol = PiecesChar::char_free;
        } else
            break;
    }
    //(-x;-y) diagonal
    i = place[0] + 1;
    j = place[1] - 1;
    for (; (i < height) && (-1 < j); (i++) && (j--)) {
        if (slots[i][j].symbol == '*') {
            slots[i][j].symbol = PiecesChar::char_free;
        } else
            break;
    }
    //(+x;-y) diagonal
    i = place[0] + 1;
    j = place[1] + 1;
    for (; (i < height) && (j < width); (i++) && (j++)) {
        if (slots[i][j].symbol == '*') {
            slots[i][j].symbol = PiecesChar::char_free;
        } else
            break;
    }
}

bool Gameboard::drawDiagonals(int place[2]) {
    bool availableMovement = false;
    int i, j;

    i = place[0] - 1;
    j = place[1] + 1;
    for (; (-1 < i) && (j < width); (i--) && (j++)) {
        if (slots[i][j].symbol == PiecesChar::char_free) {
            slots[i][j].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    i = place[0] - 1;
    j = place[1] - 1;
    for (; (-1 < i) && (-1 < j); (i--) && (j--)) {
        if (slots[i][j].symbol == PiecesChar::char_free) {
            slots[i][j].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    //(-x;-y) diagonal
    i = place[0] + 1;
    j = place[1] - 1;
    for (; (i < height) && (-1 < j); (i++) && (j--)) {
        if (slots[i][j].symbol == PiecesChar::char_free) {
            slots[i][j].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    //(+x;-y) diagonal
    i = place[0] + 1;
    j = place[1] + 1;
    for (; (i < height) && (j < width); (i++) && (j++)) {
        if (slots[i][j].symbol == PiecesChar::char_free) {
            slots[i][j].symbol = '*';
            availableMovement = true;
        } else
            break;
    }
    return availableMovement;
}

void Gameboard::initGameboard(int P1PiecesInit[][3], int nP1Pieces, int P2PiecesInit[][3], int nP2Pieces) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            slots[i][j] = Piece(PiecesChar::char_free, 1, 0, 0);
        }
    }
    int points;
    for (int i = 0; i < nP1Pieces; i++) {
        switch (P1PiecesInit[i][2]) {
            case PiecesChar::charP1_king:
                points = PiecesPoints::kingPoints;
                break;
            case PiecesChar::charP1_queen:
                points = PiecesPoints::queenPoints;
                break;
            case PiecesChar::charP1_rook:
                points = PiecesPoints::rookPoints;
                break;
            case PiecesChar::charP1_knight:
                points = PiecesPoints::knightPoints;
                break;
            case PiecesChar::charP1_bishop:
                points = PiecesPoints::bishopPoints;
                break;
            case PiecesChar::charP1_pawn:
                points = PiecesPoints::pawnPoints;
                break;

            default:
                points = 0;
                break;
        }
        slots[P1PiecesInit[i][0]][P1PiecesInit[i][1]] = Piece((char) P1PiecesInit[i][2], 0, 1, points);
    }
    for (int i = 0; i < nP2Pieces; i++) {
        switch (P2PiecesInit[i][2]) {
            case PiecesChar::charP2_king:
                points = PiecesPoints::kingPoints;
                break;
            case PiecesChar::charP2_queen:
                points = PiecesPoints::queenPoints;
                break;
            case PiecesChar::charP2_rook:
                points = PiecesPoints::rookPoints;
                break;
            case PiecesChar::charP2_knight:
                points = PiecesPoints::knightPoints;
                break;
            case PiecesChar::charP2_bishop:
                points = PiecesPoints::bishopPoints;
                break;
            case PiecesChar::charP2_pawn:
                points = PiecesPoints::pawnPoints;
                break;

            default:
                points = 0;
                break;
        }
        slots[P2PiecesInit[i][0]][P2PiecesInit[i][1]] = Piece((char) P2PiecesInit[i][2], 0, 1, points);
    }
}
