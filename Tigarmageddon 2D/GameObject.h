#pragma once
#include "destructive_reasoning.h"
#include "Sprite.h"
#include "Screen.h"

class GameObject
{
public:
	GameObject(float X, float Y, CSprite* csprite);
	GameObject(float X, float Y);
	~GameObject(void);

	virtual void Update();
	virtual void Render();

	float getX(void);
	float getY(void);
	float getVelX(void);
	float getVelY(void);
	float getCenterX(void);
	float getCenterY(void);
	float getSpeed(void);
	CSprite* getSprite(void);

	void setX(float X);
	void setY(float Y);
	void setPos(float X, float Y);
	void setVelX(float velX);
	void setVelY(float velY);
	void setVelocity(float velX, float velY);
	void setSprite(CSprite* sprite);
	void setSpeed(float speed);

	bool renderable;

protected:
	float x;
	float y;
	float velx;
	float vely;
	float centerX;
	float centerY;
	float speed;
	CSprite* sprite;
};

