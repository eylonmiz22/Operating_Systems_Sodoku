/*
 *  Threads2.h
 *
 *  Created on: May 11, 2019
 *      Author: Eylon Mizrahi - 206125411, Daniel Ivkovich - 316421262
 */

#ifndef THREADSOPTION2_H_
#define THREADSOPTION2_H_

typedef struct
{
	int missions[27];
	int completedTasksNum;
	int currentMission;
} sodokuTasks;

#define M 10

bool checkResultsArr(sodokuTasks* sTask);
void* threadTask(void* arg);

#endif
