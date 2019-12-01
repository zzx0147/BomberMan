#include "OtherPlayer.h"

#include "BombManager.h"
#include "SpritesLoader.h"
#include "InputClass.h"
#include "GameMap.h"
#include <thread>

OtherPlayer::OtherPlayer(SocketManager& socket):Player(socket)
{
	_TCPThread = new std::thread([&]() {this->RecvTCP();});
	isTCPThreadOn = false;
}


OtherPlayer::~OtherPlayer()
{
}

void OtherPlayer::Update(double deltaTime)
{
	static double aniResetTime = 0.0;
	double moveSpeed = 3 * 48 * deltaTime;

	Packet p;
	socket.RecvUDPOnce(p);


	double px = _x;
	double py = _y;

	aniResetTime += deltaTime;
	// Input Update
	if (p.x < px)
	{
		_dir = eDirection::LEFT;

		px = p.x;
		py = p.y;
	}
	else if (p.x > px)
	{
		_dir = eDirection::RIGHT;

		px = p.x;
		py = p.y;
	}

	if (p.y < py)
	{
		_dir = eDirection::UP;

		px = p.x;
		py = p.y;
	}
	else if (p.y > py)
	{
		_dir = eDirection::DOWN;

		px = p.x;
		py = p.y;
	}

	/*if (InputClass::IsKeyDown(VK_SPACE))
	{
		if (GameMap::IsMovePoint(_x, _y))
		{
			BombManager::CreateBomb(_x, _y);
		}
	}*/

	// Move Update
	if (GameMap::IsMovePoint(px, py))
	{
		GameMap::SetBlock(px, py, 8);

		_x = px;
		_y = py;

		aniResetTime = 0;
		_aniSpeed = _aniSpeed > 2 ? 0 : _aniSpeed + (deltaTime * 3);
	}

	_x = _x < 0 ? 0 : _x > 720 - 48 ? 720 - 48 : _x;
	_y = _y < 0 ? 0 : _y > 720 - 48 ? 720 - 48 : _y;

	// Ani Update
	if (.2f < aniResetTime)
	{
		aniResetTime = 0;
		_aniSpeed = 1;
	}
}

void OtherPlayer::SetBomb()
{
	if (GameMap::IsMovePoint(_x, _y))
	{
		BombManager::CreateBomb(_x, _y);
	}
}

void OtherPlayer::RecvTCP()
{
	Packet p;
	isTCPThreadOn = true;
	while (true)
	{
		socket.ReciveTCP((char*)&p,sizeof(p));
		SetBomb();
	}
}
