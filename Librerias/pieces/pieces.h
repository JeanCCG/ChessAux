#pragma once

#include "../enums/enums.h"

/*** REPOSITORIO EN GITHUB
 * https://github.com/JeanCCG/ChessAux
 * Autores:
 * Jean Carlo Gino Condori Gonzales
 * Geraldo Mark Romero Guillén
 * Gabriel Santiago Choque Mayta
 * José Enrique Vilca Campana
 */

class Piece
{
public:
	char symbol;
	bool isFree;
	bool player;
	int points;
	int movements = 0;

	Piece();

	Piece(char, bool, bool, int);

	void moveTo(int[2]);

	void setFree();

	void setPiece(Piece);
};
