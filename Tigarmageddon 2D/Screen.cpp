#include "Screen.h"


Screen::Screen(int width, int height, bool* running, bool border)
{
	screen = nullptr;
	if(border)screen = SDL_CreateWindow("Tigarmageddon 2D",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN|SDL_WINDOW_INPUT_GRABBED);
	else screen = SDL_CreateWindow("Tigarmageddon 2D",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN|SDL_WINDOW_BORDERLESS);
	if(screen == nullptr)
	{
		std:: cout << "Error creating Window" << std::endl;
		*running = false;
		return;
	}

	renderer = SDL_CreateRenderer(screen,-1,SDL_RENDERER_ACCELERATED);
	mainEvent = new SDL_Event();
}

void Screen::refresh(void)
{
	SDL_PollEvent(mainEvent);
	SDL_RenderClear(renderer);
}

void Screen::display(void)
{
	SDL_RenderPresent(renderer);
}

SDL_Window* Screen::getWindow(void)
{
	return screen;
}

SDL_Renderer* Screen::getRenderer(void)
{
	return renderer;
}

SDL_Event* Screen::getEvent(void)
{
	return mainEvent;
}

Screen::~Screen(void)
{
	delete mainEvent;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(screen);
}
