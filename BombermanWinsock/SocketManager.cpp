#include "SocketManager.h"
#include <iostream>
#include <tchar.h>

using namespace std;

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
}

SocketManager::~SocketManager()
{
}

int SocketManager::Send()
{
	int retval;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit(TEXT("socket()"));
	// ��Ƽĳ��Ʈ TTL ����
	int ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	remoteaddr.sin_port = htons(REMOTEPORT);
	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;
	// ��Ƽĳ��Ʈ ������ ������

	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		// '\n' ���� ����
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;
		// ������ ������
		retval = sendto(sock, buf, strlen(buf), 0,
			(SOCKADDR *)&remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			err_display(TEXT("sendto()"));
			continue;
		}
		printf("[UDP] %d����Ʈ�� ���½��ϴ�.\n", retval);
	}
	// closesocket()
	closesocket(sock);
	// ���� ����
	WSACleanup();
	return 0;
}

int SocketManager::Recive()
{
	int retval;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit(TEXT("socket()"));
	// SO_REUSEADDR �ɼ� ����
	BOOL optval = TRUE;
	retval = setsockopt(sock, SOL_SOCKET,
		SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// bind()
	SOCKADDR_IN localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_port = htons(LOCALPORT);
	retval = bind(sock, (SOCKADDR *)&localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));
	// ��Ƽĳ��Ʈ �׷� ����
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICASTIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// ������ ��ſ� ����� ����
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	// ��Ƽĳ��Ʈ ������ �ޱ�
	while (1) {
		// ������ �ޱ�
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(SOCKADDR *)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_display(TEXT("recvfrom()"));
			continue;
		}
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port), buf);
	}
	// ��Ƽĳ��Ʈ �׷� Ż��
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// closesocket()
	closesocket(sock);
	// ���� ����
	WSACleanup();
	return 0;
}

bool SocketManager::FindServer(unsigned long& serverIpOutput)
{
	bool serverFound = false;
	int retval;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

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
	localaddr.sin_port = htons(LOCALPORT);
	retval = bind(sock, (SOCKADDR *)&localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));

	// ��Ƽĳ��Ʈ �׷� ����
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICASTIP);
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
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port), buf);
		serverIpOutput = peeraddr.sin_addr.S_un.S_addr;
		serverFound = true;
	}

	// ��Ƽĳ��Ʈ �׷� Ż�� �� ���� ����
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	closesocket(sock);
	WSACleanup();

	return serverFound;
}

int SocketManager::SpreadIP()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

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
	remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	remoteaddr.sin_port = htons(REMOTEPORT);

	while (1) {
		// �ٵ� ���� ����� ����, Ŭ���̾�Ʈ���� ���� �����Ǹ� �˸��� ����
		retval = sendto(sock, "ServerIp", strlen("ServerIp"), 0,
			(SOCKADDR *)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR) {
			err_display(TEXT("sendto()"));
			continue;
		}
		cout << "���� �����Ǹ� �ѷȽ��ϴ�" << endl;
		Sleep(1000);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}

void SocketManager::OpenTCPServer()
{
	
}

void SocketManager::ConnectToTCPServer()
{

}

