#include <iostream>
#include "Librerias/gameboard/gameboard.h"
#include "Librerias/interface/interface.h"
#include "Librerias/IA/IA.h"

using namespace std;

OnceAnnouncement title(5);

int game(
	int P1PiecesInit[][3], int nP1Pieces, int P2PiecesInit[][3], int nP2Pieces,
	int width = 8, int height = 8,
	int mode = 0) //game function
{

	std::cout << "Creating the gameboard." << std::endl;
	int result = 0;
	int movements = 0;
	int difficulty = 2; //movement projection, predicted or calculation in the future
	bool turn = true;
	bool availableMovement = false;
    int start_aux[2];
    int end_aux[2];
	// construct/ declaring the pieces
	Gameboard gameboard(P1PiecesInit, nP1Pieces, P2PiecesInit, nP2Pieces, width, height);

	std::cout << "Gameboard created!" << std::endl;
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
	bool player;
	///IA
	char slotes[8][8];

	if (mode == 1) //* |-->->> P1 vs BOT <<-<--|
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
                    cout<< "before Pieza clave: "<<gameboard.slots[3][0].symbol<<endl;
					std::cout << "Input the piece's letter and number:" << endl;
					std::cin >> startLetter >> start[0]; // a <= startLetter <= z  && 1 <= start[0] <= 8
					start[0]--;							 // 0 <= end[0] <= 7
					startLetter = toupper(startLetter);
					start[1] = (int)startLetter - ((int)'A');	 // 0 - width
					if ((-1 < start[1]) && (start[1] < width) && // 0 <= start[1] <= width
						(-1 < start[0]) && (start[0] < height))	 // 0 <= start[0] <= height
					{
						isFree = gameboard.slots[start[0]][start[1]].isFree;
						player = gameboard.slots[start[0]][start[1]].player;
						if ((!isFree) && (player == Player::P1))
						{
							availableMovement = gameboard.piecePossibilities(start);
                            cout<< "avalablemovement Pieza clave: "<<gameboard.slots[3][0].symbol<<endl;
							if (availableMovement)
							{
								// if (P1_isChecked)
								// {
								// 	// what if I choose the king?
								// 	// if(gameboard.slots[start[0]][start[1]].symbol == PiecesChar::charP1_king)
								// 	// {
								// 	// }
								// 	//can this piece block or eat the menacing piece?
								// }
								// else
								break;
							}
							else
								std::cout << "There aren't available positions to move your piece. Please, choose another" << std::endl;
						}
						else
							std::cout << "There is no piece in that position." << std::endl;
					}
					else
						std::cout << "Imposible position." << std::endl;
				}
                cout<< "after selected position Pieza clave: "<<gameboard.slots[3][0].symbol<<endl;
				while (true)
				{
					std::cout << "Input the end position letter and number:" << std::endl;
					std::cin >> endLetter >> end[0]; // a <= endLetter <= z  && 1 <= end[0] <= 8
					end[0]--;						 // 0 <= end[0] <= 7
					endLetter = toupper(endLetter);
					end[1] = (int)endLetter - ((int)'A');	 // 0 - width
					if ((-1 < end[1]) && (end[1] < width) && // 0 <= start[1] <= width
						(-1 < end[0]) && (end[0] < height))	 // 0 <= start[0] <= height
					{
						if ((gameboard.slots[end[0]][end[1]].isFree) ||
							(gameboard.slots[end[0]][end[1]].player != Player::P1))
						{
                            cout<< "before valid end position Pieza clave: "<<gameboard.slots[3][0].symbol<<endl;
							validMovement = gameboard.validMovement(start, end);
                            cout<< "after valid end position Pieza clave: "<<gameboard.slots[3][0].symbol<<endl;
							if (validMovement)
							{
								// if (P1_isChecked)
								// {
								// 	if (gameboard.isMenaced(P1_kingBearings, Player::P1) == false)
								// 	{
								// 		std::cout << "Valid Movement & king is not checked any more" << std::endl;
								// 		P1_isChecked = false;
								// 		break;
								// 	}
								// 	else
								// 	{
								// 		gameboard.move(end, start);
								// 	}
								// }
								// else
								// {
								std::cout << "Valid Movement" << std::endl;
								validMovement = false;
								break;
								// }
							}
							else
								std::cout << "Sorry, that movement is not allowed." << std::endl;
						}
						else
							std::cout << "You can't capture your own pieces." << std::endl;
					}
					else
						std::cout << "Imposible position." << std::endl;
				}
				// make the movement in the gameboard
				gameboard.show();
				turn = !turn;
				movements++;
                 
			}
			else //* BOT turn
			{
                 
				for (int i = 0; i < 8; ++i)
				{
					for (int j = 0; j < 8; ++j)
					{
						slotes[i][j] = gameboard.slots[i][j].symbol;
					}
				}

				int arrr[1000][5];
				int arrrms;
				int numeval = minimax(slotes, Player::Difficulty, true, 0, arrr, arrrms);

				for (int i = 0; i < arrrms; ++i)
				{
					if (arrr[i][2] == numeval)
					{
						end[0] = arrr[i][0];
						end[1] = arrr[i][1];
						start[0] = arrr[i][3];
						start[1] = arrr[i][4];
					}
				}
                /*if(movements==1){
                    start[0] = 1;
                    start[1] = 4;
                    end[0] = 3;
                    end[1] = 4;
                }*/
				std::cout << "nMinimax : " << numeval << std::endl;
				std::cout << "nMOVE START " << start[0] << " " << start[1] << " piece: " << gameboard.slots[start[0]][start[1]].symbol << std::endl;
				std::cout << "nMOVE END " << end[0] << " " << end[1] << " piece: " << gameboard.slots[end[0]][end[1]].symbol << std::endl;

				if (numeval < (-90))
				{
					result = 0;
					gameboard.show();
					break;
				}

				if (gameboard.slots[end[0]][end[1]].isFree)
				{
					gameboard.move(start, end);
                    if((gameboard.slots[start[0]][start[1]].symbol==PiecesChar::charP2_king)&&(end[0]==0)&&(end[1]==6)){
                        start_aux[0]=0;
                        start_aux[1]=7;
                        end_aux[0]=0;
                        end_aux[1]=5;
                        gameboard.move(start_aux,end_aux);
                    }
                    if((gameboard.slots[start[0]][start[1]].symbol==PiecesChar::charP2_king)&&(end[0]==0)&&(end[1]==2)){
                        start_aux[0]=0;
                        start_aux[1]=0;
                        end_aux[0]=0;
                        end_aux[1]=3;
                        gameboard.move(start_aux,end_aux);
                    }
				}
				else
				{
					gameboard.eat(start, end);
				}
				if (numeval > 90)
				{
					result = 1;
					gameboard.show();
					break;
				}
				turn = !turn;
				movements++;
                 
			}
		}
	}
	else if (mode == 2) //* |-->->> P1 vs P2 <<-<--| human vs human |
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
					std::cout << "Input the piece's letter and number:" << std::endl;
					std::cin >> startLetter >> start[0];
					start[0]--;
					startLetter = toupper(startLetter);
					start[1] = (int)startLetter - ((int)'A'); // 0 - width
					std::cout << start[0] << start[1] << std::endl;
					if ((-1 < start[1]) && (start[1] < width) && (-1 < start[0]) && (start[0] < height))
					{
						symbol = gameboard.slots[start[0]][start[1]].symbol;
						//symbol = toupper(symbol);
						isFree = gameboard.slots[start[0]][start[1]].isFree;
						std::cout << symbol << isFree << std::endl;
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
								std::cout << "There aren't available positions to move your piece. Please, choose another" << std::endl;
							}
						}
						else
						{
							std::cout << "There is no piece in that position." << std::endl;
						}
					}
					else
						std::cout << "Imposible position." << std::endl;
				}
				while (true)
				{
					std::cout << "Input the end position letter and number:" << std::endl;
					std::cin >> endLetter >> end[0];
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
							std::cout << "You can't capture your own pieces." << std::endl;
					}
					else
						std::cout << "Imposible position." << std::endl;
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
					std::cout << "Input the piece's letter and number:" << std::endl;
					std::cin >> startLetter >> start[0];
					start[0]--;
					startLetter = toupper(startLetter);
					start[1] = (int)startLetter - ((int)'A'); // 0 - width
					std::cout << start[0] << start[1] << std::endl;
					if ((-1 < start[1]) && (start[1] < width) && (-1 < start[0]) && (start[0] < height))
					{
						symbol = gameboard.slots[start[0]][start[1]].symbol;
						//symbol = toupper(symbol);
						isFree = gameboard.slots[start[0]][start[1]].isFree;
						std::cout << symbol << isFree << std::endl;
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
								std::cout << "There aren't available positions to move your piece. Please, choose another" << std::endl;
							}
						}
						else
						{
							std::cout << "There is no piece in that position." << std::endl;
						}
					}
					else
						std::cout << "Imposible position." << std::endl;
				}
				while (true)
				{
					std::cout << "Input the end position letter and number:" << std::endl;
					std::cin >> endLetter >> end[0];
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
							std::cout << "You can't capture your own pieces." << std::endl;
					}
					else
						std::cout << "Imposible position." << std::endl;
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

int main()
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
   /* int P1PiecesInit[16][3] = {
            {3, 1, (int)PiecesChar::charP1_rook},
            {4, 2, (int)PiecesChar::charP1_queen},
            {4, 1, (int)PiecesChar::charP1_king},
    };
    int P2PiecesInit[16][3] = {
            {5, 7, (int)PiecesChar::charP2_rook},
            {5, 6, (int)PiecesChar::charP2_queen},
            {7, 4, (int)PiecesChar::charP2_king},
    };*/


	//* INTERFACE
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
		std::cout << "nWhat do you want to do now?" << std::endl;
		std::cout << "t[P]lay" << std::endl;
		std::cout << "t[Q]uit" << std::endl;
		std::cout << "t[S]ettings (in progress)" << std::endl;
		std::cout << std::endl;
		std::cin >> command;
		clean_screan();
		command = toupper(command);

		switch (command)
		{
		case 'P':
			std::cout << "nChoose a game mode (1 or 2)" << std::endl;
			std::cout << "t1) P1 vs PC" << std::endl;
			std::cout << "t2) P1 vs P2" << std::endl;
			std::cin >> game_mode;

			std::cout << "nLet's play!" << std::endl;
			result = game(P1PiecesInit, 16, P2PiecesInit, 16, 8, 8, game_mode);
			if (result == 0)
			{
				std::cout << " 🥳 P1 won the game!" << std::endl;
			}
			else if (result == 1)
			{
				std::cout << " P2 won the game! 🥳" << std::endl;
			}
			else
			{
				std::cout << " Tie 🤯 No one won the game" << std::endl;
			}
			break;

		case 'Q':
			std::cout << "nGood bye 😥" << std::endl;
			return 0;
			break;
		case 'S':
			std::cout << "n<<--<---SETTINGS--->-->>" << std::endl;
			// show settings:
			std::cout << "Dimensionst: " << width << height << std::endl;

			std::cout << "n<---ACTUAL GAMEBOARD--->" << std::endl;
			//print actual gameboard

			while (true)
			{
				std::cout << "nWhat do you want to modify?" << std::endl;
				std::cout << "t[B]ack" << std::endl;
				std::cout << "tP[1] settings" << std::endl;
				std::cout << "tP[2] settings" << std::endl;
				std::cout << "t[C]lear the gameboard" << std::endl;
				std::cout << "t[G]ameboard settings" << std::endl;
				// printf("t[S]how settingsn");
				printf("t[S]how Gameboardn");
				// printf("t[R]estart settingsn");
				std::cin >> settings_command;
				cleanScreen(2);

				settings_command = toupper(settings_command);

				switch (settings_command)
				{
				case 'B':
					settings_break = true;
					break;
				case '1':
					// std::cout << "nNumber P1's piecest: " << sumUp(nP1_pieces, 6) << std::endl;
					// std::cout << "tKing(s)t: " << nP1_pieces[0] << std::endl;
					// std::cout << "tQueen(s)t: " << nP1_pieces[1] << std::endl;
					// std::cout << "tRook(s)t: " << nP1_pieces[2] << std::endl;
					// std::cout << "tKnight(s)t: " << nP1_pieces[3] << std::endl;
					// std::cout << "tBishop(s)t: " << nP1_pieces[4] << std::endl;
					// std::cout << "tPawn(s)t: " << nP1_pieces[5] << std::endl;
					std::cout << std::endl;
					std::cout << "[B]ack" << std::endl;
					std::cout << "[~] Add or remove a piece" << std::endl;
					std::cout << "[C]hange a piece position" << std::endl;
					std::cout << "[S]how Gameboard" << std::endl;
					std::cout << std::endl;
					std::cin >> sub_command;
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
					// std::cout << "nNumber P2's piecest: " << sumUp(nP2_pieces, 6) << std::endl;
					// std::cout << "tKing(s)t: " << nP2_pieces[0] << std::endl;
					// std::cout << "tQueen(s)t: " << nP2_pieces[1] << std::endl;
					// std::cout << "tRook(s)t: " << nP2_pieces[2] << std::endl;
					// std::cout << "tKnight(s)t: " << nP2_pieces[3] << std::endl;
					// std::cout << "tBishop(s)t: " << nP2_pieces[4] << std::endl;
					// std::cout << "tPawn(s)t: " << nP2_pieces[5] << std::endl;
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
			std::cout << "nNot recognizable command" << std::endl;
			break;
		}

		command = ' ';
	}
}
