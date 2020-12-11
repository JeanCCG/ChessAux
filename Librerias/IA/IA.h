#pragma once

#include "../gameboard/gameboard.h"

using namespace std;


void copy(char [26][26], char [26][26]);

int max(int, int);

int min(int, int);

int p2validpieces(char);

int p1validpieces(char);

int minimax(char [26][26], int, bool, int, int [1000][5], int &);

