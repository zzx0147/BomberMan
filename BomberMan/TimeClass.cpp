#include "TimeClass.h"

#include <Windows.h>

double TimeClass::_secondsPerCount = 0.0;

double TimeClass::_prevTime = 0.0;
double TimeClass::_deltaTime = 0.0;

TimeClass::TimeClass()
{
}

TimeClass::~TimeClass()
{
}

void TimeClass::Init()
{
	__int64 countPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec);

	_secondsPerCount = 1.0f / (double)countPerSec;

	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	_deltaTime = 0;
	_prevTime = curTime;
}

void TimeClass::Update()
{
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	_deltaTime = (curTime - _prevTime) * _secondsPerCount;
	_prevTime = curTime;
}

double TimeClass::GetDeltaTime()
{
	return _deltaTime;
}
