#pragma once
#include "resources.h"
class Worker
{
public:
	Worker();
	friend DWORD WINAPI WorkerThread(LPVOID lpParam);
	BOOL SetCurrentTask(Task curTask);
	BOOL StartTask();
private:
	Task currentTask;
};

