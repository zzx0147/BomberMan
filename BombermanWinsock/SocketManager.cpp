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

	_isSpreadIpOn = false;
	_isSendUDPOn = false;
	_isRecvUDPOn = false;

	_spreadIPThread = nullptr;
	_sendUDPThread = nullptr;
	_recvUDPThread = nullptr;

	_recv_Socket_UDP = INVALID_SOCKET;
	_send_Socket_UDP = INVALID_SOCKET;
	_listen_Socket_TCP = INVALID_SOCKET;
	_client_Socket_TCP = INVALID_SOCKET;
}

SocketManager::~SocketManager()
{
	WSACleanup();
	if (_spreadIPThread != nullptr &&_spreadIPThread->joinable()) _spreadIPThread->join();

	if (_sendUDPThread != nullptr &&_sendUDPThread->joinable()) _sendUDPThread->join();

	if (_recvUDPThread != nullptr &&_recvUDPThread->joinable()) _recvUDPThread->join();

	if (_recv_Socket_UDP != INVALID_SOCKET) { closesocket(_recv_Socket_UDP); }
	if (_send_Socket_UDP != INVALID_SOCKET) { closesocket(_send_Socket_UDP); }
	if (_listen_Socket_TCP != INVALID_SOCKET) { closesocket(_listen_Socket_TCP); }
	if (_client_Socket_TCP != INVALID_SOCKET) { closesocket(_client_Socket_TCP); }
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
	_isSpreadIpOn = true;

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

	while (_isSpreadIpOn) {
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

int SocketManager::ReciveUDP()
{
	int addrlen;
	int retval;
	char buf[BUFSIZE + 1];
	SOCKADDR_IN peeraddr;
	_isRecvUDPOn = true;
	while (_isRecvUDPOn)
	{
		addrlen = sizeof(peeraddr);
		retval = recvfrom(_recv_Socket_UDP, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) { err_display(TEXT("recvfrom()"));    continue; }

		// 송신자의 IP 주소 체크   
		if (peeraddr.sin_addr.s_addr != _sendTarget_Addr_UDP.sin_addr.s_addr)
		{
			printf("[오류] 잘못된 데이터입니다!\n");    continue;
		}

		// 받은 데이터 출력   
		buf[retval] = '\0';
		printf("[UDP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}
	return 0;
}

int SocketManager::SendUDP()
{
	int len;
	int retval;
	_isSendUDPOn = true;
	while (_isSendUDPOn)
	{
		char buf[BUFSIZE + 1];
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)    break;

		// '\n' 문자 제거 
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// 데이터 보내기   
		retval = sendto(_send_Socket_UDP, buf, strlen(buf), 0, (SOCKADDR *)&_sendTarget_Addr_UDP, sizeof(_sendTarget_Addr_UDP));

		if (retval == SOCKET_ERROR) { err_display(TEXT("sendto()"));    continue; }
	}

	return 0;
}

int SocketManager::OpenTCPServer(unsigned long& clinetIpOutput)
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
	addrlen = sizeof(_sendTarget_Addr_UDP);
	_client_Socket_TCP = accept(_listen_Socket_TCP, (SOCKADDR*)&_sendTarget_Addr_UDP, &addrlen);
	if (_client_Socket_TCP == INVALID_SOCKET) { err_display(TEXT("accept()")); return -1; }

	clinetIpOutput = _sendTarget_Addr_UDP.sin_addr.s_addr;

	// 접속한 클라이언트 정보 출력
	char ip[32] = { 0 };
	inet_ntop(AF_INET, &_sendTarget_Addr_UDP.sin_addr, ip, sizeof(ip));
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n", ip, ntohs(_sendTarget_Addr_UDP.sin_port));

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

std::thread* SocketManager::StartSpreadIP()
{
	_spreadIPThread = new std::thread([&]() {this->SpreadIP(); });
	return _spreadIPThread;
}

void SocketManager::StopSpreadIP()
{
	_isSpreadIpOn = false;
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

int SocketManager::InitUDP_Server(unsigned long clientIp)
{
	return InitUDP(UDP_C2S_PORT, UDP_S2C_PORT, clientIp);
}

int SocketManager::InitUDP_Client(unsigned long serverIp)
{
	return InitUDP(UDP_S2C_PORT, UDP_C2S_PORT, serverIp);
}

int SocketManager::InitUDP(unsigned short recvPort, unsigned short sendPort, unsigned long ip)
{
	int retval;
	_recv_Socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (_recv_Socket_UDP == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// bind()  
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(recvPort);
	retval = bind(_recv_Socket_UDP, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));

	_send_Socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (_send_Socket_UDP == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// 소켓 주소 구조체 초기화  
	ZeroMemory(&_sendTarget_Addr_UDP, sizeof(_sendTarget_Addr_UDP));
	_sendTarget_Addr_UDP.sin_family = AF_INET;
	_sendTarget_Addr_UDP.sin_addr.s_addr = ip;
	_sendTarget_Addr_UDP.sin_port = htons(sendPort);
	return 0;
}

std::thread* SocketManager::StartReciveUDP()
{
	_recvUDPThread = new std::thread([&]() {this->ReciveUDP(); });
	return _recvUDPThread;
}

void SocketManager::StopReciveUDP()
{
	_isRecvUDPOn = false;
}

std::thread* SocketManager::StartSendUDP()
{
	_sendUDPThread = new std::thread([&]() {this->SendUDP(); });
	return _sendUDPThread;
}

void SocketManager::StopSendUDP()
{
	_isSendUDPOn = false;
}

