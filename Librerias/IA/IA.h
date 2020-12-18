#pragma once

#include "../gameboard/gameboard.h"

using namespace std;

void copy(char [8][8], char [8][8]);

int max(int ,int );
int min(int ,int );
int p2validpieces(char );
int p1validpieces(char );
int minimax(char [8][8], int , bool , int ,int [5]);