#pragma once
#include "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();

	// Character��(��) ���� ��ӵ�
	static void Init();
	static void Update(double deltaTime);
	static void Render();

	static const double GetX();
	static const double GetY();
private:
	static double _x;
	static double _y;

	static eDirection _dir;
	static double _aniSpeed;
};

