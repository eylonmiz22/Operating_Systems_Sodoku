/*
 *  funcs.c
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "funcs.h"

void printBoard(int* board)
{
	int i;
	for (i = 0; i < N * N; i++)
	{
		printf("%d ", *(board + i));
		if ((i + 1) % N == 0)
		{
			printf("\n");
		}
	}
}

// read the board as a string from input (stdin if fp == NULL, file otherwise)
void readBoardFromInput(char* boardStr, int fd)
{
	char lineString[31];
	int i, bytesReadNum = 0;
	strcpy(boardStr, "");

	if (!fd)
	{
		printf("Enter sodoku board\n");
	}

	for (i = 0; i < N; i++)
	{
		if (!fd)
		{
			printf("enter %d row:\n", i + 1);
			bytesReadNum = read(STDIN_FILENO, lineString, 30);
			if (bytesReadNum == -1)
			{
				fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
						__FUNCTION__, __FILE__, __LINE__, strerror(errno));
				exit(EXIT_FAILURE);
			}
			lineString[bytesReadNum] = '\0';
		}
		else
		{
			bytesReadNum = read(fd, lineString, 30);
			if (bytesReadNum == -1)
			{
				fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
						__FUNCTION__, __FILE__, __LINE__, strerror(errno));
				exit(EXIT_FAILURE);
			}
			lineString[bytesReadNum] = '\0';
		}
		strcat(boardStr, lineString);
	}
}

// convert string board to int[81] board
void convertStrBoardToIntBoard(char* boardStr, int* board, int size)
{
	int i, counter = 0;
	for (i = 0; i < N * N; i++)
	{
		while (!isdigit(boardStr[counter]) && counter < size)
		counter++;
		if (counter == size)
		{
			fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}
		*(board + i) = boardStr[counter++] - '0';
	}
}
