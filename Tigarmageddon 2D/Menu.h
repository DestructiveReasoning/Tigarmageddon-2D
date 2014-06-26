#pragma once

#include "destructive_reasoning.h"
#include "Sprite.h"
#include "Screen.h"
#include "MainMenu.h"

#define STARTING_POINT_X 300
#define STARTING_POINT_Y 300

class Menu
{
public:
	Menu(int width, int height);
	virtual ~Menu(void);

	virtual void menuLoop();

protected:
	bool running;
	const int width;
	const int height;
	
	int pawLocation;
	Uint32 start;
	Uint32 last;
	CSprite* menuSprite;
	CSprite* paw;
	Screen* screen;
};

