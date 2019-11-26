#pragma once
#include <chrono>
class TimeClass
{
public:
	TimeClass();
	~TimeClass();
	
	static void Init();
	static void Update();

	static double GetDeltaTime();
private:
	static double _secondsPerCount;

	static double _prevTime;
	static double _deltaTime;
};


