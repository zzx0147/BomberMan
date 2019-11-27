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

	virtual void Init() = 0;
	virtual void Update(const double deltaTime) = 0;
	virtual void Render() = 0;
};

