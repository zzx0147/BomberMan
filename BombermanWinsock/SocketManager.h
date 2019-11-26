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

	int Send();//멀티캐스트 예제
	int Recive();//멀티캐스트 예제

	bool FindServer(unsigned long&);//서버가 멀티캐스트를 하는지 확인해서 한다면 서버 ip를 unsigned long으로 리턴
	int SpreadIP();//자신의 주소를 멀티캐스트로 뿌림, 뿌린 주소를 FindServer로 받아옴

	void OpenTCPServer();
	void ConnectToTCPServer();
	
private:
	WSADATA wsa;
};

