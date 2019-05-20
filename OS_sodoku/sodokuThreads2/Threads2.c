/*
 *  Threads2.c
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "funcs.h"
#include "Threads2.h"
#include "Checks.h"

// Global sodokuTasks, board and boardResult
sodokuTasks* sTasks;

int boardResult;
int board[81];

// Global condition and mutexes
pthread_cond_t cond;
pthread_mutex_t tMutex;
pthread_mutex_t rMutex;

int main(int argc, char **argv)
{
	bool rightAns;
	int i, returnValue, fd = 0, tempCompletedTasksNum = 0;
	char boardStr[STRING_BOARD_SIZE];
	char* fileName = "matrix";
	pthread_t allThreads[N * n];

	// Checks if we don't have board file as argument- we will read from the input
	if (argc == 1)
	{
		readBoardFromInput(boardStr, 0);
	}
	else
	{// Read board from file
		fileName = argv[1];
		fd = open(fileName, O_RDONLY);
		if (fd == -1)
		{
			fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}

		readBoardFromInput(boardStr, fd);
		close(fd);
	}

	sTasks = (sodokuTasks*) calloc(1, sizeof(sodokuTasks));
	boardResult = 1;
	sTasks->completedTasksNum = 0;
	sTasks->currentMission = 0;
	for (i = 0; i < N * n; i++)
	{
		sTasks->missions[i] = i;
	}

	// Convert the board from string to int[81]
	convertStrBoardToIntBoard(boardStr, board, STRING_BOARD_SIZE);

	// Initializing
	pthread_mutex_init(&tMutex, NULL);
	pthread_mutex_init(&rMutex, NULL);
	pthread_cond_init(&cond, NULL);

	// Lock mutex before creating threads
	pthread_mutex_lock(&tMutex);

	// Create 27 threads, one for each task
	for (i = 0; i < N; i++)
	{
		returnValue = pthread_create(&allThreads[i], NULL, threadTask, (void*) NULL);

		// If return value is not zero there's a problem
		if (returnValue)
		{
			free(sTasks);
			fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}

	}

	// Wait until all tasks completed
	while (tempCompletedTasksNum < 27)
	{
		pthread_cond_wait(&cond, &tMutex);
		tempCompletedTasksNum = sTasks->completedTasksNum;
	}
	// unlock mutex
	pthread_mutex_unlock(&tMutex);

	// check result
	if (boardResult == 0)
	{
		rightAns = false;
	}

	// free sudokuBoard memory
	free(sTasks);

	// print if the board is legal or not
	if (rightAns)
		fprintf(stdout, "%s is legal\n", fileName);
	else
		fprintf(stdout, "%s is not legal\n", fileName);

	// Wait for all threads to end their mission
	for (i = 0; i < N; i++)
	{
		returnValue = pthread_join(allThreads[i], NULL);
		// If return value is not zero there's a problem
		if (returnValue)
		{
			free(sTasks);
			fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	// destroy mutexes and cond
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&tMutex);
	pthread_mutex_destroy(&rMutex);

	return 0;
}

// Do the a thread task on sodoku board and update answer in result array
void* threadTask(void* arg)
{
	bool taskResult = false;
	int taskNum = 0;

	while (true)
	{
		pthread_mutex_lock(&tMutex);
		if (sTasks->currentMission >= N * n)
		{
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&tMutex);
			break;
		}
		// Gets task number
		taskNum = sTasks->missions[sTasks->currentMission++];
		pthread_mutex_unlock(&tMutex);

		// Do the task and get the result
		taskResult = checkTask(taskNum, board);

		// Updates result of board if needed
		if (boardResult == true && taskResult == false)
		{
			pthread_mutex_lock(&rMutex);
			boardResult = false;
			pthread_mutex_unlock(&rMutex);
		}

		// Signal cond when ending task
		pthread_mutex_lock(&tMutex);
		sTasks->completedTasksNum++;
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&tMutex);
	}
	return NULL;
}

