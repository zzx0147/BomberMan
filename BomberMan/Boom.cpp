#include "Boom.h"

#include "SpritesLoader.h"
#include "BoomManager.h"
Boom::Boom()
{
}


Boom::~Boom()
{
	Release();
}

void Boom::Burst()
{
	Release();

	BoomManager::DeleteBoom(_idxNum);
}

void Boom::Init(const int x, const int y, const int idxNum)
{
	_x = x;
	_y = y;
	_idxNum = idxNum;
	_aniSpeed = 0;
	_delTime = 0;
}

void Boom::Update(double deltaTime)
{
	_aniSpeed = _aniSpeed > 2 ? 0 : _aniSpeed + deltaTime * 3;

	_delTime += deltaTime;
	if (_delTime > 3.0)
	{
		Burst();
	}
}

void Boom::Render()
{
	SpritesLoader::DrawSprite(_x, _y, 0, 3 + _aniSpeed);
}

void Boom::Release()
{
	_x = 0;
	_y = 0;
	_idxNum = -1;
	_aniSpeed = 0;
}

const int Boom::GetIdx() const
{
	return _idxNum;
}

const double Boom::GetX() const
{
	return _x;
}

const double Boom::GetY() const
{
	return _y;
}
