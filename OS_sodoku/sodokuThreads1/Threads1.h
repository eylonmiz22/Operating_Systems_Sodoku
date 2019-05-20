/*
 *  Threads.h
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#ifndef THREADSOPTION1_H_
#define THREADSOPTION1_H_

typedef struct
{
	int board[81];
	char result[27];
} sodokuBoard;

bool checkResultsArr(sodokuBoard* sBoard);
void* threadTask(void* arg);

#endif
