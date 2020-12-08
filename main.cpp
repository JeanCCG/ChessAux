#include <iostream>
#include "Librerias/IA/IA.h"
#include "Librerias/interface/interface.h"
#include <SDL.h>

using namespace std;

OnceAnnouncement title(5);

int game(
	int P1PiecesInit[][3], int nP1Pieces, int P2PiecesInit[][3], int nP2Pieces,
	int width = 8, int height = 8,
	int mode = 0) //game function
{
	cout << "Creating the gameboard." << endl;
	int result = 0;
	int movements = 0;
	int difficulty = 2; //movement projection, predicted or calculation in the future
	bool turn = true;
	bool availableMovement = false;

	//construct/ declaring the pieces
	Gameboard gameboard(P1PiecesInit, nP1Pieces, P2PiecesInit, nP2Pieces, width, height);
	cout << "Gameboard created!" << endl;
	//* GAME

	int start[2];
	int end[2];
	char startLetter;
	char endLetter;
	int startNumber;
	int endNumber;
	char symbol;
	char endSlot;
	bool isFree;
	bool validMovement = false;
	///IA
	char slotes[26][26];

	///
	if (mode == 1)
	{
		while (true)
		{
			// show gameboard
			gameboard.show();
			if (turn) //* P1 turn
			{
				// select a piece
				while (true)
				{
					cout << "Input the piece's letter and number:" << endl;
					cin >> startLetter >> start[0];
					start[0]--;
					startLetter = toupper(startLetter);
					start[1] = (int)startLetter - ((int)'A'); // 0 - width
					cout << start[0] << start[1] << endl;
					if ((-1 < start[1]) && (start[1] < width) && (-1 < start[0]) && (start[0] < height))
					{
						symbol = gameboard.slots[start[0]][start[1]].symbol;
						//symbol = toupper(symbol);
						isFree = gameboard.slots[start[0]][start[1]].isFree;
						cout << symbol << isFree << endl;
						// symbol is a symbol and belongs to P1
						if ((!isFree) && ((int)symbol < ((int)'z' + 1)) && ((int)symbol > ((int)'a' - 1)))
						{
							availableMovement = gameboard.piecePossibilities(start);
							if (availableMovement)
							{
								break;
							}
							else
							{
								cout << "There aren't available positions to move your piece. Please, choose another" << endl;
							}
						}
						else
						{
							cout << "There is no piece in that position." << endl;
						}
					}
					else
						cout << "Imposible position." << endl;
				}
				while (true)
				{
					cout << "Input the end position letter and number:" << endl;
					cin >> endLetter >> end[0];
					end[0]--;
					endLetter = toupper(endLetter);
					end[1] = (int)endLetter - ((int)'A'); // = 0 to width
					if ((-1 < end[1]) && (end[1] < width) && (-1 < end[0]) && (end[0] < height))
					{
						isFree = gameboard.slots[end[0]][end[1]].isFree;
						if (isFree) // endSlot is empty
						{
							gameboard.move(start, end);
							break;
						}
						if (((int)'a' - 1 < endSlot) && (endSlot < (int)'z' + 1))
						{
							switch (gameboard.slots[start[0]][start[1]].symbol)
							{
							case PiecesChar::charP1_king:
								break;
							case PiecesChar::charP1_queen:
								if ((start[0] == end[0]) || (start[1] == end[1]))
									validMovement = gameboard.goStraight(start, end);
								else
									validMovement = gameboard.goDiagonal(start, end);
								break;
							case PiecesChar::charP1_rook:
								validMovement = gameboard.goStraight(start, end);
								break;
							case PiecesChar::charP1_knight:
								validMovement = gameboard.jump(start, end);
								break;
							case PiecesChar::charP1_bishop:
								validMovement = gameboard.goDiagonal(start, end);
								break;
							case PiecesChar::charP1_pawn:
								break;

							default:
								break;
							}
							if (validMovement)
							{
								validMovement = false;
								break;
							}
							else
								cout << "Sorry, that movement is not allowed." << endl;
						}
						else
							cout << "You can't capture your own pieces." << endl;
					}
					else
						cout << "Imposible position." << endl;
				}
				// make the movement in the gameboard
				gameboard.show();
				turn = !turn;
				movements++;
			}
			else //* P2 turn
			{	 /*
			int n;
			cin >> n;
			int example[1];
			int bearingsPoints[n][3];
			int auxBearingsPoints[3];
			int start[2] = {0, 0};
			int maxPoints;
			int count = 0;

			for (int i = 0; i < n; i++)
			{
				cin >> auxBearingsPoints[0] >> auxBearingsPoints[1] >> auxBearingsPoints[2];
				if (auxBearingsPoints[1] > auxBearingsPoints[0])
				{
					continue;
				}
				bearingsPoints[count][0] = auxBearingsPoints[0];
				bearingsPoints[count][1] = auxBearingsPoints[1];
				bearingsPoints[count][2] = auxBearingsPoints[2];
				count++;
			}

			//maxPoints = intelligence(start, bearingsPoints, count, 0, difficulty, 0, true);
			//std::cout << maxPoints << std::endl;

			//* strategic points (ruin the castle movement, position, get more options, get good options, exchange strain?, etc? suggest please), piece points,

			// pass the state of the game
			// set an objective: aperture?, start?. middle?, ends?.
			// judge the options: defense, attack, develop (inrook), exchange chain,
			// make a plan
			// make the first movement plan
			// make the movement in the gameboard*/
				for (int i = 0; i < 8; ++i)
				{
					for (int j = 0; j < 8; ++j)
					{
						slotes[i][j] = gameboard.slots[i][j].symbol;
					}
				}
				for (int i = 0; i < 8; ++i)
				{
					for (int j = 0; j < 8; ++j)
					{
						cout << slotes[i][j] << " ";
					}
					cout << endl;
				}
				int arrr[1000][5];
				int arrrms;
				int numeval=minimax(slotes, 3, true, 0,arrr,arrrms);
				int arrend[2];
                int arrstart[2];
                for (int i = 0; i < 1000; ++i) {
                    if(arrr[i][2]==numeval){
                        arrend[0]=arrr[i][0];
                        arrend[1]=arrr[i][1];
                        arrstart[0]=arrr[i][3];
                        arrstart[1]=arrr[i][4];
                    }
                }
				cout << "\nMinimax : " << numeval << endl;
                cout << "\nMOVE START " << arrstart[0] <<" "<<arrstart[1]<< endl;
                cout << "\nMOVE END " << arrend[0] <<" "<<arrend[1]<< endl;
                if(gameboard.slots[arrend[0]][arrend[1]].isFree){
                    gameboard.move(arrstart, arrend);
                }else{
                    gameboard.eat(arrstart, arrend);
                }
				turn = !turn;
				movements++;
			}
		}
	}
	else if (mode == 2)
	{
		while (true)
		{
			// show gameboard
			gameboard.show();
			if (turn) //* P1 turn
			{
				// select a piece
				while (true)
				{
					cout << "Input the piece's letter and number:" << endl;
					cin >> startLetter >> start[0];
					start[0]--;
					startLetter = toupper(startLetter);
					start[1] = (int)startLetter - ((int)'A'); // 0 - width
					cout << start[0] << start[1] << endl;
					if ((-1 < start[1]) && (start[1] < width) && (-1 < start[0]) && (start[0] < height))
					{
						symbol = gameboard.slots[start[0]][start[1]].symbol;
						//symbol = toupper(symbol);
						isFree = gameboard.slots[start[0]][start[1]].isFree;
						cout << symbol << isFree << endl;
						// symbol is a symbol and belongs to P1
						if ((!isFree) && ((int)symbol < ((int)'z' + 1)) && ((int)symbol > ((int)'a' - 1)))
						{
							availableMovement = gameboard.piecePossibilities(start);
							if (availableMovement)
							{
								break;
							}
							else
							{
								cout << "There aren't available positions to move your piece. Please, choose another" << endl;
							}
						}
						else
						{
							cout << "There is no piece in that position." << endl;
						}
					}
					else
						cout << "Imposible position." << endl;
				}
				while (true)
				{
					cout << "Input the end position letter and number:" << endl;
					cin >> endLetter >> end[0];
					end[0]--;
					endLetter = toupper(endLetter);
					end[1] = (int)endLetter - ((int)'A'); // = 0 to width
					if ((-1 < end[1]) && (end[1] < width) && (-1 < end[0]) && (end[0] < height))
					{
						isFree = gameboard.slots[end[0]][end[1]].isFree;
						if (isFree) // endSlot is empty
						{
							gameboard.move(start, end);
							break;
						}
						if (((int)'a' - 1 < endSlot) && (endSlot < (int)'z' + 1)) // eat P2
						//King Exception
						//system Point
						//eat
						{
							gameboard.eat(start, end);
							break;
						}
						else
							cout << "You can't capture your own pieces." << endl;
					}
					else
						cout << "Imposible position." << endl;
				}
				// make the movement in the gameboard
				gameboard.show();
				turn = !turn;
				movements++;
			}
			else //* P2 turn
			{
				// select a piece
				while (true)
				{
					cout << "Input the piece's letter and number:" << endl;
					cin >> startLetter >> start[0];
					start[0]--;
					startLetter = toupper(startLetter);
					start[1] = (int)startLetter - ((int)'A'); // 0 - width
					cout << start[0] << start[1] << endl;
					if ((-1 < start[1]) && (start[1] < width) && (-1 < start[0]) && (start[0] < height))
					{
						symbol = gameboard.slots[start[0]][start[1]].symbol;
						//symbol = toupper(symbol);
						isFree = gameboard.slots[start[0]][start[1]].isFree;
						cout << symbol << isFree << endl;
						// symbol is a symbol and belongs to P1
						if ((!isFree) && ((int)symbol < ((int)'Z' + 1)) && ((int)symbol > ((int)'A' - 1)))
						{
							availableMovement = gameboard.piecePossibilities(start);
							if (availableMovement)
							{
								break;
							}
							else
							{
								cout << "There aren't available positions to move your piece. Please, choose another" << endl;
							}
						}
						else
						{
							cout << "There is no piece in that position." << endl;
						}
					}
					else
						cout << "Imposible position." << endl;
				}
				while (true)
				{
					cout << "Input the end position letter and number:" << endl;
					cin >> endLetter >> end[0];
					end[0]--;
					endLetter = toupper(endLetter);
					end[1] = (int)endLetter - ((int)'A'); // = 0 to width
					if ((-1 < end[1]) && (end[1] < width) && (-1 < end[0]) && (end[0] < height))
					{
						isFree = gameboard.slots[end[0]][end[1]].isFree;
						if (isFree) // endSlot is empty
						{
							gameboard.move(start, end);
							break;
						}
						if (((int)'A' - 1 < endSlot) && (endSlot < (int)'Z' + 1)) // eat P2
						{
							//King Exception
							//system Point
							//eat
							gameboard.eat(start, end);
							break;
						}
						else
							cout << "You can't capture your own pieces." << endl;
					}
					else
						cout << "Imposible position." << endl;
				}
				// make the movement in the gameboard
				gameboard.show();
				turn = !turn;
				movements++;
			}
		}
	}

	// if (gameboard.P1_kings_ptr[0].checkmate)
	// {
	// 	result = 0;
	// 	// break;
	// }
	// else if (gameboard.P2_kings_ptr[0].checkmate)
	// {
	// 	result = 1;
	// 	// break;
	// }

	return result;
}

int main(int argc, char *args[])
{
	title.init();
	int width = 8;	//ancho
	int height = 8; //altura
	int result;

	int P1PiecesInit[16][3] = {
		{7, 0, (int)PiecesChar::charP1_rook},
		{7, 1, (int)PiecesChar::charP1_knight},
		{7, 2, (int)PiecesChar::charP1_bishop},
		{7, 3, (int)PiecesChar::charP1_queen},
		{7, 4, (int)PiecesChar::charP1_king},
		{7, 5, (int)PiecesChar::charP1_bishop},
		{7, 6, (int)PiecesChar::charP1_knight},
		{7, 7, (int)PiecesChar::charP1_rook},
		{6, 0, (int)PiecesChar::charP1_pawn},
		{6, 1, (int)PiecesChar::charP1_pawn},
		{6, 2, (int)PiecesChar::charP1_pawn},
		{6, 3, (int)PiecesChar::charP1_pawn},
		{6, 4, (int)PiecesChar::charP1_pawn},
		{6, 5, (int)PiecesChar::charP1_pawn},
		{6, 6, (int)PiecesChar::charP1_pawn},
		{6, 7, (int)PiecesChar::charP1_pawn}};
	int P2PiecesInit[16][3] = {
		{0, 0, (int)PiecesChar::charP2_rook},
		{0, 1, (int)PiecesChar::charP2_knight},
		{0, 2, (int)PiecesChar::charP2_bishop},
		{0, 3, (int)PiecesChar::charP2_queen},
		{0, 4, (int)PiecesChar::charP2_king},
		{0, 5, (int)PiecesChar::charP2_bishop},
		{0, 6, (int)PiecesChar::charP2_knight},
		{0, 7, (int)PiecesChar::charP2_rook},
		{1, 0, (int)PiecesChar::charP2_pawn},
		{1, 1, (int)PiecesChar::charP2_pawn},
		{1, 2, (int)PiecesChar::charP2_pawn},
		{1, 3, (int)PiecesChar::charP2_pawn},
		{1, 4, (int)PiecesChar::charP2_pawn},
		{1, 5, (int)PiecesChar::charP2_pawn},
		{1, 6, (int)PiecesChar::charP2_pawn},
		{1, 7, (int)PiecesChar::charP2_pawn}};

	char command;
	int game_mode;
	char settings_command;
	char sub_command;
	bool settings_break = false;
	bool subSettings_break = false;

	//* INTERFACE
	while (true)
	{
		clean_screan();
		cout << "\nWhat do you want to do now?" << endl;
		cout << "\t[P]lay" << endl;
		cout << "\t[Q]uit" << endl;
		cout << "\t[S]ettings (in progress)" << endl;
		cout << endl;
		cin >> command;
		clean_screan();
		command = toupper(command);

		switch (command)
		{
		case 'P':
			cout << "\nChoose a game mode (1 or 2)" << endl;
			cout << "\t1) P1 vs PC" << endl;
			cout << "\t2) P1 vs P2" << endl;
			cin >> game_mode;

			cout << "\nLet's play!" << endl;
			result = game(P1PiecesInit, 16, P2PiecesInit, 16, 8, 8, game_mode);
			if (result == 0)
			{
				cout << " 🥳 P1 won the game!" << endl;
			}
			else if (result == 1)
			{
				cout << " P2 won the game! 🥳" << endl;
			}
			else
			{
				cout << " Tie 🤯 No one won the game" << endl;
			}
			break;

		case 'Q':
			cout << "\nGood bye 😥" << endl;
			return 0;
			break;
		case 'S':
			cout << "\n<<--<---SETTINGS--->-->>" << endl;
			// show settings:
			cout << "Dimensions\t: " << width << height << endl;

			cout << "\n<---ACTUAL GAMEBOARD--->" << endl;
			//print actual gameboard

			while (true)
			{
				cout << "\nWhat do you want to modify?" << endl;
				cout << "\t[B]ack" << endl;
				cout << "\tP[1] settings" << endl;
				cout << "\tP[2] settings" << endl;
				cout << "\t[C]lear the gameboard" << endl;
				cout << "\t[G]ameboard settings" << endl;
				// printf("\t[S]how settings\n");
				printf("\t[S]how Gameboard\n");
				// printf("\t[R]estart settings\n");
				cin >> settings_command;
				cleanScreen(2);

				settings_command = toupper(settings_command);

				switch (settings_command)
				{
				case 'B':
					settings_break = true;
					break;
				case '1':
					// cout << "\nNumber P1's pieces\t: " << sumUp(nP1_pieces, 6) << endl;
					// cout << "\tKing(s)\t: " << nP1_pieces[0] << endl;
					// cout << "\tQueen(s)\t: " << nP1_pieces[1] << endl;
					// cout << "\tRook(s)\t: " << nP1_pieces[2] << endl;
					// cout << "\tKnight(s)\t: " << nP1_pieces[3] << endl;
					// cout << "\tBishop(s)\t: " << nP1_pieces[4] << endl;
					// cout << "\tPawn(s)\t: " << nP1_pieces[5] << endl;

					cout << "\n[B]ack" << endl;
					cout << "[~] Add or remove a piece" << endl;
					cout << "[C]hange a piece position" << endl;
					cout << "[S]how Gameboard" << endl;
					cout << endl;
					cin >> sub_command;
					cleanScreen(2);

					sub_command = toupper(sub_command);

					switch (sub_command)
					{
					case 'B':
						subSettings_break = true;
						int bearingsP1_king[1][2];
						int bearingsP1_queen[1][2];
						int bearingsP1_rook[2][2];
						int bearingsP1_knight[2][2];
						int bearingsP1_bishop[2][2];
						int bearingsP1_pawns[2][2];
						break;
					case '~':
						//print [p]iece
						//another switch? :c
						subSettings_break = true;
						break;
					case 'C':
						subSettings_break = true;
						break;

					default:
						break;
					}
					sub_command = ' ';
					if (subSettings_break)
					{
						subSettings_break = false;
						break;
					}
					break;

				case '2':
					// cout << "\nNumber P2's pieces\t: " << sumUp(nP2_pieces, 6) << endl;
					// cout << "\tKing(s)\t: " << nP2_pieces[0] << endl;
					// cout << "\tQueen(s)\t: " << nP2_pieces[1] << endl;
					// cout << "\tRook(s)\t: " << nP2_pieces[2] << endl;
					// cout << "\tKnight(s)\t: " << nP2_pieces[3] << endl;
					// cout << "\tBishop(s)\t: " << nP2_pieces[4] << endl;
					// cout << "\tPawn(s)\t: " << nP2_pieces[5] << endl;
					int nP2_king[1][2];
					int nP2_queen[1][2];
					int nP2_rook[2][2];
					int nP2_knight[2][2];
					int nP2_bishop[2][2];
					int nP2_pawns[2][2];
					break;
				case 'C':
					break;
				case 'G':
					break;
				case 'S':
					break;

				default:
					break;
				}
				if (settings_break)
				{
					settings_break = false;
					break;
				}
			}
			settings_command = ' ';
			break;
		default:
			cout << "\nNot recognizable command" << endl;
			break;
		}

		command = ' ';
	}
}
