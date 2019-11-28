#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>

#define MULTICASTIP "235.7.8.9"
#define LOCALPORT 3317
#define REMOTEPORT 3317
#define BUFSIZE 512
#define SERVERPORT 3318

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	int Send();//��Ƽĳ��Ʈ ����
	int Recive();//��Ƽĳ��Ʈ ����

	bool FindServer(unsigned long&);//������ ��Ƽĳ��Ʈ�� �ϴ��� Ȯ���ؼ� �Ѵٸ� ���� ip�� unsigned long���� ����
	int SpreadIP();//�ڽ��� �ּҸ� ��Ƽĳ��Ʈ�� �Ѹ�, �Ѹ� �ּҸ� FindServer�� �޾ƿ�

	int OpenTCPServer();//TCP ������ ����.
	int ConnectToTCPServer(unsigned long);//TCP ������ �����Ѵ�. ���� �ϳ��� ����� ��! 
	                          //_Client_Socket�� �����ϱ� ����! ���� Recive�� Send �Լ��� ������ ����!
	void StopSpreadIp();

	void ReciveTCP();
	void SendTCP();
	
private:
	WSADATA _wsa;
	SOCKET _Server_Socket;
	SOCKET _Client_Socket;
	SOCKADDR_IN _Client_Addr;
	bool isSpreadIpOn;
};

