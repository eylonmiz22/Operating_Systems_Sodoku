/*
 * Child.c
 *
 *  Created on: Apr 25, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "Child.h"
#include "src/Checks.h"

int main(int argc, char* argv[])
{
	int mat[N][N];
	readBoardFromFather(mat);
	writeAnsToFather(argv[0][0], mat);
	return 1;
}

void writeAnsToFather(char checkType, int mat[N][N])
{// Child writes the answer for it's check to father
	int ans;
	char c;
	switch (checkType)
	{
		case '1':
			ans = checkRows(mat);
			break;
		case '2':
			ans = checkCols(mat);
			break;
		case '3':
			ans = checkMats(mat);
			break;
	}
	c = ans + '0';
	write(STDOUT_FILENO, &c, sizeof(char));
}

void readBoardFromFather(int board[N][N])
{// Child reads game board from father
	char boardStr[N * N];
	read(STDIN_FILENO, boardStr, N * N);
	strToMatrix(board, boardStr);
}

void strToMatrix(int mat[N][N], char str[])
{// Copies integer string into an integer matrix (N * N size)
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			mat[i][j] = (*(str + i * N + j) - '0');
		}
	}
}
