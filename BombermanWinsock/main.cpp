#include <iostream>
#include <thread>
#include "SocketManager.h"
using namespace std;

int main()
{
	SocketManager s;
	unsigned long serverIP;
	char buf[512];
	
	std::thread* t1;
	std::thread* t2;

	if (!s.FindServer(serverIP))//서버를 찾는다 못찾을시 자신이 서버가 되어 아이피를 뿌리고 TCP listen 한다.
	{
		s.StartSpreadIP();
		unsigned long clientIP;
		if (s.OpenTCPServer(clientIP) == 0)
		{
			s.StopSpreadIP();
			s.ReciveTCP(buf,sizeof(buf));
			printf("%s",buf);
			system("pause");
			s.InitUDP_Server(clientIP);
			t1 = s.StartReciveUDP();
			t2 = s.StartSendUDP();
			t1->join();
			t2->join();
		}
	}
	else//서버를 찾았을시 해당 주소로 TCP connect 한다
	{
		s.ConnectToTCPServer(serverIP);
		strcpy_s(buf, "Call Server");
		s.SendTCP(buf,sizeof(buf));
		system("pause");
		s.InitUDP_Client(serverIP);

		t1 = s.StartReciveUDP();
		t2 = s.StartSendUDP();
		t1->join();
		t2->join();
	}

	return 0;
}