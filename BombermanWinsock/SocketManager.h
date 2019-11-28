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

	int Send();//멀티캐스트 예제
	int Recive();//멀티캐스트 예제

	bool FindServer(unsigned long&);//서버가 멀티캐스트를 하는지 확인해서 한다면 서버 ip를 unsigned long으로 리턴
	int SpreadIP();//자신의 주소를 멀티캐스트로 뿌림, 뿌린 주소를 FindServer로 받아옴

	int OpenTCPServer();//TCP 서버를 연다.
	int ConnectToTCPServer(unsigned long);//TCP 서버에 접속한다. 둘중 하나만 사용할 것! 
	                          //_Client_Socket을 공유하기 때문! 또한 Recive와 Send 함수도 공유할 예정!
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

