#pragma once
#pragma comment(lib, "ws2_32")
#include <thread>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#define MULTICASTIP "235.7.8.9"
#define MULTICASTPORT 3317
#define BUFSIZE 512
#define TCPPORT 9090
#define UDP_S2C_PORT 5080 //�������� Ŭ��� �����͸� �����Ҷ� ���� ��Ʈ
#define UDP_C2S_PORT 5090 //Ŭ�󿡼� ������ �����͸� �����Ҷ� ���� ��Ʈ

struct Packet
{
	double x;
	double y;
};

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

	int ReciveTCP(char*,const int&);//����� TCP�� ���� �����͸� �޴´�.
	int SendTCP(char*,const int&);//����� TCP�� ���� �����͸� ������.

	int InitUDP_Server(unsigned long);//�������� UDP �ʱ�ȭ      // �� ���� ������ �޴� ��Ʈ ������ �ٸ���
	int InitUDP_Client(unsigned long);//Ŭ���̾�Ʈ���� UDP �ʱ�ȭ//
	int InitUDP(unsigned short, unsigned short,unsigned long);//UDP ��� �ʱ�ȭ

	std::thread* StartReciveUDP();
	void StopReciveUDP();
	std::thread* StartSendUDP();
	void StopSendUDP();
	int SendUDPOnce(const Packet&);
	int RecvUDPOnce(Packet&);

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

