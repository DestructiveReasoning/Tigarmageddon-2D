#include "GameModeMenu.h"
#include "MapMenu.h"
#include <memory>

#define GAP_HEIGHT 32
#define STARTING_POINT_X 500 - 40
#define STARTING_POINT_Y 200 - 24

GameModeMenu::GameModeMenu(int width, int height) :
	Menu(width,height),
	mainMenu(false),
	survival(false)
{
	menuSprite = new CSprite(screen->getRenderer(),"GameModeMenu.png",0,0,width,height);
	running = true;
}

void GameModeMenu::menuLoop()
{
	while(running && screen->getEvent()->type != SDL_QUIT)
	{
		//printf("Looping\n");
		start = SDL_GetTicks();
		screen->refresh();
		menuSprite->draw(0,0,0,0);
		paw->draw(STARTING_POINT_X,STARTING_POINT_Y + pawLocation * GAP_HEIGHT,0,0);

		if(screen->getEvent()->type == SDL_KEYDOWN)
		{
			if(screen->getEvent()->key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;

				mainMenu = true;
				survival = false;
				break;
			}

			else
			{
				switch(screen->getEvent()->key.keysym.sym)
				{
				case SDLK_RETURN:
					running = false;
					mainMenu = false;
					survival = true;
					break;
				default:
					break;
				}
			}
		}

		if(1000/100 > SDL_GetTicks() - start) SDL_Delay(1000/100 - (SDL_GetTicks() - start));
		screen->display();
	}
}

GameModeMenu::~GameModeMenu(void)
{
	if(mainMenu)
	{
		std::unique_ptr<MainMenu> m = std::unique_ptr<MainMenu>(new MainMenu(width,height));
		m->menuLoop();
	}
	else if(survival)
	{
		std::unique_ptr<MapMenu> m = std::unique_ptr<MapMenu>(new MapMenu(width,height,Main::Survival));
		m->menuLoop();
	}
}
