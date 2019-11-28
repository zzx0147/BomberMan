#pragma once
#pragma comment(lib, "ws2_32")
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MULTICASTIP "235.7.8.9"
#define MULTICASTPORT 3317
#define BUFSIZE 512
#define TCPPORT 3318
#define UDPPORT 3319

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	void StartSpreadIP();     //SpreadIP�� ������� ���۽�Ŵ
	void StopSpreadIP();      //SpreadIP�� �ߴܽ�Ŵ
	bool FindServer(unsigned long&);//������ ��Ƽĳ��Ʈ�� �ϴ��� Ȯ���ؼ� �Ѵٸ� ���� ip�� unsigned long���� ����

	int OpenTCPServer();//TCP ������ ����.
	int ConnectToTCPServer(unsigned long);//TCP ������ �����Ѵ�. ���� �ϳ��� ����� ��! 
	                          //_Client_Socket�� �����ϱ� ����! ���� Recive()�� Send() �Լ��� ������ ����
	int OpenUDPServer();
	int ConnectToUDPServer(unsigned long);

	int ReciveTCP(char*,const int&);
	int SendTCP(char*,const int&);

	void StartReciveUDP();
	void StopReciveUDP();
	void StartSendUDP();
	void StopSendUDP();
	
private:
	int SpreadIP();//�ڽ��� �ּҸ� ��Ƽĳ��Ʈ�� �Ѹ�, �Ѹ� �ּҸ� FindServer�� �޾ƿ�



private:
	WSADATA _wsa;
	
	SOCKET _listen_Socket_TCP;
	SOCKET _client_Socket_TCP;
	
	SOCKET _socket_UDP;//UDP�� ����ϴ� ����
	SOCKADDR_IN _other_Addr_UDP;//UDP���� ����ϴ� �ݴ����� �ּ�, ������ Ŭ���� �ּ�, Ŭ��� ������ �ּ�
	bool isSpreadIpOn;//SpreadIP()�� �۵��ϴ����� ǥ���ϴ� bool, false�� �Ǹ� SpreadIP()�� ����
	std::thread* spreadIPThread;
};

