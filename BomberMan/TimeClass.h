#pragma once
#include <chrono>
class TimeClass
{
public:
	TimeClass();
	~TimeClass();
	
	static void CheckTime();
	static double GetDeltaTime();
	
private:
	static std::chrono::system_clock::time_point now;
	static std::chrono::system_clock::time_point pre;

};


