#include "SocketManager.h"
#include <iostream>
#include <tchar.h>

// ���� �Լ� ���� ��� �� ����
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

	// SO_REUSEADDR �ɼ� ����
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

	// ��Ƽĳ��Ʈ �׷� ����
	struct ip_mreq mreq;
	inet_pton(AF_INET, MULTICASTIP, &mreq.imr_multiaddr.s_addr);

	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// ������ ��ſ� ����� ����
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	//���� �ð� ���� ������ ������ ��Ŷ�� ��ٸ���. �� �ð����� ��Ŷ�� ���� ���� �� ������ ���� ������ �����Ѵ�.
	int waittime = 3000;
	retval = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&waittime, sizeof(waittime));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// ��Ƽĳ��Ʈ ������ �ޱ�
	addrlen = sizeof(peeraddr);
	retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);

	if (retval == SOCKET_ERROR)
	{
		err_display(TEXT("recvfrom()"));
		serverFound = false;
	}
	else
	{
		// ���� ������ ���
		buf[retval] = '\0';
		char ip[32];
		inet_ntop(AF_INET, &peeraddr.sin_addr, ip, sizeof(ip));
		printf("[UDP/%s:%d] %s\n", ip, ntohs(peeraddr.sin_port), buf);
		serverIpOutput = peeraddr.sin_addr.S_un.S_addr;
		serverFound = true;
	}

	// ��Ƽĳ��Ʈ �׷� Ż�� �� ���� ����
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

	// ��Ƽĳ��Ʈ TTL ����
	int ttl = 10;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));

	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	inet_pton(AF_INET, MULTICASTIP, &remoteaddr.sin_addr.s_addr);
	remoteaddr.sin_port = htons(MULTICASTPORT);

	while (isSpreadIpOn) {
		// �ٵ� ���� ����� ����, Ŭ���̾�Ʈ���� ���� �����Ǹ� �˸��� ����
		retval = sendto(sock, "ServerIp", strlen("ServerIp"), 0, (SOCKADDR *)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR) {
			err_display(TEXT("sendto()"));
			continue;
		}
		printf("���� �����Ǹ� �ѷȽ��ϴ�\n");
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

	// ������ ��ſ� ����� ����

	int addrlen;
	char buf[BUFSIZE + 1];
	// Ŭ���̾�Ʈ�� ������ ���
	addrlen = sizeof(_other_Addr_UDP);
	retval = recvfrom(_UDP_Socket, buf, BUFSIZE, 0, (SOCKADDR *)&_other_Addr_UDP, &addrlen);
	if (retval == SOCKET_ERROR) { err_display(TEXT("recvfrom()")); return -1; }
	// ���� ������ ���
	buf[retval] = '\0';
	printf("[UDP/%s:%d] %s\n", inet_ntoa(_other_Addr_UDP.sin_addr), ntohs(_other_Addr_UDP.sin_port), buf);

	Sleep(10000);

	// ������ ������
	retval = sendto(_UDP_Socket, buf, retval, 0, (SOCKADDR *)&_other_Addr_UDP, sizeof(_other_Addr_UDP));
	if (retval == SOCKET_ERROR) { err_display(TEXT("sendto()")); return -1; }

	std::cout << "[����] UDP ���� ����" << std::endl;

	return 0;
}

int SocketManager::ConnectToUDPServer(unsigned long ip)
{
	int retval;
	// socket()

	_socket_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	if (_socket_UDP == INVALID_SOCKET) err_quit(TEXT("socket()"));

	// ���� �ּ� ����ü �ʱ�ȭ
	ZeroMemory(&_other_Addr_UDP, sizeof(_other_Addr_UDP));
	_other_Addr_UDP.sin_family = AF_INET;
	_other_Addr_UDP.sin_addr.s_addr = ip;
	_other_Addr_UDP.sin_port = htons(UDPPORT);

	// ������ ��ſ� ����� ����
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// ������ ������
	retval = sendto(_socket_UDP, "UDP client", strlen("UDP client"), 0, (SOCKADDR *)&_other_Addr_UDP, sizeof(_other_Addr_UDP));
	if (retval == SOCKET_ERROR) { err_display(TEXT("sendto()"));  return -1;}
	printf("[UDP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
	
	Sleep(10000);

	// ������ �ޱ�
	addrlen = sizeof(peeraddr);
	retval = recvfrom(_socket_UDP, buf, BUFSIZE, 0, (SOCKADDR *)&peeraddr, &addrlen);
	if (retval == SOCKET_ERROR) { err_display(TEXT("recvfrom()")); return -1; }

	if (memcmp(&_other_Addr_UDP, &peeraddr, sizeof(_other_Addr_UDP)) == 0)
	{
		std::cout << "[Ŭ���̾�Ʈ] UDP ���� ����" << std::endl;
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

	// ������ ��ſ� ����� ����
	int addrlen;

	// accept()
	addrlen = sizeof(_other_Addr_UDP);
	_client_Socket_TCP = accept(_listen_Socket_TCP, (SOCKADDR*)&_other_Addr_UDP, &addrlen);
	if (_client_Socket_TCP == INVALID_SOCKET) { err_display(TEXT("accept()")); return -1; }

	// ������ Ŭ���̾�Ʈ ���� ���
	char ip[32] = { 0 };
	inet_ntop(AF_INET, &_other_Addr_UDP.sin_addr, ip, sizeof(ip));
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n", ip, ntohs(_other_Addr_UDP.sin_port));

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
	printf("\n[TCP Ŭ���̾�Ʈ] ���� ����: IP �ּ�=%s", ip);
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

