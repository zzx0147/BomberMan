#include "OtherPlayer.h"



OtherPlayer::OtherPlayer(SocketManager& socket):Player(socket)
{
}


OtherPlayer::~OtherPlayer()
{
}

void OtherPlayer::Update(double deltatime)
{
	Packet p;
	socket.RecvUDPOnce(p);
	
	_x = p.x;
	_y = p.y;
}
