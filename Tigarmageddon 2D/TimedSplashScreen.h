#pragma once

#include "destructive_reasoning.h"
#include "Sprite.h"
#include "Screen.h"

class TimedSplashScreen
{
public:
	TimedSplashScreen(char* path, const int _width, const int _height, int millis);
	~TimedSplashScreen(void);

private:
	CSprite* sprite;
	Screen* screen;
	const int width;
	const int height;
	bool running;
};

