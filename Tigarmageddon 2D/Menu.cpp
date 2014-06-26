#include "Menu.h"

Menu::Menu(int width, int height) : 
	width(width),
	height(height),
	pawLocation(0)
{
	screen = new Screen(width,height,&running,true);
	paw = new CSprite(screen->getRenderer(),"tigerpaw.png",STARTING_POINT_X * width/800,STARTING_POINT_Y * height/600,40,40);
}

void Menu::menuLoop()
{
}

Menu::~Menu(void)
{
	delete paw;
	delete menuSprite;
	delete screen;
}
