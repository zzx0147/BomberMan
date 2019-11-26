#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>

#define MULTICASTIP "235.7.8.9"
#define LOCALPORT 3317
#define REMOTEPORT 3317
#define BUFSIZE 512

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	int Send();//��Ƽĳ��Ʈ ����
	int Recive();//��Ƽĳ��Ʈ ����

	bool FindServer(unsigned long&);//������ ��Ƽĳ��Ʈ�� �ϴ��� Ȯ���ؼ� �Ѵٸ� ���� ip�� unsigned long���� ����
	int SpreadIP();//�ڽ��� �ּҸ� ��Ƽĳ��Ʈ�� �Ѹ�, �Ѹ� �ּҸ� FindServer�� �޾ƿ�

	void OpenTCPServer();
	void ConnectToTCPServer();
	
private:
	WSADATA wsa;
};

