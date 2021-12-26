#pragma once
#include "resources.h"
#include "Client.h"
#include "Worker.h"
class Manager
{
public:
	Manager(const char* serverName_, const char* Port_);
	BOOL Start();
private:
	std::queue<Task> m_queue;
	std::unique_ptr<Client> client;
	std::string m_currentString, m_currentAnswer;

	Worker worker;
	VOID AddTask(std::queue<Task> &queue, std::string &buf);

	VOID GetInfo(std::string& buf);
};

