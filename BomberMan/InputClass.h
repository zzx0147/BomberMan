#pragma once
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	static void Initialize();

	static void KeyDown(unsigned int);
	static void KeyUp(unsigned int);
	static bool IsKeyDown(unsigned int);

private:
	static bool m_keys[256];
};

