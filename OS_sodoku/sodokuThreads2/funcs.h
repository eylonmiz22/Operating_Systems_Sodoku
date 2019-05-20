/*
 *  funcs.h
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#ifndef UTILS_H_
#define UTILS_H_
#define N 9
#define n 3
#define STRING_BOARD_SIZE 300

typedef enum
{
	false, true
} bool;

void readBoardFromInput(char* boardStr, int fd);
void convertStrBoardToIntBoard(char* boardStr, int* board, int size);
void printBoard(int* board);

#endif
