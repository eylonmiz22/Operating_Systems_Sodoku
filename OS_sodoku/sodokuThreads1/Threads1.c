/*
 *  Threads.c
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include "Checks.h"
#include "Threads1.h"

// Global sodoku board and results array
sodokuBoard* sBoard;

int main(int argc, char **argv)
{
	bool rightAns;
	int i, taskNum[N * n], returnValue, fd = 0;
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

	sBoard = (sodokuBoard*) calloc(1, sizeof(sodokuBoard));

	// Convert the board from string to int[81]
	convertStrBoardToIntBoard(boardStr, sBoard->board, STRING_BOARD_SIZE);

	// Create 27 threads for each task
	for (i = 0; i < 27; i++)
	{
		taskNum[i] = i;
		returnValue = pthread_create(&allThreads[i], NULL, threadTask,
				(void*) &taskNum[i]);
		// If return value is not zero there's a problem
		if (returnValue)
		{
			free(sBoard);
			fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	// Wait for the threads to end tasks
	for (i = 0; i < N * n; i++)
	{
		returnValue = pthread_join(allThreads[i], NULL);
		// If return value is not zero there's a problem
		if (returnValue)
		{
			free(sBoard);
			fprintf(stderr, "ERROR in %s() function from \"%s\" file, line %d: %s\n",
					__FUNCTION__, __FILE__, __LINE__, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	rightAns = checkResultsArr(sBoard);

	// free sodokuBoard memory
	free(sBoard);

	// Checks answer
	if (rightAns)
		fprintf(stdout, "%s is legal\n", fileName);
	else
		fprintf(stdout, "%s is not legal\n", fileName);
	return 0;

}

// Do the a thread task on sodoku board and update answer in result array
void* threadTask(void* arg)
{
	int taskNum = *(int*) arg;
	if (checkTask(taskNum, sBoard->board) == true)
	{
		sBoard->result[taskNum] = 1;
	}
	else
	{
		sBoard->result[taskNum] = 0;
	}
	return NULL;
}

bool checkResultsArr(sodokuBoard* sBoard) {// Check results in results array
	int i;
	for (i = 0; i < N * n; i++)
	{
		if (sBoard->result[i] == 0)
		{
			return false;
		}
	}
	return true;
}

