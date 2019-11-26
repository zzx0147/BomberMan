#pragma once
#include <Windows.h>

enum eDirection
{
	LEFT,
	DOWN,
	RIGHT,
	UP,
};

class Character
{
public:
	Character();
	~Character();

	void Frame(double deltaTime);
	void Render();
	RECT GetCharacterRect();
private:
	double x;
	double y;

	eDirection _dir = eDirection::DOWN;
	double _aniSpeed = 0;
};

