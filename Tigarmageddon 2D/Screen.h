#pragma once

#include "destructive_reasoning.h"

class Screen
{
public:
	Screen(int width, int height, bool* running,bool border);
	~Screen(void);
	SDL_Window* getWindow(void);
	SDL_Renderer* getRenderer(void);
	SDL_Event* getEvent(void);
	void refresh(void);
	void display(void);

private:

	SDL_Renderer* renderer;
	SDL_Event* mainEvent;
	SDL_Window* screen;
};

