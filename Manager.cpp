#include "Manager.h"

Manager::Manager(const char* serverName_, const char* Port_)
{
	client = std::make_unique<Client>(serverName_,Port_);
}

BOOL Manager::Start()
{
	client->ConnectToServer();
	DWORD dwEvent;
	HANDLE events[] = { client->connected,client->readyRead,client->closed };
	//for (auto &ev : events) {
	//	ev = CreateEventA(NULL,FALSE,FALSE,NULL);
	//}
	//events[1] = CreateEventA(NULL, TRUE, FALSE, NULL);
	std::cout << "Start()\n";
	for (;;) {
		m_currentAnswer = "main thread\n";
		client->Send(m_currentAnswer);
		dwEvent = WaitForMultipleObjects(3,events,FALSE,3000);
		switch (dwEvent) {
		case WAIT_OBJECT_0+0://connected
			std::cout << "connect...\n";
			ResetEvent(client->connected);
			GetInfo(m_currentAnswer);
			client->Send(m_currentAnswer);
			
			break;

		case WAIT_OBJECT_0+1://readyRead
			std::cout << "read...\n";
			client->fillBuf(m_currentString);
			//std::cout << currentString;
			//AddTask(m_queue,m_currentString);
			ResetEvent(client->readyRead);
			break;
		case WAIT_OBJECT_0+2://closed
			ResetEvent(client->closed);
			break;
		case WAIT_TIMEOUT:
			break;
		default:
			break;
		}
	}
	return 0;
}

VOID Manager::AddTask(std::queue<Task>& queue, std::string& buf)
{
	if (queue.empty())return VOID();
	std::size_t startPos, endPos;
	startPos = buf.find("<<");

	return VOID();
}

VOID Manager::GetInfo(std::string& buf)
{
	auto GetProcessInfo = [](std::stringstream& stream)->int {
		unsigned int count{ 0 };
		TCHAR processName[MAX_PATH] = TEXT("unknown");
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapShot) {
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hSnapShot, &pe32))
				do
				{
					count++;
					stream << std::setw(35) << std::left << std::string(pe32.szExeFile) << pe32.th32ProcessID << "\n";

				} while (Process32Next(hSnapShot, &pe32));
				CloseHandle(hSnapShot);
				return count;
		}
		else return 0;
	};
	buf.clear();
	std::stringstream stream;
	stream << "New Connection\n";
	char userName[50];
	char computerName[50];
	DWORD size = sizeof(userName) / sizeof(userName[0]);
	GetUserNameA(userName, &size);
	stream<<"userName: "<<std::string(userName)<<"\n";
	DWORD size2 = sizeof(computerName) / sizeof(computerName[0]);
	GetComputerNameA(computerName, &size2);
	stream << "computerName: " + std::string(computerName) + "\n";
	stream << "CurrentProcessId: " + std::to_string(GetCurrentProcessId()) + "\n";
	stream << "List of processes: \n";
	stream << std::setw(35) << std::left << "NAME" << "ProcID" << "\n";
	GetProcessInfo(stream);
	buf = stream.str();
	return VOID();
}
