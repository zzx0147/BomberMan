#include <iostream>
#include <thread>
#include "SocketManager.h"
using namespace std;

int main()
{
	SocketManager s;
	unsigned long serverIP;
	
	if (!s.FindServer(serverIP))//서버를 찾는다.
	{
		thread t([&]() {s.SpreadIP();});

		if (s.OpenTCPServer() == 0)
		{
			s.StopSpreadIp();
		}
		t.join();
	}
	else
	{
		s.ConnectToTCPServer(serverIP);
	}

	system("pause");

	return 0;
}