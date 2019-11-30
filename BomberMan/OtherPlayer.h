#pragma once
#include "Player.h"
class OtherPlayer :
	public Player
{
public:
	OtherPlayer(SocketManager&);
	~OtherPlayer();

	void Update(double deltatime) override;

};

