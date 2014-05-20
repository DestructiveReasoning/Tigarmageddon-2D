#include <SDL.h>
#include <SDL_image.h>
#include "destructive_reasoning.h"
#include "Main.h"
#include "MainMenu.h"
#include "TimedSplashScreen.h"

#define WIDTH 800
#define HEIGHT (WIDTH * 3/4)
#define endl std::endl

/*
 Tigarmageddon 2D
 Created By Harley Wiltzer
 Powered by Har Wiltz's Destructive Reasoning
 Using SDL2

 Started: Somewhere in Time (In late 2013)
 NOTE: School is time-consuming.

 I don't care what anyone tells you, there is absolutely no warranty.

 NOTES:
	Do not delete Sprites in the GameObject destructor
	Create Sprites in the Main constructor, and delete them in the Main destructor

	Grass terrain ends at terrain width - (screen width)/2 - (player width)/2

	CMath Trig Functions are in Radians
	*SDL Angles are in Degrees

	Getcha Pull
*/

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TimedSplashScreen* splash = new TimedSplashScreen("PoweredByDestructiveReasoning.png",640,480,5000);
	delete splash;
	MainMenu* mainMenu = new MainMenu(WIDTH,HEIGHT);
	mainMenu->menuLoop();
	delete mainMenu;
	//delete main;
	SDL_Quit();
	exit(0);
	//system("PAUSE");
	return 0;
}