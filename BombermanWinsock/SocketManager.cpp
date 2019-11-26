#include "SocketManager.h"
#include <iostream>
#include <tchar.h>

using namespace std;

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
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit(TEXT("socket()"));
	// 멀티캐스트 TTL 설정
	int ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,
		(char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// 소켓 주소 구조체 초기화
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	remoteaddr.sin_port = htons(REMOTEPORT);
	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	int len;
	// 멀티캐스트 데이터 보내기

	while (1) {
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;
		// 데이터 보내기
		retval = sendto(sock, buf, strlen(buf), 0,
			(SOCKADDR *)&remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			err_display(TEXT("sendto()"));
			continue;
		}
		printf("[UDP] %d바이트를 보냈습니다.\n", retval);
	}
	// closesocket()
	closesocket(sock);
	// 윈속 종료
	WSACleanup();
	return 0;
}

int SocketManager::Recive()
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit(TEXT("socket()"));
	// SO_REUSEADDR 옵션 설정
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
	// 멀티캐스트 그룹 가입
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICASTIP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	retval = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	// 멀티캐스트 데이터 받기
	while (1) {
		// 데이터 받기
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0,
			(SOCKADDR *)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			err_display(TEXT("recvfrom()"));
			continue;
		}
		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port), buf);
	}
	// 멀티캐스트 그룹 탈퇴
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	// closesocket()
	closesocket(sock);
	// 윈속 종료
	WSACleanup();
	return 0;
}

bool SocketManager::FindServer(unsigned long& serverIpOutput)
{
	bool serverFound = false;
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

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
	localaddr.sin_port = htons(LOCALPORT);
	retval = bind(sock, (SOCKADDR *)&localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) err_quit(TEXT("bind()"));

	// 멀티캐스트 그룹 가입
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MULTICASTIP);
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
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port), buf);
		serverIpOutput = peeraddr.sin_addr.S_un.S_addr;
		serverFound = true;
	}

	// 멀티캐스트 그룹 탈퇴 및 소켓 해제
	retval = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
	if (retval == SOCKET_ERROR) err_quit(TEXT("setsockopt()"));
	closesocket(sock);
	WSACleanup();

	return serverFound;
}

int SocketManager::SpreadIP()
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

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
	remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	remoteaddr.sin_port = htons(REMOTEPORT);

	while (1) {
		// 바디 없이 헤더만 전송, 클라이언트에게 서버 아이피를 알리는 역할
		retval = sendto(sock, "ServerIp", strlen("ServerIp"), 0,
			(SOCKADDR *)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR) {
			err_display(TEXT("sendto()"));
			continue;
		}
		cout << "서버 아이피를 뿌렸습니다" << endl;
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

