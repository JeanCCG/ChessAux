#include <iostream>
#include "../interface/interface.h"
#include "gameboard.h"

using namespace std;

int P1_score = 0;
int P2_score = 0;
int P1_kingBearings[2];
int P2_kingBearings[2];

void clean_screan()
{
	system("cls");
	OnceAnnouncement title(5);
}

Gameboard::Gameboard(int P1PiecesInit[][3], int nP1Pieces, int P2PiecesInit[][3], int nP2Pieces, int iWidth, int iHeight)
{
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

void Gameboard::capture(int start[2], int end[2])
{
	if (slots[start[0]][start[1]].player == Player::P1) //P1
		P1_score += slots[end[0]][end[1]].points;
	else
		P2_score += slots[end[0]][end[1]].points;
	slots[end[0]][end[1]].setFree();
	move(start, end);
}

void Gameboard::eat(int start[2], int end[2])
{
	capture(start, end);
}

bool Gameboard::goDiagonal(int start[2], int end[2]) // Diagonal
{
	if ((abs(start[0] - end[0]) == 1) && (abs(start[1] - end[1]) == 1))
	{
		if (slots[end[0]][end[1]].isFree)
			move(start, end);
		else
			eat(start, end);
		return true;
	}
	else if (abs(start[0] - end[0]) == abs(start[1] - end[1]))
	{
		cout << "ELSE IF" << endl;
		bool isUp = ((start[0] - end[0]) > 0) ? true : false;
		bool isLeft = ((start[1] - end[1]) > 0) ? true : false;
		int i, it;
		int iLimit = end[0];
		int j, jt;
		int jLimit = end[1];
		if (isUp)
		{
			i = start[0] - 1;
			it = -1;
		}
		else
		{
			i = start[0] + 1;
			it = 1;
		}
		if (isLeft)
		{
			j = start[1] - 1;
			jt = -1;
		}
		else
		{
			j = start[1] + 1;
			jt = 1;
		}
		for (; (i != iLimit) && (j != jLimit); (i += it) && (j += jt))
			if (slots[i][j].symbol != PiecesChar::char_free)
				return false;
		if (slots[end[0]][end[1]].isFree)
			move(start, end);
		else
			eat(start, end);
		return true;
	}
	else
		return false;
}

bool Gameboard::goStraight(int start[2], int end[2]) // Línea recta
{
	if ((abs(start[0] - end[0]) == 0) && (abs(start[1] - end[1]) == 0))
	{
		if (slots[end[0]][end[1]].isFree)
			move(start, end);
		else
			eat(start, end);
		return true;
	}
	else if (start[0] == end[0])
	{

		bool isLeft = ((start[1] - end[1]) > 0) ? true : false;
		bool isAPieceInMiddle = false;
		int j, jt;
		int jLimit = end[1];
		if (isLeft)
		{
			j = start[1] - 1;
			jt = -1;
		}
		else
		{
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
	}
	else if (start[1] == end[1])
	{
		bool isUp = ((start[0] - end[0]) > 0) ? true : false;
		int i, it;
		int iLimit = end[0];
		if (isUp)
		{
			i = start[0] - 1;
			it = -1;
		}
		else
		{
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
	}
	else
		return false;
}

bool Gameboard::jump(int start[2], int end[2]) // Salto de caballo
{
	int xDistance = abs(start[0] - end[0]);
	int yDistance = abs(start[1] - end[1]);
	if (((xDistance == 1) && (yDistance == 2)) ||
		((xDistance == 2) && (yDistance == 1)))
	{
		if (slots[end[0]][end[1]].isFree)
			move(start, end);
		else
			eat(start, end);
		return true;
	}
	else
		return false;
}

bool Gameboard::isMenaced(int place[2], bool player)
{
	std::cout << "|->" << place[0] << " , " << place[1] << "<-|" << std::endl;
	char bishop, queen, rook, knight, pawn, king;
	bool isMyKingInMiddle = false;
	// int pawnDirection;
	if (player == Player::P1)
	{
		king = PiecesChar::charP1_king;
		bishop = PiecesChar::charP2_bishop;
		queen = PiecesChar::charP2_queen;
		rook = PiecesChar::charP2_rook;
		knight = PiecesChar::charP2_knight;
		pawn = PiecesChar::charP2_knight;
		// pawnDirection = 1;
	}
	else
	{
		king = PiecesChar::charP2_king;
		pawn = PiecesChar::charP1_bishop;
		bishop = PiecesChar::charP1_bishop;
		queen = PiecesChar::charP1_queen;
		rook = PiecesChar::charP1_rook;
		knight = PiecesChar::charP1_knight;
		// pawnDirection = -1;
	}
	////////////*
	//* straight
	////////////*
	std::cout << "rook menacing?" << std::endl;
	//(+x) line
	std::cout << "(+x) line" << std::endl;
	for (int j = place[1] + 1; j < width; j++)
	{
		std::cout << "slots[" << place[0] << "][" << j << "] : ";
		if (slots[place[0]][j].isFree == false)
		{
			std::cout << "is NOT Free : " << slots[place[0]][j].symbol << std::endl;
			std::cout << slots[place[0]][j].player << " !players= " << player << std::endl;
			if ((slots[place[0]][j].player != player) &&
				((slots[place[0]][j].symbol == rook) ||
				 (slots[place[0]][j].symbol == queen)))
				return true;
			else if ((slots[place[0]][j].player == player))
			{
				if (slots[place[0]][j].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
		else
			std::cout << "isFree : " << slots[place[0]][j].symbol << std::endl;
	}
	//(+y) line
	std::cout << "(+y) line" << std::endl;
	for (int i = place[0] - 1; - 1 < i; i--)
	{
		std::cout << "slots[" << i << "][" << place[1] << "] : ";
		if (slots[i][place[1]].isFree == false)
		{
			std::cout << "is NOT Free" << std::endl;
			std::cout << slots[i][place[1]].player << " !players= " << player << std::endl;
			if ((slots[i][place[1]].player != player) &&
				((slots[i][place[1]].symbol == rook) ||
				 (slots[i][place[1]].symbol == queen)))
				return true;
			else if ((slots[i][place[1]].player == player))
			{
				if (slots[i][place[1]].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
		else
			std::cout << "isFree : " << slots[i][place[1]].symbol << std::endl;
	}
	//(-x) line
	std::cout << "(-x) line" << std::endl;
	for (int j = place[1] - 1; - 1 < j; j--)
	{
		std::cout << "slots[" << place[0] << "][" << j << "] : ";
		if (slots[place[0]][j].isFree == false)
		{
			std::cout << "is NOT Free" << std::endl;
			std::cout << slots[place[0]][j].player << " !players= " << player << std::endl;
			if ((slots[place[0]][j].player != player) &&
				((slots[place[0]][j].symbol == rook) ||
				 (slots[place[0]][j].symbol == queen)))
				return true;
			else if ((slots[place[0]][j].player == player))
			{
				if (slots[place[0]][j].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
		else
			std::cout << "isFree : " << slots[place[0]][j].symbol << std::endl;
	}
	//(-y) line
	std::cout << "(-y) line" << std::endl;
	for (int i = place[0] + 1; i < height; i++)
	{
		std::cout << "slots[" << i << "][" << place[1] << "] : ";
		if (slots[i][place[1]].isFree == false)
		{
			std::cout << "is NOT Free" << std::endl;
			std::cout << slots[i][place[1]].player << " !players= " << player << std::endl;
			if ((slots[i][place[1]].player != player) &&
				((slots[i][place[1]].symbol == rook) ||
				 (slots[i][place[1]].symbol == queen)))
				return true;
			else if ((slots[i][place[1]].player == player))
			{
				if (slots[i][place[1]].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
		else
			std::cout << "isFree : " << slots[i][place[1]].symbol << std::endl;
	}
	////////////*
	//* Diagonal
	////////////*
	std::cout << "is menaced by its diagonals?" << std::endl;
	//pawns
	if ((player == Player::P1) && (place[0] > 0))
	{
		if ((place[1] > 0) && (slots[place[0] - 1][place[1] - 1].symbol == pawn))
			return true;
		else if ((place[1] < width) && (slots[place[0] - 1][place[1] + 1].symbol == pawn))
			return true;
	}
	else if (place[0] < height - 1)
	{
		if ((place[1] > 0) && (slots[place[0] + 1][place[1] - 1].symbol == pawn))
			return true;
		else if ((place[1] < width) && (slots[place[0] + 1][place[1] + 1].symbol == pawn))
			return true;
	}
	//queens or bishops
	int i, j;
	//(+x;+y) diagonal
	i = place[0] - 1;
	j = place[1] + 1;
	for (; (-1 < i) && (j < width); (i--) && (j++))
		if (slots[i][j].isFree == false)
		{
			if ((slots[i][j].player != player) &&
				((slots[i][j].symbol == bishop) ||
				 (slots[i][j].symbol == queen)))
				return true;
			else if ((slots[i][j].player == player))
			{
				if (slots[i][j].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
	//(-x;+y) diagonal
	i = place[0] - 1;
	j = place[1] - 1;
	for (; (-1 < i) && (-1 < j); (i--) && (j--))
		if (slots[i][j].isFree == false)
		{
			if ((slots[i][j].player != player) &&
				((slots[i][j].symbol == bishop) ||
				 (slots[i][j].symbol == queen)))
				return true;
			else if ((slots[i][j].player == player))
			{
				if (slots[i][j].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
	//(-x;-y) diagonal
	i = place[0] + 1;
	j = place[1] - 1;
	for (; (i < height) && (-1 < j); (i++) && (j--))
		if (slots[i][j].isFree == false)
		{
			if ((slots[i][j].player != player) &&
				((slots[i][j].symbol == bishop) ||
				 (slots[i][j].symbol == queen)))
				return true;
			else if ((slots[i][j].player == player))
			{
				if (slots[i][j].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
	//(+x;-y) diagonal
	i = place[0] + 1;
	j = place[1] + 1;
	for (; (i < height) && (j < width); (i++) && (j++))
		if (slots[i][j].isFree == false)
		{
			if ((slots[i][j].player != player) &&
				((slots[i][j].symbol == bishop) ||
				 (slots[i][j].symbol == queen)))
				return true;
			else if ((slots[i][j].player == player))
			{
				if (slots[i][j].symbol == king)
				{
					std::cout << "***KING exception***" << std::endl;
					continue;
				}
				else
					break;
			}
		}
	////////////*
	//* Jumps
	////////////*
	std::cout << "knight menacing?" << std::endl;
	if ((place[0] > 1) && (place[1] > 0) && (slots[place[0] - 2][place[1] - 1].symbol == knight))
		return true;
	else if ((place[0] > 1) && (place[1] < 7) && (slots[place[0] - 2][place[1] + 1].symbol == knight))
		return true;
	else if ((place[0] > 0) && (place[1] > 1) && (slots[place[0] - 1][place[1] - 2].symbol == knight))
		return true;
	else if ((place[0] < 7) && (place[1] > 1) && (slots[place[0] + 1][place[1] - 2].symbol == knight))
		return true;
	else if ((place[0] < 6) && (place[1] > 0) && (slots[place[0] + 2][place[1] - 1].symbol == knight))
		return true;
	else if ((place[0] < 6) && (place[1] < 7) && (slots[place[0] + 2][place[1] + 1].symbol == knight))
		return true;
	else if ((place[0] > 0) && (place[1] < 6) && (slots[place[0] - 1][place[1] + 2].symbol == knight))
		return true;
	else if ((place[0] < 7) && (place[1] < 6) && (slots[place[0] + 1][place[1] + 2].symbol == knight))
		return true;
	std::cout << "<--<--| IS NOT MENACED |-->-->" << std::endl;
	return false;
}

void Gameboard::drawDot(int place[2])
{
	slots[place[0]][place[1]].symbol = PiecesChar::char_dot;
}
void Gameboard::unDrawDot(int place[2])
{
	slots[place[0]][place[1]].symbol = PiecesChar::char_free;
}

bool Gameboard::availableKingMovement(int kingBearings[2])
{
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
	int iLimit = 3;
	// int j = 0;
	int jLimit = 3;
	// if (inTheUpperBorder)
	// 	i = 1;
	if (inTheBottomBorder)
		iLimit = 2;
	if (inTheRightBorder)
		jLimit = 2;
	// if (inTheLeftBorder)
	// 	j = 1;

	for (int i = (inTheUpperBorder) ? 1 : 0; i < iLimit; i++)
	{
		end[0] = kingBearings[0] + (-1 + i);
		for (int j = (inTheLeftBorder) ? 1 : 0; j < jLimit; j++)
		{
			end[1] = kingBearings[1] + (-1 + j);
			slotMenaced = isMenaced(end, slots[kingBearings[0]][kingBearings[1]].player);
			if (slotMenaced == false)
			{
				if (slots[end[0]][end[1]].isFree)
				{
					drawDot(end);
					availableMovement = true;
				}
				else if (slots[end[0]][end[1]].player != player)
				{
					availableMovement = true;
					break;
				}
			}
			else
			{
				std::cout << "<--<--| IS MENACEEEEEEEEEEED |-->-->" << std::endl;
			}
		}
	}
	// if (slots[kingBearings[0]][kingBearings[1]].movements == 0)
	// {
	// 	if (slots[kingBearings[0]][0].movements == 0)
	// 	{
	// 		for (int j = kingBearings[1] - 1; 0 < j; j--)
	// 			if (slots[kingBearings[0]][j].isFree == false)
	// 			{
	// 				availableQueensideCastling = false;
	// 				break;
	// 			}
	// 	}
	// 	if (slots[kingBearings[0]][width - 1].movements == 0)
	// 	{
	// 		for (int j = kingBearings[1] + 1; j < width - 1; j++)
	// 			if (slots[kingBearings[0]][j].isFree == false)
	// 			{
	// 				availableKingsideCastling = false;
	// 				break;
	// 			}
	// 	}
	// }
	// if (availableKingsideCastling)
	// {
	// 	availableMovement = true;
	// 	end[0] = kingBearings[0];
	// 	end[1] = 2;
	// 	drawDot(end);
	// }
	// if (availableQueensideCastling)
	// {
	// 	availableMovement = true;
	// 	end[0] = kingBearings[0];
	// 	end[1] = width - 3;
	// 	drawDot(end);
	// }
	return availableMovement;
}

int Gameboard::validPawnMovement(int start[2], int end[2])
{
	bool valid = false;
	bool player = slots[start[0]][start[1]].player;
	int direction = (player == Player::P1) ? 1 : -1;
	std::cout << "inside Pawn Movement Validator" << std::endl;
	if (slots[start[0]][start[1]].movements == 0)
	{
		std::cout << "first Movement" << std::endl;
		if ((start[0] - end[0]) <= direction * 2)
		{
			if ((start[0] - end[0]) == direction)
			{
				if (abs(start[1] - end[1]) == 1)
				{
					if (slots[end[0]][end[1]].player == !player)
					{
						eat(start, end);
						valid = true;
					}
				}
				else if ((start[1] == end[1]) && (slots[end[0]][end[1]].isFree))
				{
					int aux[2];
					aux[0] = end[0] - 1;
					aux[1] = end[1];
					eat(start, aux);
					move(aux, end);
					valid = true;
				}
			}
			else
			{
				if ((start[1] == end[1]) && (slots[end[0]][end[1]].isFree))
				{
					move(start, end);
					valid = true;
				}
			}
		}
	}
	else
	{
		std::cout << "not first Movement" << std::endl;
		if ((start[0] - end[0]) == direction)
		{
			if (abs(start[1] - end[1]) == 1)
			{
				if (slots[end[0]][end[1]].player == !player)
				{
					eat(start, end);
					valid = true;
				}
			}
			else if ((start[1] == end[1]) && (slots[end[0]][end[1]].isFree))
			{
				move(start, end);
				valid = true;
			}
		}
	}
	return valid;
}

bool Gameboard::validMovement(int start[2], int end[2])
{
	cout << "validMovement analysis" << endl;
	bool player = (slots[start[0]][start[1]].player == Player::P1) ? Player::P1 : Player::P2;
	bool valid = false;
	int points = slots[end[0]][end[1]].points;
	bool wasFree = slots[end[0]][end[1]].isFree;
	Piece auxEndPiece;
	if (!wasFree)
	{
		auxEndPiece.setPiece(slots[end[0]][end[1]]);
		auxEndPiece.movements--;
	}
	switch (slots[start[0]][start[1]].symbol)
	{
	case PiecesChar::charP1_king:
		int distance[2];
		distance[0] = abs(start[0] - end[0]);
		distance[1] = abs(start[1] - end[1]);
		if (((distance[0] == 0) || (distance[0] == 1)) &&
			((distance[1] == 0) || (distance[1] == 1)))
		{
			valid = !isMenaced(end, slots[start[0]][start[1]].player);
			if (valid)
			{
				P1_kingBearings[0] = end[0];
				P1_kingBearings[1] = end[1];
				// P2_kingBearings[2];
				if (slots[end[0]][end[1]].isFree)
					move(start, end);
				else
					eat(start, end);
			}
			else
			{
				std::cout << "<--<--| IS MENACEEEEEEEEEEED |-->-->" << std::endl;
			}
		}
		break;
	case PiecesChar::charP1_queen:
		valid = (goDiagonal(start, end) || goStraight(start, end));
		break;
	case PiecesChar::charP1_rook:
		valid = goStraight(start, end);
		break;
	case PiecesChar::charP1_knight:
		valid = jump(start, end);
		break;
	case PiecesChar::charP1_bishop:
		valid = goDiagonal(start, end);
		break;
	case PiecesChar::charP1_pawn:
		valid = validPawnMovement(start, end);
		break;

	default:
		break;
	}
	if (player == Player::P1)
	{
		if (isMenaced(P1_kingBearings, Player::P1) == true)
		{
			//restore the movement
			if (wasFree)
			{
				move(end, start);
				slots[start[0]][start[1]].movements--;
				slots[end[0]][end[1]].movements--;
				std::cout << "Your king would be in Danger" << std::endl;
			}
			else
			{
				P1_score -= points;
				slots[start[0]][start[1]].movements--;
				move(end, start);
				slots[end[0]][end[1]].setPiece(auxEndPiece);
				slots[end[0]][end[1]].movements--;
				std::cout << "Your king would be in Danger" << std::endl;
			}
			P1_kingBearings[0] = start[0];
			P1_kingBearings[1] = start[1];
			return false;
		}
		else
		{
			std::cout << "<--<--| IS MENACEEEEEEEEEEED |-->-->" << std::endl;
		}
	}
	else
	{
		if (isMenaced(P2_kingBearings, Player::P2) == true)
		{
			if (wasFree)
			{
				move(end, start);
				slots[start[0]][start[1]].movements--;
				slots[end[0]][end[1]].movements--;
				std::cout << "Your king would be in Danger" << std::endl;
			}
			else
			{
				P1_score -= points;
				slots[start[0]][start[1]].movements--;
				move(end, start);
				slots[end[0]][end[1]].setPiece(auxEndPiece);
				slots[end[0]][end[1]].movements--;
				std::cout << "Your king would be in Danger" << std::endl;
			}
			return false;
		}
		else
		{
			std::cout << "<--<--| IS MENACEEEEEEEEEEED |-->-->" << std::endl;
		}
	}
	return valid;
}

void Gameboard::undrawKingDots(int kingBearings[2])
{
	bool inTheUpperBorder = (kingBearings[0] == 0) ? true : false;
	bool inTheBottomBorder = (kingBearings[0] == height - 1) ? true : false;
	bool inTheLeftBorder = (kingBearings[1] == 0) ? true : false;
	bool inTheRightBorder = (kingBearings[1] == width - 1) ? true : false;
	bool availableQueensideCastling = true;
	bool availableKingsideCastling = true;

	// std::cout << "borders" << std::endl;
	// std::cout << inTheUpperBorder << inTheBottomBorder << inTheLeftBorder << inTheRightBorder << std::endl;

	int end[2];
	// int i = 0;
	int iLimit = 3;
	// int j = 0;
	int jLimit = 3;
	// if (inTheUpperBorder)
	// 	i = 1;
	if (inTheBottomBorder)
		iLimit = 2;
	if (inTheRightBorder)
		jLimit = 2;
	// if (inTheLeftBorder)
	// 	j = 1;
	for (int i = (inTheUpperBorder) ? 1 : 0; i < iLimit; i++)
	{
		end[0] = kingBearings[0] + (-1 + i);
		for (int j = (inTheLeftBorder) ? 1 : 0; j < jLimit; j++)
		{
			end[1] = kingBearings[1] + (-1 + j);
			if (slots[end[0]][end[1]].symbol == PiecesChar::char_dot)
				unDrawDot(end);
		}
	}
	// if (slots[kingBearings[0]][kingBearings[1]].movements == 0)
	// {
	// 	if (slots[kingBearings[0]][0].movements == 0)
	// 	{
	// 		for (int j = kingBearings[1] - 1; 0 < j; j--)
	// 			if (slots[kingBearings[0]][j].isFree == false)
	// 			{
	// 				availableQueensideCastling = false;
	// 				break;
	// 			}
	// 	}
	// 	if (slots[kingBearings[0]][width - 1].movements == 0)
	// 	{
	// 		for (int j = kingBearings[1] + 1; j < width - 1; j++)
	// 			if (slots[kingBearings[0]][j].isFree == false)
	// 			{
	// 				availableKingsideCastling = false;
	// 				break;
	// 			}
	// 	}
	// }
	// if (availableKingsideCastling)
	// {
	// 	availableMovement = true;
	// 	end[0] = kingBearings[0];
	// 	end[1] = 2;
	// 	drawDot(end);
	// }
	// if (availableQueensideCastling)
	// {
	// 	availableMovement = true;
	// 	end[0] = kingBearings[0];
	// 	end[1] = width - 3;
	// 	drawDot(end);
	// }
}

bool Gameboard::piecePossibilities(int place[2]) // ONLY humans need visual reference
{
	cout << "piecePossibilities" << endl;
	int piecePossibilities;
	bool availableMovement = false;

	switch (slots[place[0]][place[1]].symbol)
	{
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
	if (availableMovement)
	{
		show();
		switch (piecePossibilities)
		{
		case 0:
			undrawKingDots(place);
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
	}
	else
		return false;
}

void Gameboard::show()
{
	clean_screan();
	// Letters, upper gameframe
	cout << equatorBlank << meridianChar;
	for (int i = 0; i < width; i++)
	{
		slotPiece[slotWidth / 2] = ((char)letter + i);
		cout << slotPiece << meridianChar;
	}
	cout << endl;
	// Gameboard
	for (int i = 0; i < height; i++)
	{
		//first frame
		cout << equatorFrame << meridianChar;
		for (int j = 0; j < width; j++)
		{
			cout << equatorSlot << meridianChar;
		}
		cout << equatorFrame << endl;

		for (int j = 0; j < 2; j++)
		{
			// Left frame (equator)
			if (j == 1)
			{
				cout << blankChar << number + i << blankChar << meridianChar;
			}
			else
			{
				cout << equatorBlank << meridianChar;
			}
			// Slot
			for (int o = 0; o < slotWidth + 1; o++)
			{
				if (j == 1)
				{
					slotPiece[slotWidth / 2] = slots[i][o].symbol;
					cout << slotPiece << meridianChar;
				}
				else
				{
					cout << slotBlank << meridianChar;
				}
			}
			//Right Frame (equator)
			cout << equatorBlank << endl;
		}
	}
	// Letters, bottom gameframe
	cout << equatorFrame << meridianChar;
	for (int j = 0; j < width; j++)
	{
		cout << equatorSlot << meridianChar;
	}
	cout << equatorFrame << endl;
	cout << equatorBlank << meridianChar;
	for (int i = 0; i < width; i++)
	{
		slotPiece[slotWidth / 2] = ((char)letter + i);
		cout << slotPiece << meridianChar;
	}
	cout << endl;
}

void Gameboard::initShowVars()
{
	equatorFrame = new char[equatorFrameWidth];
	equatorSlot = new char[slotWidth];
	equatorBlank = new char[equatorFrameWidth];
	meridianFrame = new char[meridianFrameWidth];
	meridianBlank = new char[meridianFrameWidth];
	slotPiece = new char[slotWidth];
	slotBlank = new char[slotWidth];

	for (int i = 0; i < equatorFrameWidth; i++)
	{
		equatorFrame[i] = equatorChar;
		equatorBlank[i] = blankChar;
	}
	for (int i = 0; i < meridianFrameWidth; i++)
	{
		meridianFrame[i] = meridianChar;
		meridianBlank[i] = blankChar;
	}
	for (int i = 0; i < slotWidth; i++)
	{
		equatorSlot[i] = equatorChar;
		slotBlank[i] = blankChar;
		slotPiece[i] = blankChar;
	}
}

bool Gameboard::drawLines_P(int place[2])
{
	bool player = slots[place[0]][place[1]].player;
	bool availableMovement = false;
	int p_y = place[0] - 1;
	int cas = 0;
	if (place[0] == 6)
	{
		cas = 2;
	}
	else
	{
		cas = 1;
	}
	for (int i = 0; i < cas; i++)
	{
		if (slots[p_y][place[1]].symbol == PiecesChar::char_free)
		{
			slots[p_y][place[1]].symbol = '*';
			availableMovement = true;
			p_y--;
		}
		else if ((slots[p_y][place[1]].isFree == false) && (slots[p_y][place[1]].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	return availableMovement;
}

void Gameboard::undrawLines_P(int place[2])
{
	int p_y = place[0] - 1;
	int cas = 0;
	if (place[0] == 6)
		cas = 2;
	else
		cas = 1;
	for (int j = 0; j < cas; j++)
	{
		if (slots[p_y][place[1]].symbol == '*')
		{
			slots[p_y][place[1]].symbol = PiecesChar::char_free;
			p_y--;
		}
		else
			break;
	}
}

void Gameboard::undrawLines(int place[2])
{
	//(+x) line
	for (int j = place[1] + 1; j < width; j++)
	{
		if (slots[place[0]][j].symbol == '*')
			slots[place[0]][j].symbol = PiecesChar::char_free;
		else
			break;
	}
	//(+y) line
	for (int i = place[0] - 1; - 1 < i; i--)
	{
		if (slots[i][place[1]].symbol == '*')
			slots[i][place[1]].symbol = PiecesChar::char_free;
		else
			break;
	}
	//(-x) line
	for (int j = place[1] - 1; - 1 < j; j--)
	{
		if (slots[place[0]][j].symbol == '*')
			slots[place[0]][j].symbol = PiecesChar::char_free;
		else
			break;
	}
	//(-y) line
	for (int i = place[0] + 1; i < height; i++)
	{
		if (slots[i][place[1]].symbol == '*')
			slots[i][place[1]].symbol = PiecesChar::char_free;
		else
			break;
	}
}

bool Gameboard::drawJumps(int place[2]) //Knight Jumps
{
	bool availableMovement = false;
	if ((place[0] > 1) && (place[1] > 0) && (slots[place[0] - 2][place[1] - 1].symbol == PiecesChar::char_free))
	{
		slots[place[0] - 2][place[1] - 1].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] > 1) && (place[1] < 7) && (slots[place[0] - 2][place[1] + 1].symbol == PiecesChar::char_free))
	{
		slots[place[0] - 2][place[1] + 1].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] > 0) && (place[1] > 1) && (slots[place[0] - 1][place[1] - 2].symbol == PiecesChar::char_free))
	{
		slots[place[0] - 1][place[1] - 2].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] < 7) && (place[1] > 1) && (slots[place[0] + 1][place[1] - 2].symbol == PiecesChar::char_free))
	{
		slots[place[0] + 1][place[1] - 2].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] < 6) && (place[1] > 0) && (slots[place[0] + 2][place[1] - 1].symbol == PiecesChar::char_free))
	{
		slots[place[0] + 2][place[1] - 1].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] < 6) && (place[1] < 7) && (slots[place[0] + 2][place[1] + 1].symbol == PiecesChar::char_free))
	{
		slots[place[0] + 2][place[1] + 1].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] > 0) && (place[1] < 6) && (slots[place[0] - 1][place[1] + 2].symbol == PiecesChar::char_free))
	{
		slots[place[0] - 1][place[1] + 2].symbol = '*';
		availableMovement = true;
	}
	if ((place[0] < 7) && (place[1] < 6) && (slots[place[0] + 1][place[1] + 2].symbol == PiecesChar::char_free))
	{
		slots[place[0] + 1][place[1] + 2].symbol = '*';
		availableMovement = true;
	}
	return availableMovement;
}

void Gameboard::undrawJumps(int place[2])
{
	if ((place[0] > 1) && (place[1] > 0) && (slots[place[0] - 2][place[1] - 1].symbol == '*'))
	{
		slots[place[0] - 2][place[1] - 1].symbol = PiecesChar::char_free;
	}
	if ((place[0] > 1) && (place[1] < 7) && (slots[place[0] - 2][place[1] + 1].symbol == '*'))
	{
		slots[place[0] - 2][place[1] + 1].symbol = PiecesChar::char_free;
	}
	if ((place[0] > 0) && (place[1] > 1) && (slots[place[0] - 1][place[1] - 2].symbol == '*'))
	{
		slots[place[0] - 1][place[1] - 2].symbol = PiecesChar::char_free;
	}
	if ((place[0] < 7) && (place[1] > 1) && (slots[place[0] + 1][place[1] - 2].symbol == '*'))
	{
		slots[place[0] + 1][place[1] - 2].symbol = PiecesChar::char_free;
	}
	if ((place[0] < 6) && (place[1] > 0) && (slots[place[0] + 2][place[1] - 1].symbol == '*'))
	{
		slots[place[0] + 2][place[1] - 1].symbol = PiecesChar::char_free;
	}
	if ((place[0] < 6) && (place[1] < 7) && (slots[place[0] + 2][place[1] + 1].symbol == '*'))
	{
		slots[place[0] + 2][place[1] + 1].symbol = PiecesChar::char_free;
	}
	if ((place[0] > 0) && (place[1] < 6) && (slots[place[0] - 1][place[1] + 2].symbol == '*'))
	{
		slots[place[0] - 1][place[1] + 2].symbol = PiecesChar::char_free;
	}
	if ((place[0] < 7) && (place[1] < 6) && (slots[place[0] + 1][place[1] + 2].symbol == '*'))
	{
		slots[place[0] + 1][place[1] + 2].symbol = PiecesChar::char_free;
	}
}

bool Gameboard::drawLines(int place[2])
{
	bool availableMovement = false;
	bool player = slots[place[0]][place[1]].player;
	//(+x) line
	for (int j = place[1] + 1; j < width; j++)
	{
		if (slots[place[0]][j].symbol == PiecesChar::char_free)
		{
			slots[place[0]][j].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[place[0]][j].isFree == false) && (slots[place[0]][j].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	//(+y) line
	for (int i = place[0] - 1; - 1 < i; i--)
	{
		if (slots[i][place[1]].symbol == PiecesChar::char_free)
		{
			slots[i][place[1]].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[i][place[1]].isFree == false) && (slots[i][place[1]].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	//(-x) line
	for (int j = place[1] - 1; - 1 < j; j--)
	{
		if (slots[place[0]][j].symbol == PiecesChar::char_free)
		{
			slots[place[0]][j].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[place[0]][j].isFree == false) && (slots[place[0]][j].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	//(-y) line
	for (int i = place[0] + 1; i < height; i++)
	{
		if (slots[i][place[1]].symbol == PiecesChar::char_free)
		{
			slots[i][place[1]].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[i][place[1]].isFree == false) && (slots[i][place[1]].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	return availableMovement;
}

void Gameboard::undrawDiagonals(int place[2])
{
	int i, j;
	//(+x;+y) diagonal
	i = place[0] - 1;
	j = place[1] + 1;
	for (; (-1 < i) && (j < width); (i--) && (j++))
	{
		if (slots[i][j].symbol == '*')
		{
			slots[i][j].symbol = PiecesChar::char_free;
		}
		else
			break;
	}
	//(-x;+y) diagonal
	i = place[0] - 1;
	j = place[1] - 1;
	for (; (-1 < i) && (-1 < j); (i--) && (j--))
	{
		if (slots[i][j].symbol == '*')
		{
			slots[i][j].symbol = PiecesChar::char_free;
		}
		else
			break;
	}
	//(-x;-y) diagonal
	i = place[0] + 1;
	j = place[1] - 1;
	for (; (i < height) && (-1 < j); (i++) && (j--))
	{
		if (slots[i][j].symbol == '*')
		{
			slots[i][j].symbol = PiecesChar::char_free;
		}
		else
			break;
	}
	//(+x;-y) diagonal
	i = place[0] + 1;
	j = place[1] + 1;
	for (; (i < height) && (j < width); (i++) && (j++))
	{
		if (slots[i][j].symbol == '*')
		{
			slots[i][j].symbol = PiecesChar::char_free;
		}
		else
			break;
	}
}

bool Gameboard::drawDiagonals(int place[2])
{
	bool availableMovement = false;
	bool player = slots[place[0]][place[1]].player;
	int i, j;

	i = place[0] - 1;
	j = place[1] + 1;
	for (; (-1 < i) && (j < width); (i--) && (j++))
	{
		if (slots[i][j].symbol == PiecesChar::char_free)
		{
			slots[i][j].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[i][j].isFree == false) && (slots[i][j].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	i = place[0] - 1;
	j = place[1] - 1;
	for (; (-1 < i) && (-1 < j); (i--) && (j--))
	{
		if (slots[i][j].symbol == PiecesChar::char_free)
		{
			slots[i][j].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[i][j].isFree == false) && (slots[i][j].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	//(-x;-y) diagonal
	i = place[0] + 1;
	j = place[1] - 1;
	for (; (i < height) && (-1 < j); (i++) && (j--))
	{
		if (slots[i][j].symbol == PiecesChar::char_free)
		{
			slots[i][j].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[i][j].isFree == false) && (slots[i][j].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	//(+x;-y) diagonal
	i = place[0] + 1;
	j = place[1] + 1;
	for (; (i < height) && (j < width); (i++) && (j++))
	{
		if (slots[i][j].symbol == PiecesChar::char_free)
		{
			slots[i][j].symbol = '*';
			availableMovement = true;
		}
		else if ((slots[i][j].isFree == false) && (slots[i][j].player != player))
		{
			availableMovement = true;
			break;
		}
		else
			break;
	}
	return availableMovement;
}

void Gameboard::initGameboard(int P1PiecesInit[][3], int nP1Pieces, int P2PiecesInit[][3], int nP2Pieces)
{
	int points;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			slots[i][j] = Piece(PiecesChar::char_free, 1, 0, 0);
	for (int i = 0; i < nP1Pieces; i++)
	{
		switch (P1PiecesInit[i][2])
		{
		case PiecesChar::charP1_king:
			points = PiecesPoints::kingPoints;
			P1_kingBearings[0] = P1PiecesInit[i][0];
			P1_kingBearings[1] = P1PiecesInit[i][1];
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
		slots[P1PiecesInit[i][0]][P1PiecesInit[i][1]] = Piece((char)P1PiecesInit[i][2], 0, Player::P1, points);
	}
	for (int i = 0; i < nP2Pieces; i++)
	{
		switch (P2PiecesInit[i][2])
		{
		case PiecesChar::charP2_king:
			points = PiecesPoints::kingPoints;
			P2_kingBearings[0] = P2PiecesInit[i][0];
			P2_kingBearings[1] = P2PiecesInit[i][1];
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
		slots[P2PiecesInit[i][0]][P2PiecesInit[i][1]] = Piece((char)P2PiecesInit[i][2], 0, Player::P2, points);
	}
}
