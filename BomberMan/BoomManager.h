#pragma once
class BoomManager
{
private:
	static int _count;
	static const int _maxCount = 30;
	static class Boom* _booms;// [_maxCount];
public:
	BoomManager();
	~BoomManager();

	static void CreateBoom(const int x, const int y);
	static void DeleteBoom(const int idx);

	static void Init();
	static void Update(double deltaTime);
	static void Render();
	static void Release();
};

