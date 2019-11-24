#include "GameMap.h"

#include "SpritesLoader.h"

#define WALL 2
#define WALL_SPR_X 3
#define WALL_SPR_Y 3

GameMap::GameMap()
{
}

GameMap::~GameMap()
{
}

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

void GameMap::Render(HINSTANCE hInstance, HDC hdc)
{
	for (int i = 0; i < _height; ++i)
	{
		for (int j = 0; j < _width; ++j)
		{
			switch (_map[i][j])
			{
				case WALL:
					SpritesLoader::DrawSprite(hdc, hInstance, i * 48, j * 48, WALL_SPR_X, WALL_SPR_Y);
					break;
			}
		}
	}
}
