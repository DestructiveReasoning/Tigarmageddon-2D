#include "MapMenu.h"
#include "Main.h"
#include "GameModeMenu.h"
#include <memory>
#include <time.h>

#define GAP_HEIGHT 66
#define STARTING_POINT_X 500 - 8
#define STARTING_POINT_Y 200 -32

MapMenu::MapMenu(int width, int height, int _gameMode) :
	Menu(width,height),
	back(false),
	field(false),
	siberia(false),
	random(false),
	gameMode(_gameMode)
{
	pawLocation = 0;
	menuSprite = new CSprite(screen->getRenderer(),"MapMenu.png",0,0,width,height);
	running = true;
}

void MapMenu::menuLoop()
{
	while(running && screen->getEvent()->type != SDL_QUIT)
	{
		start = SDL_GetTicks();
		screen->refresh();
		menuSprite->draw(0,0,0,0);

		if(screen->getEvent()->type == SDL_KEYDOWN)
		{
			switch(screen->getEvent()->key.keysym.sym)
			{
			case SDLK_ESCAPE:
				running = false;
				back = true;
				field = false;
				siberia = false;
				break;
			case SDLK_UP:
				movePaw(Up);
				break;
			case SDLK_DOWN:
				movePaw(Down);
				break;
			case SDLK_RETURN:
				switch(pawLocation)
				{
				case Main::Field:
					field = true;
					back = false;
					siberia = false;
					running = false;
					random = false;
					break;
				case Main::Siberia:
					siberia = true;
					field = false;
					running = false;
					back = false;
					random = false;
					break;
				case Main::EndMap:
					random = true;
					siberia = false;
					field = false;
					running = false;
					back = false;
					break;
				}
			default:
				break;
			}
		}

		paw->draw(STARTING_POINT_X,STARTING_POINT_Y + pawLocation * GAP_HEIGHT,0,0);
		if(1000/100 > SDL_GetTicks() - start) SDL_Delay(1000/100 - (SDL_GetTicks() - start));
		screen->display();
	}
}

void MapMenu::movePaw(int dir)
{
	if(SDL_GetTicks() - last >= 150)
	{
		switch(dir)
		{
		case Up:
			if(pawLocation == 0)
			{
				pawLocation = Main::EndMap;
			}
			else pawLocation--;
			break;
		case Down:
			if(pawLocation >= Main::EndMap) pawLocation = 0;
			else pawLocation++;
			break;
		default:
			break;
		}

		last = SDL_GetTicks();
	}
}

MapMenu::~MapMenu(void)
{
	srand(time(NULL));
	if(back)
	{
		std::unique_ptr<GameModeMenu> gm = std::unique_ptr<GameModeMenu>(new GameModeMenu(width,height));
		gm->menuLoop();
	}
	else if(field)
	{
		std::unique_ptr<Main> m = std::unique_ptr<Main>(new Main(width,height,Main::Field, gameMode));
		m->gameLoop();
	}
	else if(siberia)
	{
		std::unique_ptr<Main> m = std::unique_ptr<Main>(new Main(width,height,Main::Siberia, gameMode));
		m->gameLoop();
	}
	else if(random)
	{
		std::unique_ptr<Main> m = std::unique_ptr<Main>(new Main(width,height,rand()%(Main::EndMap), gameMode));
		m->gameLoop();
	}
}
