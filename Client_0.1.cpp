

const char* serverName = "192.168.1.40";
const char* Port = "25000";
#include "Manager.h"
int main() {
	Manager manager(serverName,Port);
	manager.Start();

	system("pause");
	return 0;
}