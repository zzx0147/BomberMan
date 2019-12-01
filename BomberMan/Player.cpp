#include "Player.h"

#include <cmath>

#include "BombManager.h"
#include "SpritesLoader.h"
#include "InputClass.h"
#include "GameMap.h"

Player::Player(SocketManager& socket):socket(socket)
{
}


Player::~Player()
{
}

void Player::Init()
{
	 _x = 48;
	 _y = 48;

	 _dir = eDirection::DOWN;
	 _aniSpeed = 0;
}

void Player::Update(double deltaTime)
{
	static double aniResetTime = 0.0;
	double moveSpeed = 3 * 48 * deltaTime;

	double px = _x;
	double py = _y;

	aniResetTime += deltaTime;
	// Input Update
	if (InputClass::IsKeyDown(VK_LEFT))
	{
		_dir = eDirection::LEFT;

		px -= moveSpeed;
		py = round(_y / 48) * 48;
	}
	else if (InputClass::IsKeyDown(VK_RIGHT))
	{
		_dir = eDirection::RIGHT;

		px += moveSpeed;
		py = round(_y / 48) * 48;
	}

	if (InputClass::IsKeyDown(VK_UP))
	{
		_dir = eDirection::UP;

		px = round(_x / 48) * 48;
		py -= moveSpeed;
	}
	else if (InputClass::IsKeyDown(VK_DOWN))
	{
		_dir = eDirection::DOWN;

		px = round(_x / 48) * 48;
		py += moveSpeed;
	}

	if (InputClass::IsKeyDown(VK_SPACE))
	{
		if (GameMap::IsMovePoint(_x, _y))
		{
			BombManager::CreateBomb(_x, _y);
			Packet sendbomb = {0};
			socket.SendTCP((char*)&sendbomb,sizeof(sendbomb));
		}
	}

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

	Packet packet;
	packet.x = _x;
	packet.y = _y;

	socket.SendUDPOnce(packet);

}

void Player::Render()
{
	switch (_dir)
	{
		case LEFT: SpritesLoader::DrawSprite(_x, _y, 0 + _aniSpeed, 0); break;
		case DOWN: SpritesLoader::DrawSprite(_x, _y, 3 + _aniSpeed, 0); break;
		case RIGHT: SpritesLoader::DrawSprite(_x, _y, 0 + _aniSpeed, 1); break;
		case UP: SpritesLoader::DrawSprite(_x, _y, 3 + _aniSpeed, 1); break;
	}
}

const double Player::GetX()
{
	return _x;
}

const double Player::GetY()
{
	return _y;
}
