#pragma once

#include "Sprite.h"
#include "Screen.h"

class MainMenu
{
public:
	MainMenu(int width, int height);
	~MainMenu(void);

	void menuLoop(void);

private:
	bool running;

	int width;
	int height;

	int audioRate;
	Uint16 audioFormat;
	int audioChannels;
	int audioBuffers;

	Mix_Music* themeSong;

	int pawLocation;
	Uint32 start;
	Uint32 last;

	Screen* screen;

	CSprite* menuSprite;
	CSprite* paw;

	bool help;
};

