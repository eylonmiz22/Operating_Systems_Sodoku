/*
 * Child.h
 *
 *  Created on: May 6, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#ifndef CHILD_H_
#define CHILD_H_

#define N 9
#define n 3

void readBoardFromFather(int board[N][N]);
void strToMatrix(int mat[N][N], char str[]);
void writeAnsToFather(char checkType, int mat[N][N]);

#endif /* CHILD_H_ */
