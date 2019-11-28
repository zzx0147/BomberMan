#include <iostream>
#include <thread>
#include "SocketManager.h"
using namespace std;

int main()
{
	SocketManager s;
	unsigned long serverIP;
	char buf[512];
	
	if (!s.FindServer(serverIP))//������ ã�´� ��ã���� �ڽ��� ������ �Ǿ� �����Ǹ� �Ѹ��� TCP listen �Ѵ�.
	{
		s.StartSpreadIP();
		if (s.OpenTCPServer() == 0)
		{
			s.StopSpreadIP();
			s.ReciveTCP(buf,sizeof(buf));
			printf("%s",buf);
			s.OpenUDPServer();
		}
	}
	else//������ ã������ �ش� �ּҷ� TCP connect �Ѵ�
	{
		s.ConnectToTCPServer(serverIP);
		strcpy_s(buf, "Call Server");
		s.SendTCP(buf,sizeof(buf));
		s.ConnectToUDPServer(serverIP);
	}
	system("pause");
	return 0;
}