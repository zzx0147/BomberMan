#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();

	// Character��(��) ���� ��ӵ�
	void Init();
	void Update(double deltaTime);
	void Render();

	const double GetX();
	const double GetY();
private:
	double _x;
	double _y;

	eDirection _dir;
	double _aniSpeed;
};

