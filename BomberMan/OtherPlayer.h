#pragma once
#include <thread>
#include "Player.h"
class OtherPlayer :
	public Player
{
public:
	OtherPlayer(SocketManager&);
	~OtherPlayer();

	void Update(double deltatime) override;
	void SetBomb();

private:
	void RecvTCP();

private:
	std::thread* _TCPThread;
	bool isTCPThreadOn;
};

