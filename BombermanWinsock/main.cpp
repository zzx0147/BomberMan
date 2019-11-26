#include <iostream>
#include "SocketManager.h"
using namespace std;

int main()
{
	SocketManager s;
	unsigned long serverIP;

	if (!s.FindServer(serverIP))//서버를 찾는다.
	{
		cout << "Server was not Found" << endl;
		s.SpreadIP();
	}

	system("pause");

	return 0;
}