#include "TimeClass.h"
#include <chrono>

std::chrono::system_clock::time_point TimeClass::now;
std::chrono::system_clock::time_point TimeClass::pre;

TimeClass::TimeClass()
{
}

TimeClass::~TimeClass()
{
}

void TimeClass::CheckTime()
{
	pre = now;
	now = std::chrono::system_clock::now();
}

double TimeClass::GetDeltaTime()
{
	std::chrono::duration<double> sec = now - pre;
	return sec.count();
}
