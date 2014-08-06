#include "Tiger.h"
#include "Main.h"

Tiger::Tiger(SDL_Renderer* _renderer,std::vector<std::shared_ptr<CSprite>> _spriteVector,float X, float Y, float* _xOffset, float* _yOffset, Player* _player, const int tiger_type) : 
	GameObject(X,Y,_spriteVector[0].get()),
	xOffset(_xOffset),
	yOffset(_yOffset),
	player(_player),
	animationSpeed(250),
	renderer(_renderer),
	spriteVector(_spriteVector),
	BounceVel(4)
{
	switch(tiger_type)
	{
	case TIGER_NORMAL:
		rect.w = 16;
		rect.h = 16;
		health = 100;
		speed = Player::Default_Player_Speed * NORMAL_TIGER_SPEED_CONSTANT;
		break;
	case TIGER_SIBERIAN:
		rect.w = 12;
		rect.h = 12;
		health = 70;
		speed = Player::Default_Player_Speed * SIBERIAN_TIGER_SPEED;
		break;
	case TIGER_SUMATRAN:
		rect.w = 20;
		rect.h = 20;
		health = 130;
		speed = Player::Default_Player_Speed * SUMATRAN_TIGER_SPEED;
		break;
	default:
		printf("INVALID TIGER TYPE: %d\n",tiger_type);
		break;
	}

	last = SDL_GetTicks();
	now = SDL_GetTicks();
	rect.x = int(x);
	rect.y = int(y);
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

	velx = (float)-(cos(angle) * speed);
	vely = (float)-(sin(angle) * speed);

	bool colliding = false;
	for(unsigned int c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision(Main::stones[c],velx,0)) colliding = true;
	}
	if(!colliding) x += velx;
	else x -= signOfFloat(velx) * BounceVel;
	for(unsigned int c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision(Main::stones[c],0,vely)) colliding = true;
		if(onCollision(Main::stones[c],velx,vely)) col = true;
	}
	if(!colliding) y += vely;
	else y -= signOfFloat(vely) * BounceVel;
	squareRect.x = (int)x;
	squareRect.y = (int)y;
	//rect.x = x + sin(angle + M_PI/2) * sprite->getHeight();
	//rect.y = y - sin(angle + M_PI/2) * sprite->getWidth();
	centerX = x + sprite->getRect().w/2;
	centerY = y + sprite->getRect().h/2;
	rect.x = (int)centerX + (int)(cos(angle) - sin(angle));
	rect.y = (int)centerY + (int)(cos(angle) + sin(angle));
	//std::cout<<angle<<std::endl;
	//std::cout<<onCollision()<<std::endl;
}

void Tiger::Render(void)
{
	//std::cout<<"Rendering"<<std::endl;
	sprite->getRectByReference()->x = int(x) + (int)*xOffset;
	sprite->getRectByReference()->y = int(y) + (int)*yOffset;
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

const float Tiger::NORMAL_TIGER_SPEED_CONSTANT = 1.2f;
const float Tiger::SIBERIAN_TIGER_SPEED = 1.4f;
const float Tiger::SUMATRAN_TIGER_SPEED = 1.05f;