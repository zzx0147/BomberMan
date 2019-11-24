#include "InputClass.h"


bool InputClass::m_keys[256];

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass &)
{
}

InputClass::~InputClass()
{
}

void InputClass::Initialize()
{
	
	for (int i = 0; i < 256; ++i)//m_keys ÃÊ±âÈ­
	{
		m_keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	m_keys[input] = true;
	return;
}

void InputClass::KeyUp(unsigned int input)
{
	m_keys[input] = false;
	return;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
