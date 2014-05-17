#include "TimedSplashScreen.h"


TimedSplashScreen::TimedSplashScreen(char* path, const int _width, const int _height, int millis) :
	width(_width),
	height(_height),
	running(true)
{
	screen = new Screen(width,height,&running,false);
	//screen->getWindow() = SDL_CreateWindow("",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_BORDERLESS);
	screen->refresh();
	sprite = new CSprite(screen->getRenderer(),path,0,0,width,height);
	sprite->draw(0,0,0,0);
	SDL_RenderPresent(screen->getRenderer());
	SDL_Delay(millis);
	running = false;
}


TimedSplashScreen::~TimedSplashScreen(void)
{
	delete sprite;
	delete screen;
}
