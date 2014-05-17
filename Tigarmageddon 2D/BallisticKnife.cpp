#include "BallisticKnife.h"
#include "Main.h"

BallisticKnife::BallisticKnife(float X, float Y, float velX, float velY, float* _xOffset, float* _yOffset,SDL_Renderer* renderer) :
	Bullet(X,Y,velX,velY,_xOffset,_yOffset,35,100,renderer),
	_sprite(renderer,"ballisticknife.png",X,Y,16,16),
	angle(atan2(velY,velX))
{
	destructible = false;
	sprite = _sprite;
	speed = BULLET_SPEED - 2;
}

void BallisticKnife::Update(void)
{
	if(onTheGround)
	{
		std::shared_ptr<CSprite> s = std::shared_ptr<CSprite>(new CSprite(sprite.getRenderer(), "ballisticknife.png",x,y,sprite.getWidth()*2,sprite.getHeight()*2));
		s->draw(x,y,*xOffset,*yOffset);
		return;
	}
	for(auto c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision2(Main::stones[c])) destroy = true;
	}
	x += velx;
	y += vely;
	age++;
	for(auto c = 0; c < Main::tigers.size(); c++)
	{
		if(onCollision(Main::tigers[c].get()))
		{
			destroy = true;
			Main::tigers[c]->Damage(damage);
		}
	}

	for(auto c = 0; c < Main::tgs.size(); c++)
	{
		if(onCollision(Main::tgs[c].get()))
		{
			destroy = true;
			Main::tgs[c] = Main::tgs.back();
			Main::tgs.pop_back();
			printf("Destroyed Tiger Generator\n");
		}
	}
}

void BallisticKnife::Render(void)
{
	SDL_Point center = {sprite.getWidth()/2,sprite.getHeight()/2};
	sprite.getRectByReference()->x = int(x) + *xOffset;
	sprite.getRectByReference()->y = int(y) + *yOffset;
	SDL_RenderCopyEx(sprite.getRenderer(),sprite.getTexture(),NULL,sprite.getRectByReference(),angle * 180/M_PI + 90,&center,SDL_FLIP_NONE);
}

BallisticKnife::~BallisticKnife(void)
{
}

const int BallisticKnife::Speed = BULLET_SPEED/2;