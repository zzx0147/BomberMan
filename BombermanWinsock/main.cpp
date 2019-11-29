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

	if (!s.FindServer(serverIP))//������ ã�´� ��ã���� �ڽ��� ������ �Ǿ� �����Ǹ� �Ѹ��� TCP listen �Ѵ�.
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
	else//������ ã������ �ش� �ּҷ� TCP connect �Ѵ�
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