/*
 *  Checks.c
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#include "Checks.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

const char DIGITS[] = {'0', '1'};

bool checkTask(int taskNum, int* boardArr)
{// checks the rows or columns or matrices in the board
	int j, boardMat[N][N];
	unsigned int ans = 0;
	int part = taskNum % N;

	boardArrToBoardMat(boardArr, boardMat);

	for (j = 0; j < N; j++)
	{
		if (0 <= taskNum && taskNum <= (N - 1))
		{ // rows
			ans = ans | (1 << boardMat[part][j]);
		}
		else if (N <= taskNum && taskNum <= (N * 2 - 1))
		{ // columns
			ans = ans | (1 << boardMat[j][part]);
		}
		else if (18 <= taskNum && taskNum <= (N * n - 1))
		{ // matrices
			ans = ans
			| (1 << boardMat[(j / n) + (part / n) * n][(j % n) + (part % n) * n]);
		}

	}

	if (ans != 0b1111111110)
	{// Every bit from bit 2, is an answer for a part of the board, we need to check all bits from bit 2
		return false;
	}
	return true;
}

// Convert board from int[81] to int[9][9]
void boardArrToBoardMat(int* board1D, int board2D[9][9])
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			board2D[i][j] = *(board1D + i * N + j);
		}
	}
}

