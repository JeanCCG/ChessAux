#pragma once

enum PiecesPoints
{
	int_eaten = -1,
	int_free = 0,
	kingPoints = 1000,
	queenPoints = 9,
	rookPoints = 5,
	knightPoints = 3,
	bishopPoints = 3,
	pawnPoints = 1
};

enum PiecesChar
{
	char_free = ' ',
	char_dot = '*',
	// P1 is always in minuscules
	charP1_king = 'r',
	charP1_queen = 'q',
	charP1_rook = 't',
	charP1_knight = 'c',
	charP1_bishop = 'a',
	charP1_pawn = 'p',

	// P2 IS ALWAYS IN MAJUSCULES
	charP2_king = 'R',
	charP2_queen = 'Q',
	charP2_rook = 'T',
	charP2_knight = 'C',
	charP2_bishop = 'A',
	charP2_pawn = 'P'

};

enum Player
{
    Difficulty = 3,
	P1 = true,
	P2 = false
};
