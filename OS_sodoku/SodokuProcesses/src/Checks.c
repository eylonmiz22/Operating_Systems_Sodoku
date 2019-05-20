/*
 * hello.c
 *
 *  Created on: May 10, 2019
 *	Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#include <stdio.h>
#include <stdlib.h>
#include "Checks.h"

int compareInt(const void* num1, const void* num2)
{
	int n1 = *((const int*) num1);
	int n2 = *((const int*) num2);
	if(n1 < n2) return -1;
	else if(n1 > n2) return 1;
	else return 0;
}

int checkRows(int board[N][N])
{
	int i, j, *found, arr[N];
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			arr[j] = board[i][j];
		}

		qsort(arr, N, sizeof(int), compareInt);

		for(j = 1; j <= N; j++)
		{
			found = (int*) bsearch(&j, arr, N, sizeof(int), compareInt);
			if(!found)
			{
				return 0;
			}
		}
	}
	return 1;
}

int checkCols(int board[N][N])
{
	int i, j, *found, arr[N];
	for(j = 0; j < N; j++)
	{
		for(i = 0; i < N; i++)
		{
			arr[i] = board[i][j];
		}

		qsort(arr, N, sizeof(int), compareInt);

		for(i = 1; i <= N; i++)
		{
			found = (int*) bsearch(&i, arr, N, sizeof(int), compareInt);
			if(!found)
			{
				return 0;
			}
		}
	}
	return 1;
}

int checkMats(int board[N][N])
{
	int i, j, k, t, *found, *arr;

	arr = (int*) malloc(N * sizeof(int));
	if(!arr)
	{
		return 0;
	}

	for(k = 0; k < n; k++)
	{
		for(t = 0; t < n; t++)
		{
			for(i = 0; i < n; i++)
			{
				for(j = 0; j < n; j++)
				{
					arr[i * n + j] = board[n * k + i][n * t + j];
				}
			}

			qsort(arr, N, sizeof(int), compareInt);

			for(j = 1; j < N; j++)
			{
				found = (int*) bsearch(&j, arr, N, sizeof(int), compareInt);
				if(!found)
				{
					free(arr);
					return 0;
				}
			}
		}
	}
	free(arr);
	return 1;
}
