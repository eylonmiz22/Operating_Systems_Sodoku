/*
 * hello.h
 *
 *  Created on: May 10, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262

 */

#ifndef HELLO_H_
#define HELLO_H_

#define N 9
#define n 3

int checkRows(int board[N][N]);
int checkCols(int board[N][N]);
int checkMats(int board[N][N]);
int compareInt(const void* num1, const void* num2);

#endif /* HELLO_H_ */
