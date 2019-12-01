#pragma once
#include "Character.h"


class Player : public Character
{
public:
	Player(SocketManager&);
	~Player();

	// Character��(��) ���� ��ӵ�
	void Init();
	void Update(double deltaTime);
	void Render();

	const double GetX();
	const double GetY();

private:	



protected:
	SocketManager& socket;
	double _x;
	double _y;
	double _aniSpeed;
	eDirection _dir;
};

