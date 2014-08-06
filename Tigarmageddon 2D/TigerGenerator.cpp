#include "TigerGenerator.h"
#include <time.h>
#include "Main.h"

TigerGenerator::TigerGenerator(float X, float Y, float *_xOffset, float *_yOffset, SDL_Renderer* _renderer, Player* _player) :
	GameObject(X,Y),
	renderer(_renderer),
	xOffset(_xOffset),
	yOffset(_yOffset),
	player(_player)
{
	sprite = new CSprite(renderer,"TigerGenerator.png",0,0,32,32);
	srand((unsigned int)time(NULL));
	normalvector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"tigerclosed.png",(int)x,(int)y,32,80)));
	normalvector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"tigerextended.png",(int)x,(int)y,32,80)));
	siberianvector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"siberianTigerExtended.png",(int)x,(int)y,28,72)));
	siberianvector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"siberianTigerClosed.png",(int)x,(int)y,28,72)));
	sumatranvector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"sumatranTigerExtended.png",(int)x,(int)y,36,88)));
	sumatranvector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"sumatranTigerClosed.png",(int)x,(int)y,36,88)));

	spritevector.push_back(normalvector);
	spritevector.push_back(siberianvector);
	spritevector.push_back(sumatranvector);

	pos.x = (int)X;
	pos.y = (int)Y;
}

void TigerGenerator::Update()
{
	if(rand()%Probability == Probability - 1)
	{
		//Spawn Tiger
		spawn();
	}
}

void TigerGenerator::spawn()
{
	if(Main::spawning)
	{
		int type = rand()%spritevector.size();
		Main::tigers.push_back(std::shared_ptr<Tiger>(new Tiger(renderer,spritevector[type],x,y,xOffset,yOffset,player,type)));
		Main::tigersSpawnedWave++;
	}
}

void TigerGenerator::Render()
{
	sprite->draw((int)x,(int)y,(int)*xOffset,(int)*yOffset);
}

TigerGenerator::~TigerGenerator(void)
{
	delete sprite;
}

const int TigerGenerator::Probability = 500;
