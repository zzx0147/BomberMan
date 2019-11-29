#pragma once
#pragma comment(lib, "ws2_32")
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MULTICASTIP "235.7.8.9"
#define MULTICASTPORT 3317
#define BUFSIZE 512
#define TCPPORT 3318
#define UDP_S2C_PORT 3319 //�������� Ŭ��� �����͸� �����Ҷ� ���� ��Ʈ
#define UDP_C2S_PORT 3320 //Ŭ�󿡼� ������ �����͸� �����Ҷ� ���� ��Ʈ

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	std::thread* StartSpreadIP();     //SpreadIP�� ������� ���۽�Ŵ
	void StopSpreadIP();      //SpreadIP�� �ߴܽ�Ŵ
	bool FindServer(unsigned long&);//������ ��Ƽĳ��Ʈ�� �ϴ��� Ȯ���ؼ� �Ѵٸ� ���� ip�� unsigned long���� ����

	int OpenTCPServer(unsigned long&);//TCP ������ ����.
	int ConnectToTCPServer(unsigned long);//TCP ������ �����Ѵ�. ���� �ϳ��� ����� ��! 
	                          //_Client_Socket�� �����ϱ� ����! ���� Recive()�� Send() �Լ��� ������ ����

	int ReciveTCP(char*,const int&);
	int SendTCP(char*,const int&);

	int InitUDP_Server(unsigned long);
	int InitUDP_Client(unsigned long);
	int InitUDP(unsigned short, unsigned short,unsigned long);

	std::thread* StartReciveUDP();
	void StopReciveUDP();
	std::thread* StartSendUDP();
	void StopSendUDP();
	
private:
	int SpreadIP();//�ڽ��� �ּҸ� ��Ƽĳ��Ʈ�� �Ѹ�, �Ѹ� �ּҸ� FindServer�� �޾ƿ�
	int ReciveUDP();
	int SendUDP();


private:
	WSADATA _wsa;
	
	SOCKET _listen_Socket_TCP;
	SOCKET _client_Socket_TCP;
	
	SOCKET _send_Socket_UDP;//server to client
	SOCKET _recv_Socket_UDP;//client to server
	SOCKADDR_IN _sendTarget_Addr_UDP;//UDP���� ����ϴ� sendto�� Ÿ�� �ּ�, ������ Ŭ���� �ּ�, Ŭ��� ������ �ּ�
	
	std::thread* _spreadIPThread;
	bool _isSpreadIpOn;//SpreadIP()�� �۵��ϴ����� ǥ���ϴ� bool, false�� �Ǹ� SpreadIP()�� ����

	std::thread* _sendUDPThread;
	bool _isSendUDPOn;

	std::thread* _recvUDPThread;
	bool _isRecvUDPOn;
};

