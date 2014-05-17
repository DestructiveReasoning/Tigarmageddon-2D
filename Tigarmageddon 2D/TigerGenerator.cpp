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
	srand(time(NULL));
	spritevector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"tigerclosed.png",x,y,32,80)));
	spritevector.push_back(std::shared_ptr<CSprite>(new CSprite(renderer,"tigerextended.png",x,y,32,80)));

	pos.x = X;
	pos.y = Y;
}

void TigerGenerator::Update()
{
	if(rand()%Probability == Probability - 1)
	{
		//Spawn Tiger
		Main::tigers.push_back(std::shared_ptr<Tiger>(new Tiger(renderer,spritevector,x,y,xOffset,yOffset,player)));
	}
}

void TigerGenerator::Render()
{
	sprite->draw(x,y,*xOffset,*yOffset);
}

TigerGenerator::~TigerGenerator(void)
{
	delete sprite;
}

const int TigerGenerator::Probability = 500;
