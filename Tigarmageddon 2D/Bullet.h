#pragma once
#include "GameObject.h"

#define BULLET_SPEED 20

class Main;

class Bullet : public GameObject
{
public:
	Bullet(float X, float Y, float velX, float velY, float* _xOffset, float* _yOffset, int range, int damage, SDL_Renderer* renderer);
	virtual void Update();
	virtual void Render();
	int getRange(void);
	int getAge(void);

	bool onCollision(GameObject* object);
	bool onCollision2(GameObject* object);
	
	bool destroy;

	bool destructible;

	CSprite getProjectileSprite(void);

	~Bullet(void);

protected:
	CSprite sprite;
	int age;
	int range;
	int damage;
	float* xOffset;
	float* yOffset;
};

