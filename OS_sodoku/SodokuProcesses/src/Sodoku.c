/*
 ============================================================================
 Name        : Sodoku.c
 Author      : Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 Version     :
 Copyright   :
 Description : Sodoku
 ============================================================================
 */

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "Sodoku.h"
#include "Checks.h"

#define DEBUG_EXE_2

#ifdef DEBUG_EXE_1
	#define EXERCISE 1
#else
	#ifdef DEBUG_EXE_2
		#define EXERCISE 2
	#endif
#endif

int main(int argc, char* argv[])
{
	int k;
	pid_t pid1, pid2, pid3;
	int boards[argc - 1][N][N];
	printf("Q%d----------------------------------------------\n", EXERCISE);
	filesToMat(boards, argc, argv);

	if(EXERCISE == 1)
	{
		char* child[3];
		char boardStr[N * N];
		int pipeC1[2], pipeC2[2], pipeC3[2], pipeAns[2]; // Arrays to the pipes for communication with each child and array to the pipe that responsible for the answer.
		createAllPipes(pipeC1, pipeC2, pipeC3, pipeAns);

		for(k = 0; k < argc - 1; k++)
		{
			fileToStr(argv, boardStr, k + 1);
			writeBoardToChildren(pipeC1, pipeC2, pipeC3, boardStr);
			pid1 = fork();
			if(pid1 == 0)
			{// Child1 process
				setChildToExec(pipeAns, pipeC1, child, "1");
				// checked
				if(execvp(child[0], child + 1) == -1)
				{// Exec failed
					printf("exec1 failed");
					exit(EXIT_FAILURE);
				}
			}
			else if(pid1 < 0)
			{// Fork failed
				printf("fork1 failed");
				exit(EXIT_FAILURE);
			}
			else
			{// Father process
				pid2 = fork();
				if(pid2 == 0)
				{// Child2 process
					setChildToExec(pipeAns, pipeC2, child, "2");
					if(execvp(child[0], child + 1) == -1)
					{// Exec failed
						printf("exec2 failed");
						exit(EXIT_FAILURE);
					}
				}
				else if(pid2 < 0)
				{// Fork failed
					printf("fork2 failed");
					exit(EXIT_FAILURE);
				}
				else
				{// Father process
					pid3 = fork();
					if(pid3 == 0)
					{// Child3 process
						setChildToExec(pipeAns, pipeC3, child, "3");
						if(execvp(child[0], child + 1) == -1)
						{// Exec failed
							printf("exec3 failed");
							exit(EXIT_FAILURE);
						}
					}
					else if(pid3 < 0)
					{// Fork failed
						printf("fork3 failed");
						exit(EXIT_FAILURE);
					}
					else
					{// Father process
						fatherCheckPipe(pipeAns, argv[k + 1]);
					}
				}
			}
		}
		closeAllPipes(pipeC1, pipeC2, pipeC3, pipeAns);
	}
	else if(EXERCISE == 2)
	{
		Shared* sharedBoard = (Shared*) malloc(sizeof(Shared));
		sharedBoard = mmap(NULL, sizeof(Shared*), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
		for(k = 0; k < argc - 1; k++)
		{
			sharedBoard->isChildRunning = n;
			cloneMatToSharedStruct(boards[k], sharedBoard);
			sharedBoard->status[0] = 0, sharedBoard->status[1] = 0, sharedBoard->status[2] = 0;

			pid1 = fork();
			if(pid1 == 0)
			{// Child1 process
				childCheckToSharedZone(sharedBoard, 0, checkRows);
			}
			else if(pid1 < 0)
			{// Fork failed
				printf("fork1 failed");
				exit(EXIT_FAILURE);
			}
			else
			{// Father process
				pid2 = fork();
				if(pid2 == 0)
				{// Child2 process
					childCheckToSharedZone(sharedBoard, 1, checkCols);
				}
				else if(pid2 < 0)
				{// Fork failed
					printf("fork2 failed");
					exit(EXIT_FAILURE);
				}
				else
				{// Father process
					pid3 = fork();
					if(pid3 == 0)
					{// Child3 process
						childCheckToSharedZone(sharedBoard, 2, checkMats);
					}
					else if(pid3 < 0)
					{// Fork failed
						printf("fork3 failed");
						exit(EXIT_FAILURE);
					}
					else
					{// Father process
						while(sharedBoard->isChildRunning > 0) {}
						fatherCheckSharedZone(sharedBoard->status, argv[k + 1]);
					}
				}
			}
		}
		return 0;
	}
}

void fileToStr(char* argv[], char str[], int index)
{
	int i = 0;
	char c;
	FILE* file;
	file = fopen(argv[index], "r");
	while(!feof(file))
	{
		c = fgetc(file);
		if(c == ' ' || c == '\n')
		{
			continue;
		}
		str[i++] = c;
	}
}

void filesToMat(int boards[][N][N], int argc, char* argv[])
{
	int k, i, j;
	char dig;
	FILE* file;
	for(k = 0; k < argc - 1; k++)
	{
		file = fopen(argv[k + 1], "r");
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N && feof(file) == 0; j++)
			{
				fscanf(file, "%c ", &dig);
				boards[k][i][j] = dig - '0';
			}
		}
	}
	fclose(file);
}

void printBoards(int boards[][N][N], int size)
{
	int i, j, k;
	for(k = 0; k < size; k++)
	{
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				printf("%d ", boards[k][i][j]);
			}
			printf("\n");
		}
		printf("\n");
		printf("\n");
	}
}

void closeAllPipes(int p1[], int p2[], int p3[], int p4[])
{
	if(close(p1[0]) == -1 || close(p2[0]) == -1  || close(p1[1]) == -1 || close(p2[1]) == -1 ||
	   close(p3[0]) == -1 || close(p4[0]) == -1  || close(p3[1]) == -1 || close(p4[1]) == -1)
	{
		printf("pipe close failed");
		exit(EXIT_FAILURE);
	}
}

void createAllPipes(int p1[], int p2[], int p3[], int p4[])
{
	if(pipe(p1) == -1 || pipe(p2) == -1 || pipe(p3) == -1 || pipe(p4) == -1)
	{
		printf("pipe open failed");
		exit(EXIT_FAILURE);
	}
}

void setChildToExec(int pipeAns[], int pipeChild[], char* child[], char childNum[])
{
	dup2(pipeChild[0], STDIN_FILENO);
	dup2(pipeAns[1], STDOUT_FILENO);

	child[0] = "./child";
	child[1] = childNum;
	child[2] = NULL;
}

void writeBoardToChildren(int pipeC1[], int pipeC2[], int pipeC3[], char boardStr[])
{// Father writes game board to children
	write(pipeC1[1], boardStr, N * N);
	write(pipeC2[1], boardStr, N * N);
	write(pipeC3[1], boardStr, N * N);
}

void printMat(char mat[N][N])
{
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			printf("%c ", mat[i][j]);
		}
	}
}

void stdinToMatrix(char mat[N][N])
{
	char c = '\0';
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			scanf("%c ", &c);
			mat[i][j] = c;
		}
	}
}

void cloneMatToSharedStruct(int mat[][N], Shared* shareZone)
{// Copies one integer matrix to another in shared memory structure
	int i, j;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			shareZone->solution[i][j] = mat[i][j];
		}
	}
}

void childCheckToSharedZone(Shared *sharedBoard, int index, int(*check)(int mat[N][N]))
{
	sharedBoard->status[index] = check(sharedBoard->solution);
	sharedBoard->isChildRunning--;
	exit(0);
}

void fatherCheckPipe(int pipeAns[], char* arg)
{
	int i, isChildRunning = n;
	char c = '9', ans[n];
	while(isChildRunning > 0)
	{
		if(c == '1' || c == '0')
		{
			ans[n - isChildRunning] = c;
			isChildRunning--;
			c = '9';
		}
		else if(read(pipeAns[0], &c, sizeof(char)) == 0) // EOF
		{
			c = '9';
		}
	}

	for(i = 0; i < n; i++)
	{
		if(ans[i] == '0')
		{
			printf("%s is not legal\n", arg);
			break;
		}
	}
	if(i == n)
	{
		printf("%s is legal\n", arg);
	}
}

void fatherCheckSharedZone(int status[], char* arg)
{
	int i;
	for(i = 0; i < n; i++)
	{
		if(!status[i])
		{
			printf("%s is not legal\n", arg);
			break;
		}
	}

	if(i == n)
	{
		printf("%s is legal\n", arg);
	}
}
