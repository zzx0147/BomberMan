#pragma once
#include <Windows.h>
class Character
{
public:
	Character();
	~Character();

	void Frame();
	void Render();
	RECT GetCharacterRect();
private:
	double x;
	double y;
};

