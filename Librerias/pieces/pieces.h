// #ifndef CHESS_UCSP_PIECES_H
// #define CHESS_UCSP_PIECES_H
// #endif

#pragma once
#include "../enums/enums.h"

int P1_score = 0;
int P2_score = 0;
class Piece
{
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

	Piece() {}
	// Piece(char, bool, bool, int);
	Piece(char iSymbol, bool iIsFree, bool iPlayer, int iPoints)
	{
		symbol = iSymbol;
		isFree = iIsFree;
		if (isFree == false)
			player = iPlayer; // P1 = TRUE-+
		points = iPoints;
	}
	// void moveTo(int[2]);
	void moveTo(int place[2])
	{
		movements++;
	}
	// void setFree();
	void setFree()
	{
		symbol = PiecesChar::char_free;
		isFree = true;
		points = 0;
		movements = 0;
		playerMenacing++;
	}
	void setPiece(Piece piece)
	{
		symbol = piece.symbol;
		isFree = false;
		player = piece.player;
		points = piece.points;
		movements += piece.movements;
		playerMenacing--;
	}

	// void setPiece(Piece);
	// bool inCheck();
	// void eat(Piece);
	// void capture(Piece);
protected:
private:
};
