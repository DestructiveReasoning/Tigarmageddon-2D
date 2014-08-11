#include "GameModeMenu.h"
#include "MapMenu.h"
#include <memory>

#define GAP_HEIGHT 66
#define STARTING_POINT_X 500 - 100
#define STARTING_POINT_Y 200 - 24

GameModeMenu::GameModeMenu(int width, int height) :
	Menu(width,height),
	mainMenu(false),
	survival(false),
	mercenary(false),
	training(false)
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
				mercenary = false;
				training = false;
				break;
			}

			else
			{
				switch(screen->getEvent()->key.keysym.sym)
				{
				case SDLK_DOWN:
					movePaw(Down);
					break;
				case SDLK_UP:
					movePaw(Up);
					break;
				case SDLK_RETURN:
					running = false;
					switch(pawLocation)
					{
					case Main::Survival:
						survival = true;
						mainMenu = false;
						mercenary = false;
						training = false;
						break;
					case Main::Mercenary:
						mercenary = true;
						mainMenu = false;
						survival = false;
						training = false;
						break;
					case Main::Training:
						training = true;
						mainMenu = false;
						survival = false;
						mercenary = false;
						break;
					default:
						printf("Invalid Option\n");
						mainMenu = true;
						mercenary = false;
						survival = false;
						training = false;
						break;
					}
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

void GameModeMenu::movePaw(int dir)
{
	if(SDL_GetTicks() - last >= 150)
	{
		switch(dir)
		{
		case Up:
			if(pawLocation == 0)
			{
				pawLocation = Main::EndMode - 1;
			}
			else pawLocation--;
			break;
		case Down:
			if(pawLocation >= Main::EndMode - 1) pawLocation = 0;
			else pawLocation++;
			break;
		default:
			break;
		}

		last = SDL_GetTicks();
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
	else if(mercenary)
	{
		std::unique_ptr<MapMenu> m = std::unique_ptr<MapMenu>(new MapMenu(width,height,Main::Mercenary));
		m->menuLoop();
	}
	else if(training)
	{
		std::unique_ptr<MapMenu> m = std::unique_ptr<MapMenu>(new MapMenu(width,height,Main::Training));
		m->menuLoop();
	}
}
