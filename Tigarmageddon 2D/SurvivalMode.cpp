#include "SurvivalMode.h"


SurvivalMode::SurvivalMode(int _width, int _height, int _levelWidth, int _levelHeight, float* _xOffset, float* _yOffset, Player* _player, Screen* screen) :
	maxAlive(0),
	killquota(0),
	counter(0),
	maxWave(0),
	minGenerators(0),
	waveGenerators(0),
	width(_width),
	height(_height),
	levelWidth(_levelWidth),
	levelHeight(_levelHeight),
	screen(screen),
	player(_player),
	xOffset(_xOffset),
	yOffset(_yOffset),
	draw(false)
{

	last = 0;

	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font10.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font9.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font8.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font7.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font6.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font5.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font4.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font3.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font2.png",0,0,128,128)));
	numbers.push_back(std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"font1.png",0,0,128,128)));
	nextWave();
}

void SurvivalMode::update()
{
	if(Main::tigers.size() >= maxAlive || Main::tigersSpawnedWave >= maxWave) Main::spawning = false;
	else Main::spawning = true;

	if(Main::tigersKilledWave >= killquota) nextWave();
	if(Main::tigers.size() == 0 && Main::tgs.size() == 0) nextWave();

	if(draw && counter < 10)
		numbers[counter]->draw(width/2 - 64,64,0,0);

	if(Mix_PlayingMusic() == 0 && Main::playMusic == true)
	{
		Mix_PlayMusic(GameMode::playList[rand()%GameMode::playList.size()],0);
	}
}

void SurvivalMode::nextWave()
{
	draw = true;
	Main::spawning = false;
	if(counter < 10)
	{
		if(SDL_GetTicks() - last >= 1000 || last == 0)
		{
			//numbers[counter]->draw(346,64,0,0);
			counter++;
			last = SDL_GetTicks();
		}
		return;
	}
	Main::tigersKilledWave = 0;
	Main::tigersSpawnedWave = 0;
	maxAlive = Main::wave * 5 + 5;
	killquota = Main::wave * 5 + 10;
	maxWave = killquota;
	Main::wave++;
	waveGenerators = (Main::wave/3) + 4;

	while(Main::tgs.size() < waveGenerators)
	{
		Position p = {rand()%levelWidth, rand()%levelHeight};
		for(unsigned int j = 0; j < Main::tgs.size(); j++)
		{
			do
			{
				p.x = rand()%(levelWidth-32*3) + 32;
				p.y = rand()%(levelHeight-32*3) + 32;
				for(unsigned int q = 0; q < Main::stones.size(); q++)
				{
					Position pq = {(int)Main::stones[q]->getX(),(int)Main::stones[q]->getY()};
					while(p == pq)
					{
						p.x = rand()%(levelWidth-32*3) + 32;
					p.y = rand()%(levelHeight-32*3) + 32;
					}
				}
			}
			while((p == Main::tgs[j].get()->pos));
		}
		if(Main::tgs.size() == 0)
		{
			for(unsigned int q = 0; q < Main::stones.size(); q++)
			{
				Position pq = {(int)Main::stones[q]->getX(), (int)Main::stones[q]->getY()};
				while(p == pq)
				{
					p.x = rand()%(levelWidth-32*3) + 32;
					p.y = rand()%(levelHeight-32*3) + 32;
				}
			}
		}
		Main::tgs.push_back(std::shared_ptr<TigerGenerator>(new TigerGenerator((float)p.x,(float)p.y,xOffset,yOffset,screen->getRenderer(), player)));
		printf("Added Tiger Generator\ntgs size: %d\n", Main::tgs.size());
	}

	printf("Max TGS: %d\n",waveGenerators);
	counter = 0;
	draw = false;
}

SurvivalMode::~SurvivalMode(void)
{
}
