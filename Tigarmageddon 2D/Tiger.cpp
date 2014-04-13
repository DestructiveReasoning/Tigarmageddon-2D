#include "Tiger.h"
#include "Main.h"

Tiger::Tiger(SDL_Renderer* _renderer,std::vector<std::shared_ptr<CSprite>> _spriteVector,float X, float Y, int* _xOffset, int* _yOffset, Player* _player) : 
	GameObject(X,Y,_spriteVector[0].get()),
	xOffset(_xOffset),
	yOffset(_yOffset),
	player(_player),
	animationSpeed(250),
	renderer(_renderer),
	spriteVector(_spriteVector)
{
	speed = Player::Default_Player_Speed;
	last = SDL_GetTicks();
	now = SDL_GetTicks();
	rect.x = int(x);
	rect.y = int(y);
	rect.w = 16;
	rect.h = 16;
	health = 100;
	col = false;
}

void Tiger::Update(void)
{
	col = false;
	now = SDL_GetTicks();
	if(now - last >= animationSpeed)
	{
		switchSprites();
		last = now;
	}

	double xcomponent, ycomponent;
	xcomponent = x + 16 - player->getCenterX();
	ycomponent = y + 40 - player->getCenterY();
	angle = atan2(ycomponent,xcomponent);
	//std::cout<<angle<<std::endl;

	velx = -(cos(angle) * speed);
	vely = -(sin(angle) * speed);

	bool colliding = false;
	for(int c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision(Main::stones[c],velx,0)) colliding = true;
	}
	if(!colliding) x += velx;
	for(int c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision(Main::stones[c],0,vely)) colliding = true;
		if(onCollision(Main::stones[c],velx,vely)) col = true;
	}
	if(!colliding) y += vely;
	squareRect.x = x;
	squareRect.y = y;
	//rect.x = x + sin(angle + M_PI/2) * sprite->getHeight();
	//rect.y = y - sin(angle + M_PI/2) * sprite->getWidth();
	centerX = x + sprite->getRect().w/2;
	centerY = y + sprite->getRect().h/2;
	rect.x = centerX + (cos(angle) - sin(angle));
	rect.y = centerY + (cos(angle) + sin(angle));
	//std::cout<<angle<<std::endl;
	//std::cout<<onCollision()<<std::endl;
}

void Tiger::Render(void)
{
	//std::cout<<"Rendering"<<std::endl;
	sprite->getRectByReference()->x = int(x) + *xOffset;
	sprite->getRectByReference()->y = int(y) + *yOffset;
	SDL_Point center = {16,40};
	SDL_RenderCopyEx(renderer,sprite->getTexture(),NULL,sprite->getRectByReference(),angle * 180/M_PI - 90,&center,SDL_FLIP_NONE);
}

bool Tiger::onCollision(GameObject* object, float velX, float velY)
{
	int signX;
	int signY;
	if(velX < 0) signX = -1;
	else signX = 1;
	if(velY < 0) signY = -1;
	else signY = 1;
	if(rect.x + velx > object->getX() + object->getSprite()->getWidth()){
		//col = false;
		return false;
	}
	if(rect.x + velx + rect.w * signX < object->getX()){
		//col = false;
		return false;
	}
	if(rect.y + vely > object->getY() + object->getSprite()->getHeight()){
		//col = false;
		return false;
	}
	if(rect.y + vely + rect.h * signY < object->getY()){
		//col = false;
		return false;
	}
	//std::cout<<"Colliding!!!"<<std::endl;
	col = true;
	return true;
}

bool Tiger::onCollision(void)
{
	return col;
}

void Tiger::Damage(int damage)
{
	if(!onCollision()) health -= damage;
}

int Tiger::getHealth(void)
{
	return health;
}

void Tiger::setSpriteVector(std::vector<std::shared_ptr<CSprite>> _spriteVector)
{
	spriteVector = _spriteVector;
	setSprite(spriteVector[0].get());
}

void Tiger::switchSprites(void)
{
	if(sprite == spriteVector[0].get())
	{
		sprite = spriteVector[1].get();
		return;
	}

	sprite = spriteVector[0].get();
}

double Tiger::getAngle(void)
{
	return angle;
}

Tiger::~Tiger(void)
{
}
