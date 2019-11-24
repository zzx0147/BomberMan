#include "GameMap.h"

#include "SpritesLoader.h"
#include <cmath>

#define WALL 2
#define WALL_SPR_X 3
#define WALL_SPR_Y 3

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

const bool GameMap::IsMovePoint(const int px, const int py)
{
	double x = px / 48.0;
	double y = py / 48.0;

	if (_map[(int)floor(y)][(int)floor(x)])
		return false;

	if (_map[(int)ceil(y)][(int)ceil(x)])
		return false;

	if (_map[(int)floor(y)][(int)ceil(x)])
		return false;

	if (_map[(int)ceil(y)][(int)floor(x)])
		return false;
	return true;
}
