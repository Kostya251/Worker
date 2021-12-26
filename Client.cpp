#include "Client.h"
DWORD __stdcall MainThread(LPVOID lpParam);
Client::Client(const char* serverName_, const char* Port_)
{
	serverName = serverName_;
	Port = Port_;
	m_socket = INVALID_SOCKET;
	sendbuf = "default msg\n";
	result = NULL;
}

BOOL Client::ConnectToServer()
{
	InitSocket();
	return 0;
}

BOOL Client::CloseClient()
{
	std::cout << "CloseClient\n";
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	WSACleanup();
	return 0;
}

BOOL Client::StartThread()
{
	hThread = CreateThread(NULL, 0, MainThread, this, NULL, &dwThreadId);
	if (hThread == NULL)
	{
		std::cout << "error create thread\n";
		return 4;
	}
	return 0;
}

BOOL Client::Send(std::string& buf)
{
	sendbuf = buf.c_str();
	int iResult = send(m_socket, sendbuf, (int)strlen(sendbuf), 0);
	std::cout << "send result: " << iResult<<", len:"<<strlen(sendbuf)<<"\n";
	return 0;
}

BOOL Client::fillBuf(std::string& str)
{
	std::cout << "fillBuf\n";
	str.clear();
	//int count;//количество байт, которые нужно прочитать
	//recv(m_socket, (char*)&count, sizeof(int), 0);
	//std::cout << count << "\n";
	//char* buf = new char[count];
	//int i{ 0 };
	//int res = recv(m_socket, buf, count, 0);
	char* buf = new char[BUFSIZ];
	int res = recv(m_socket, buf, BUFSIZ, 0);
	buf[res] = '\0';
	//Sleep(1000);
	str.append(buf);
	delete[]buf;
	return 0;
}

BOOL Client::InitSocket()
{
	std::cout << "InitSocket: ";
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed: " << iResult << "\n";
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo(serverName, Port, &hints, &result);
	if (iResult != 0)
	{
		std::cout << "getaddrinfo failed with error: " << iResult << "\n";
		WSACleanup();
		return 2;
	}
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		std::cout << "socket failed with error: " << WSAGetLastError() << "\n";
		WSACleanup();
		return 3;
	}
	CreateEvent_();
	StartThread();
	std::cout << "GOOD\n";
	return 0;
}

BOOL Client::CreateEvent_()
{
	readyRead = CreateEventA(NULL, FALSE, FALSE, NULL);
	connected = CreateEventA(NULL, FALSE, FALSE, NULL);
	closed = CreateEventA(NULL, FALSE, FALSE, NULL);
	hEvent = WSACreateEvent();
	WSAEventSelect(m_socket, hEvent, FD_CONNECT | FD_READ | FD_CLOSE | FD_WRITE);

	return 0;
}

BOOL Client::OnRead()
{
	SetEvent(readyRead);
	return 0;
}

BOOL Client::OnWrite()
{
	return 0;
}

BOOL Client::OnConnect()
{
	SetEvent(connected);
	return 0;
}

DWORD __stdcall MainThread(LPVOID lpParam)
{
	Client* client = (Client*)lpParam;

	for (;;) {
		if (!client->isConnected) {
			fd_set fd;
			timeval time_out;
			FD_ZERO(&fd); // Обнуляем множество
			FD_SET(client->m_socket, &fd); // Заносим в него наш сокет
			time_out.tv_sec = 0; time_out.tv_usec = 1000000; //Таймаут 1 сек.
			std::cout << "trying connect to server\n";
			int res = select(0, NULL, &fd, NULL, &time_out);
			if (res == 0) {
				client->iResult = connect(client->m_socket, client->result->ai_addr, (int)client->result->ai_addrlen);
			}
			else client->isConnected = TRUE;
		}
		int returnCode = WSAWaitForMultipleEvents(1, &client->hEvent, FALSE, 10, FALSE);
		WSANETWORKEVENTS netEvent;
		WSAEnumNetworkEvents(client->m_socket, client->hEvent, &netEvent);

		if ((netEvent.lNetworkEvents & FD_READ) && (netEvent.iErrorCode[FD_READ_BIT] == 0)) {
			std::cout << "ready to read\n";
			client->OnRead();
		}
		if ((netEvent.lNetworkEvents & FD_CONNECT) && (netEvent.iErrorCode[FD_CONNECT_BIT] == 0)) {
			freeaddrinfo(client->result);
			std::cout << "connected to " << client->serverName << "\n";
			client->iResult = send(client->m_socket, client->sendbuf, (int)strlen(client->sendbuf), 0);
			client->OnConnect();
		}
		if ((netEvent.lNetworkEvents & FD_WRITE) && (netEvent.iErrorCode[FD_WRITE_BIT] == 0)) {
			std::cout << "ready to write\n";
			client->OnWrite();
		}
	}
	return 0;
}
