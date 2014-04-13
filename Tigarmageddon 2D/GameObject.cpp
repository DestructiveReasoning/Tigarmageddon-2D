#include "GameObject.h"

GameObject::GameObject(float X, float Y, CSprite* csprite)
{
	x = X;
	y = Y;
	velx = 0;
	vely = 0;
	sprite = csprite;
	if(csprite == nullptr) renderable = false;
	else renderable = true;
	if(renderable)
	{
		centerX = x + sprite->getRect().w/2;
		centerY = y + sprite->getRect().h/2;
	}else
	{
		centerX = centerY = -1;
	}
}

GameObject::GameObject(float X, float Y)
{
	x = X;
	y = Y;
	velx = 0;
	vely = 0;
	renderable = false;
}

void GameObject::Update()
{
	x += velx;
	y += vely;
}

void GameObject::Render()
{
}

float GameObject::getX(void)
{
	return x;
}

float GameObject::getY(void)
{
	return y;
}

float GameObject::getVelX(void)
{
	return velx;
}

float GameObject::getVelY(void)
{
	return vely;
}

float GameObject::getSpeed(void)
{
	return speed;
}

float GameObject::getCenterX(void)
{
	return centerX;
}

float GameObject::getCenterY(void)
{
	return centerY;
}

CSprite* GameObject::getSprite(void)
{
	return sprite;
}

void GameObject::setX(float X)
{
	x = X;
}

void GameObject::setY(float Y)
{
	y = Y;
}

void GameObject::setPos(float X, float Y)
{
	x = X;
	y = Y;
}

void GameObject::setVelX(float velX)
{
	velx = velX;
}

void GameObject::setVelY(float velY)
{
	vely = velY;
}

void GameObject::setVelocity(float velX, float velY)
{
	velx = velX;
	vely = velY;
}

void GameObject::setSpeed(float s)
{
	speed = s;
}

void GameObject::setSprite(CSprite* csprite)
{
	sprite = csprite;
	renderable = true;
	centerX = x + sprite->getRect().w/2;
	centerY = y + sprite->getRect().h/2;
}

GameObject::~GameObject(void)
{
	//delete sprite;
}
