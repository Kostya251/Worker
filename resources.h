#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <memory>
//#include <VersionHelpers.h>
#include <tlhelp32.h>
//#include <psapi.h>
//#include <tchar.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#pragma comment(lib, "Ws2_32.lib")

class Task {
public:
	INT id;
	BYTE type;
	std::string command;
	std::vector<std::string> args;
};