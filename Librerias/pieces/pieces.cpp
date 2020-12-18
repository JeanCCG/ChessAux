#include "pieces.h"
#include "../enums/enums.h"

/*** REPOSITORIO EN GITHUB
 * https://github.com/JeanCCG/ChessAux
 * Autores:
 * Jean Carlo Gino Condori Gonzales
 * Geraldo Mark Romero Guillén
 * Gabriel Santiago Choque Mayta
 * José Enrique Vilca Campana
 */

Piece::Piece() {}

// Piece(char, bool, bool, int);
Piece::Piece(char iSymbol, bool iIsFree, bool iPlayer, int iPoints)
{
	symbol = iSymbol;
	isFree = iIsFree;
	if (isFree == false)
		player = iPlayer; // P1 = TRUE-+
	points = iPoints;
}

// void moveTo(int[2]);
void Piece::moveTo(int place[2])
{
	movements++;
}

// void setFree();
void Piece::setFree()
{
	symbol = PiecesChar::char_free;
	isFree = true;
	points = 0;
	movements = 0;
	// playerMenacing++;
}

void Piece::setPiece(Piece piece)
{
	symbol = piece.symbol;
	isFree = false;
	player = piece.player;
	points = piece.points;
	movements += piece.movements + 1;
}
