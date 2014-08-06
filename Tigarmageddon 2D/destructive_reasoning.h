#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <cmath>
#include <vector>
#include <string>
#include <SDL_mixer.h>
#include <memory>
#include <time.h>

#define POSITION_BUFFER 500

typedef struct Position;

struct Position
{
	int x;
	int y;

	bool operator==(Position p)
	{
		if(abs(x - p.x) <= POSITION_BUFFER && abs(y - p.y) <= POSITION_BUFFER) return true;
		return false;
	}

	bool operator!=(Position p)
	{
		if(abs(x - p.x) >= POSITION_BUFFER || abs(y - p.y) >= POSITION_BUFFER) return false;
		return true;
	}
};

static int signOfFloat(float f)
{
	int i = (int) f;
	(i >= 0) ? i = 1 : i = -1;
	return i;
}
