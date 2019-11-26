#pragma once
class Boom 
{
private:
	int _idxNum;

	double _x;
	double _y;

	double _aniSpeed = 0;
	double _delTime = 0;
public:
	Boom();
	~Boom();

	void Burst();

	void Init(const int x, const int y, const int idxNum);
	void Update(double deltaTime);
	void Render();
	void Release();

	const int GetIdx() const;
	const double GetX() const;
	const double GetY() const;
};

