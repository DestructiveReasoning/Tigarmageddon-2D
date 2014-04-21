#include "Bullet.h"
#include "Main.h"

Bullet::Bullet(float X, float Y, float velX, float velY, float* _xOffset, float* _yOffset, int _range, int _damage, SDL_Renderer* renderer) : 
	GameObject(X,Y), 
	xOffset(_xOffset), 
	yOffset(_yOffset),
	range(_range),
	age(0),
	damage(_damage),
	sprite(renderer,"bullet8.png",int(X),int(Y),8,8),
	onTheGround(false)
{
	velx = velX;
	vely = velY;
	//setSprite(&sprite);
	setSpeed(BULLET_SPEED);
	destructible = true;
	destroy = false;
}

void Bullet::Update()
{
		for(int c = 0; c < Main::stones.size(); c++)
		{
			if(onCollision2(Main::stones[c])) destroy = true;
	    }
		x += velx;
		y += vely;
		age++;
		for(int c = 0; c < Main::tigers.size(); c++)
		{
			if(onCollision(Main::tigers[c].get()))
			{
				destroy = true;
				Main::tigers[c]->Damage(damage);
			}
		}
}

bool Bullet::onCollision(GameObject* object)
{
	float velX, velY;
	int signX, signY;
	velX = abs(velx);
	velY = abs(vely);
	if(velX < 0) signX = -1;
	else signX = 1;
	if(velY < 0) signY = -1;
	else signY = 1;
	for(int c = 0; c < int(velX); c++)
	{
		for(int j = 0; j < int(velY); j++)
		{
			if(((x + sprite.getWidth() + signX * c) >= object->getX()) && ((x + c * signX) < (object->getX() + object->getSprite()->getWidth())))
			{
				if(((y + sprite.getHeight() + signY * j) >= object->getY()) && ((y + j * signY) < (object->getY() + object->getSprite()->getHeight())))
				{
					velx = signX * c;
					vely = signY * j;
					return true;
				}
			}
		}
	}
	return false;
}

bool Bullet::onCollision2(GameObject* object)
{
	if(((x + sprite.getWidth() + velx) >= object->getX()) && ((x + velx) < (object->getX() + object->getSprite()->getWidth())))
	{
		if(((y + sprite.getHeight() + vely) >= object->getY()) && ((y + vely) < (object->getY() + object->getSprite()->getHeight())))
		{
			return true;
		}
	}
	return false;
}

void Bullet::Render()
{
	sprite.getRectByReference()->x = int(x);
	sprite.getRectByReference()->y = int(y);
	sprite.draw(int(x),int(y),*xOffset,*yOffset);
}

CSprite Bullet::getProjectileSprite(void)
{
	return sprite;
}

int Bullet::getRange(void)
{
	return range;
}

int Bullet::getAge()
{
	return age;
}

Bullet::~Bullet(void)
{
	std::cout<<"Deleting bullet..." << std::endl;
	//delete xOffset;
	//delete yOffset;
	//delete &sprite;
	std::cout<<"Done."<<std::endl;
}
