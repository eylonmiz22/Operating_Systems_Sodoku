/*
 *  Checks.h
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#ifndef CHECKSUDOKUPARTS_H_
#define CHECKSUDOKUPARTS_H_

#include "funcs.h"

bool checkTask(int taskNum, int* boardArr);
void boardArrToBoardMat(int* board1D, int board2D[9][9]);

#endif
