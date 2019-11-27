#pragma once
class Bomb
{
private:
	int _idxNum;

	double _x;
	double _y;

	double _aniSpeed;
	double _delTime;

	bool _isActive;
	bool _isOnPlayer;
public:
	Bomb();
	~Bomb();

	void Burst();

	void Init(const int x, const int y, const int idxNum);
	void Update(double deltaTime);
	void Render();
	void Release();

	const int GetIdx() const;
	const double GetX() const;
	const double GetY() const;
};

