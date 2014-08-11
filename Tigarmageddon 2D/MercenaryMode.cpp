#include "MercenaryMode.h"


MercenaryMode::MercenaryMode(int width, int height, int levelWidth, int levelHeight, float* xOffset, float* yOffset, Player* player, Screen* screen) :
	SurvivalMode(width,height,levelWidth,levelHeight,xOffset,yOffset,player,screen)
{
	for(int c = 0; c < Player::Weapon_END; c++)
	{
		AmmoPrices.push_back(0);
	}

	AmmoPrices[Player::PaintballGun] = 20;
	AmmoPrices[Player::Skorpion] = 20;
	AmmoPrices[Player::Ballistic_Knife] = 10;
	AmmoPrices[Player::GattlingGun] = 60;
	AmmoPrices[Player::Glock] = 0;
	AmmoPrices[Player::Spas] = 30;
}

void MercenaryMode::update()
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

	s.str(std::string());
	s.clear();
	s<<"Currency: "<< Main::points << " | " << "Tigers Killed: " << Main::killcount;
	SDL_SetWindowTitle(screen->getWindow(),s.str().c_str());
}

MercenaryMode::~MercenaryMode(void)
{
}

std::vector<const int> MercenaryMode::AmmoPrices;
const int MercenaryMode::TigerGeneratorBounty	= 10;
const int MercenaryMode::TigerBounty			= 1;