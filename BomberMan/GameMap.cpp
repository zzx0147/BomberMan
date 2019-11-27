#include "GameMap.h"

#include "SpritesLoader.h"
#include <cmath>

#define WALL 2
#define WALL_SPR_X 3
#define WALL_SPR_Y 3

#define BROKEWALL_SPR_X 4
#define BROKEWALL_SPR_Y 3

int GameMap::_map[15][15];
const int GameMap::_width;
const int GameMap::_height;

void GameMap::Init()
{
	for (int i = 0; i < _width; ++i)
	{
		_map[0][i] = WALL;
		_map[_height - 1][i] = WALL;
	}

	for (int i = 0; i < _height; ++i)
	{
		_map[i][0] = WALL;
		_map[i][_width - 1] = WALL;
	}

	for (int i = 2; i < _height; i += 2)
	{
		for (int j = 2; j < _width; j += 2)
		{
			_map[i][j] = WALL;
		}
	}
}

void GameMap::Update()
{
	for (int i = 0; i < _height; ++i)
	{
		for (int j = 0; j < _width; ++j)
		{
			_map[i][j] = 0;
		}
	}

	Init();
}

void GameMap::Render() 
{
	for (int i = 0; i < _height; ++i)
	{
		for (int j = 0; j < _width; ++j)
		{
			switch (_map[i][j])
			{
				case WALL:
					SpritesLoader::DrawSprite(i * 48, j * 48, WALL_SPR_X, WALL_SPR_Y);
					break;
			}
		}
	}
}

void GameMap::SetBlock(const double x, const double y)
{
	int px = round(x / 48);
	int py = round(y / 48);
	if (_width <= px || _height <= py) return;

	_map[py][px] = 1;
}

void GameMap::RemoveBlock(const double x, const double y)
{
	int px = round(x / 48);
	int py = round(y / 48);
	if (_width <= px || _height <= py) return;

	_map[py][px] = 0;
}

const bool GameMap::IsMovePoint(const double x, const double y)
{
	double px = x / 48;
	double py = y / 48;

	if (_map[(int)floor(py)][(int)floor(px)]) return false;
	if (_map[(int) ceil(py)][(int) ceil(px)]) return false;
	if (_map[(int)floor(py)][(int) ceil(px)]) return false;
	if (_map[(int) ceil(py)][(int)floor(px)]) return false;

	return true;
}
