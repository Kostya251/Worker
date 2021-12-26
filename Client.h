#pragma once
#include "resources.h"
#define BUF_SIZE 1024
class Client
{
public:
	Client(const char* serverName_, const char* Port_);
	BOOL ConnectToServer();
	BOOL CloseClient();
	BOOL StartThread();

	HANDLE connected;
	HANDLE readyRead;
	HANDLE closed;

	BOOL Send(std::string& buf);

	BOOL fillBuf(std::string& str);

	friend DWORD WINAPI MainThread(LPVOID lpParam);
private:

	DWORD   dwThreadId;
	HANDLE  hThread;

	const char* serverName = NULL;
	const char* Port = NULL;
	WSAEVENT hEvent = WSA_INVALID_EVENT;

	SOCKET m_socket = INVALID_SOCKET;
	const char* sendbuf = "default msg";
	const char* readbuf[BUF_SIZE];
	WSADATA wsaData;
	struct addrinfo* result = NULL, hints;
	int iResult;

	BOOL isConnected = FALSE;

	BOOL InitSocket();
	BOOL CreateEvent_();

	BOOL OnRead();
	BOOL OnWrite();
	BOOL OnConnect();
};

