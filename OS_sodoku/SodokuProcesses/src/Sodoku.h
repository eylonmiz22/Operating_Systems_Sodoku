/*
 * Sodoku.h
 *
 *  Created on: Apr 7, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#ifndef SODOKU_H_
#define SODOKU_H_

#define N 9
#define n 3

typedef struct
{
	int isChildRunning;
	int solution[N][N];
	int status[n];
} Shared;

void setChildToExec(int pipeAns[], int pipeChild[], char* child[], char childNum[]);
void writeBoardToChildren(int pipeC1[], int pipeC2[], int pipeC3[], char boardStr[]);
void fileToStr(char* argv[], char str[], int index);
void createAllPipes(int p1[], int p2[], int p3[], int p4[]);
void closeAllPipes(int p1[], int p2[], int p3[], int p4[]);
void filesToMat(int boards[][N][N], int argc, char* argv[]);
void printBoards(int boards[][N][N], int size);
void cloneMatToSharedStruct(int mat[][N], Shared* shareZone);
void printMat(char mat[N][N]);
void stdinToMatrix(char mat[N][N]);
void childCheckToSharedZone(Shared *sharedBoard, int index, int(*check)(int mat[N][N]));
void fatherCheckPipe(int pipeAns[], char* arg);
void fatherCheckSharedZone(int status[], char* arg);

#endif /* SODOKU_H_ */
