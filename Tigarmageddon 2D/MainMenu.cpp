#include "MainMenu.h"
#include "Main.h"

#define STARTINGPOINT_X 640
#define STARTINGPOINT_Y 427

MainMenu::MainMenu(int _width, int _height)
{
	screen = new Screen(_width,_height,&running);
	width = _width;
	height = _height;

	menuSprite = new CSprite(screen->getRenderer(),"tigarmageddon2Dmenu.png",0,0,width,height);
	paw = new CSprite(screen->getRenderer(),"tigerpaw.png",STARTINGPOINT_X,STARTINGPOINT_Y,40,40);

	running = true;
	pawLocation = 0;
	std::cout<<"New Main Menu" << std::endl;
}

void MainMenu::menuLoop(void)
{
	while(running && screen->getEvent()->type != SDL_QUIT)
	{
		start = SDL_GetTicks();
		screen->refresh();
		switch(screen->getEvent()->type)
		{
		case SDL_QUIT:
			exit(0);
			return;
		case SDL_KEYDOWN:
			switch(screen->getEvent()->key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;
			case SDLK_UP:
				if(start - last > 125)
				{
					pawLocation--;
					if(pawLocation < 0)
					{
						pawLocation = 2;
					}
					last = start;
				}
				break;
			case SDLK_DOWN:
				if(start - last > 125)
				{
					pawLocation++;
					if(pawLocation > 2)
					{
						pawLocation = 0; 
					}
					last = start;
				}
				break;
			case SDLK_RETURN:
				switch(pawLocation)
				{
				case 0:
					running = false;
					return;
				case 2:
					exit(0);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		menuSprite->draw(0,0,0,0);
		paw->draw(paw->getX() - 40,paw->getY() + (pawLocation * 60),0,0);
		if(1000/100 > SDL_GetTicks() - start) SDL_Delay(1000/100 - (SDL_GetTicks() - start));
		screen->display();
	}
}


MainMenu::~MainMenu(void)
{
	delete screen;
	delete menuSprite;
	delete paw;
	std::unique_ptr<Main> main = std::unique_ptr<Main>(new Main(width,height));
	main->gameLoop();
}