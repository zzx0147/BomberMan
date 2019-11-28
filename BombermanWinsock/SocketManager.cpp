#include "SocketManager.h"
#include <iostream>
#include <tchar.h>

// 소켓 함수 오류 출력 후 종료
void err_quit(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	system("pause");
	exit(1);
}

void err_display(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	_wsetlocale(LC_ALL, _T("korean"));
	_tprintf(TEXT("[%s] %s"), msg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}

SocketManager::SocketManager()
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0) return;
	isSpreadIpOn = false;
	spreadIPThread = nullptr;
}

SocketManager::~SocketManager()
{
	WSACleanup();
	if (spreadIPThread != nullptr) spreadIPThread->join();
}

bool SocketManager::FindServer(unsigned long& serverIpOutput)
{
	bool serverFound = false;
	int retval;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// SO_REUSEADDR 옵션 설정
	BOOL optval = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// bind()
	SOCKADDR_IN localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_port = htons(MULTICASTPORT);

	retval = bind(sock, (SOCKADDR *)&localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));

	// 멀티캐스트 그룹 가입
	struct ip_mreq mreq;
	inet_pton(AF_INET, MULTICASTIP, &mreq.imr_multiaddr.s_addr);

	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	//일정 시간 동안 서버가 보내는 패킷을 기다린다. 그 시간동안 패킷이 오지 않을 시 서버가 없는 것으로 간주한다.
	int waittime = 3000;
	retval = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&waittime, sizeof(waittime));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// 멀티캐스트 데이터 받기
	addrlen = sizeof(peeraddr);
	retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);

	if (retval == SOCKET_ERROR)
	{
		err_display(TEXT("recvfrom()"));
		serverFound = false;
	}
	else
	{
		// 받은 데이터 출력
		buf[retval] = '\0';
		char ip[32];
		inet_ntop(AF_INET, &peeraddr.sin_addr, ip, sizeof(ip));
		printf("[UDP/%s:%d] %s\n", ip, ntohs(peeraddr.sin_port), buf);
		serverIpOutput = peeraddr.sin_addr.S_un.S_addr;
		serverFound = true;
	}

	// 멀티캐스트 그룹 탈퇴 및 소켓 해제
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	closesocket(sock);

	return serverFound;
}

int SocketManager::SpreadIP()
{
	int retval;
	isSpreadIpOn = true;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// 멀티캐스트 TTL 설정
	int ttl = 10;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// 소켓 주소 구조체 초기화
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	inet_pton(AF_INET, MULTICASTIP, &remoteaddr.sin_addr.s_addr);
	remoteaddr.sin_port = htons(MULTICASTPORT);

	while (isSpreadIpOn) {
		// 바디 없이 헤더만 전송, 클라이언트에게 서버 아이피를 알리는 역할
		retval = sendto(sock, "ServerIp", strlen("ServerIp"), 0, (SOCKADDR *)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR) {
			err_display(TEXT("sendto()"));
			continue;
		}
		printf("서버 아이피를 뿌렸습니다\n");
		Sleep(1000);
	}

	closesocket(sock);
	return 0;
}

int SocketManager::OpenUDPServer()
{
	int retval;

	SOCKET _UDP_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (_UDP_Socket == INVALID_SOCKET) err_quit(TEXT("socket()"));
	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(UDPPORT);
	retval = bind(_UDP_Socket, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));

	// 데이터 통신에 사용할 변수

	int addrlen;
	char buf[BUFSIZE + 1];
	// 클라이언트와 데이터 통신
	addrlen = sizeof(_other_Addr_UDP);
	retval = recvfrom(_UDP_Socket, buf, BUFSIZE, 0, (SOCKADDR *)&_other_Addr_UDP, &addrlen);
	if (retval == SOCKET_ERROR) { err_display(TEXT("recvfrom()")); return -1; }
	// 받은 데이터 출력
	buf[retval] = '\0';
	printf("[UDP/%s:%d] %s\n", inet_ntoa(_other_Addr_UDP.sin_addr), ntohs(_other_Addr_UDP.sin_port), buf);

	Sleep(10000);

	// 데이터 보내기
	retval = sendto(_UDP_Socket, buf, retval, 0, (SOCKADDR *)&_other_Addr_UDP, sizeof(_other_Addr_UDP));
	if (retval == SOCKET_ERROR) { err_display(TEXT("sendto()")); return -1; }

	std::cout << "[서버] UDP 연결 성공" << std::endl;

	return 0;
}

int SocketManager::ConnectToUDPServer(unsigned long ip)
{
	int retval;
	// socket()

	_socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket_UDP == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// 소켓 주소 구조체 초기화
	ZeroMemory(&_other_Addr_UDP, sizeof(_other_Addr_UDP));
	_other_Addr_UDP.sin_family = AF_INET;
	_other_Addr_UDP.sin_addr.s_addr = ip;
	_other_Addr_UDP.sin_port = htons(UDPPORT);

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// 데이터 보내기
	retval = sendto(_socket_UDP, "UDP client", strlen("UDP client"), 0, (SOCKADDR *)&_other_Addr_UDP, sizeof(_other_Addr_UDP));
	if (retval == SOCKET_ERROR) { err_display(TEXT("sendto()"));  return -1;}
	printf("[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);
	
	Sleep(10000);

	// 데이터 받기
	addrlen = sizeof(peeraddr);
	retval = recvfrom(_socket_UDP, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);
	if (retval == SOCKET_ERROR) { err_display(TEXT("recvfrom()")); return -1; }

	if (memcmp(&_other_Addr_UDP, &peeraddr, sizeof(_other_Addr_UDP)) == 0)
	{
		std::cout << "[클라이언트] UDP 연결 성공" << std::endl;
		return 0;
	}

	Sleep(3000);

	return -1;
}

int SocketManager::OpenTCPServer()
{
	int retval;

	// socket()
	_listen_Socket_TCP = socket(AF_INET, SOCK_STREAM, 0);
	if (_listen_Socket_TCP == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(TCPPORT);
	retval = bind(_listen_Socket_TCP, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));

	// listen()
	retval = listen(_listen_Socket_TCP, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(TEXT("listen()"));

	// 데이터 통신에 사용할 변수
	int addrlen;

	// accept()
	addrlen = sizeof(_other_Addr_UDP);
	_client_Socket_TCP = accept(_listen_Socket_TCP, (SOCKADDR*)&_other_Addr_UDP, &addrlen);
	if (_client_Socket_TCP == INVALID_SOCKET) { err_display(TEXT("accept()")); return -1; }

	// 접속한 클라이언트 정보 출력
	char ip[32] = { 0 };
	inet_ntop(AF_INET, &_other_Addr_UDP.sin_addr, ip, sizeof(ip));
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", ip, ntohs(_other_Addr_UDP.sin_port));

	return 0;
}

int SocketManager::ConnectToTCPServer(unsigned long serverip)
{
	int retval;

	// socket()
	_client_Socket_TCP = socket(AF_INET, SOCK_STREAM, 0);
	if (_client_Socket_TCP == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = serverip;
	serveraddr.sin_port = htons(TCPPORT);
	retval = connect(_client_Socket_TCP, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("connect()"));

	char ip[32] = { 0 };
	inet_ntop(AF_INET, &serveraddr.sin_addr, ip, sizeof(ip));
	printf("\n[TCP 클라이언트] 서버 연결: IP 주소=%s", ip);
	return 0;
}

void SocketManager::StartSpreadIP()
{
	spreadIPThread = new std::thread([&]() {this->SpreadIP(); });
}

void SocketManager::StopSpreadIP()
{
	isSpreadIpOn = false;
}

int SocketManager::ReciveTCP(char*data, const int& len)
{
	int retval = recv(_client_Socket_TCP, data, len, 0);
	return retval;
}

int SocketManager::SendTCP(char* data, const int& len)
{
	send(_client_Socket_TCP, data, len, 0);
	return 0;
}

