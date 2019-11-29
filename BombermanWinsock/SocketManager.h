#pragma once
#pragma comment(lib, "ws2_32")
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#define MULTICASTIP "235.7.8.9"
#define MULTICASTPORT 3317
#define BUFSIZE 512
#define TCPPORT 3318
#define UDP_S2C_PORT 3319 //서버에서 클라로 데이터를 전송할때 쓰는 포트
#define UDP_C2S_PORT 3320 //클라에서 서버로 데이터를 전송할때 쓰는 포트

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	std::thread* StartSpreadIP();     //SpreadIP를 쓰레드로 시작시킴
	void StopSpreadIP();      //SpreadIP를 중단시킴
	bool FindServer(unsigned long&);//서버가 멀티캐스트를 하는지 확인해서 한다면 서버 ip를 unsigned long으로 리턴

	int OpenTCPServer(unsigned long&);//TCP 서버를 연다.
	int ConnectToTCPServer(unsigned long);//TCP 서버에 접속한다. 둘중 하나만 사용할 것! 
	                          //_Client_Socket을 공유하기 때문! 또한 Recive()와 Send() 함수도 공유할 예정

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
	int SpreadIP();//자신의 주소를 멀티캐스트로 뿌림, 뿌린 주소를 FindServer로 받아옴
	int ReciveUDP();
	int SendUDP();


private:
	WSADATA _wsa;
	
	SOCKET _listen_Socket_TCP;
	SOCKET _client_Socket_TCP;
	
	SOCKET _send_Socket_UDP;//server to client
	SOCKET _recv_Socket_UDP;//client to server
	SOCKADDR_IN _sendTarget_Addr_UDP;//UDP에서 사용하는 sendto의 타겟 주소, 서버면 클라의 주소, 클라면 서버의 주소
	
	std::thread* _spreadIPThread;
	bool _isSpreadIpOn;//SpreadIP()가 작동하는지를 표시하는 bool, false가 되면 SpreadIP()가 꺼짐

	std::thread* _sendUDPThread;
	bool _isSendUDPOn;

	std::thread* _recvUDPThread;
	bool _isRecvUDPOn;
};

