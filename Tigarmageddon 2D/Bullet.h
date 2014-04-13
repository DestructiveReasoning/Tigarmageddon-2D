#pragma once
#include "GameObject.h"

#define BULLET_SPEED 20

class Main;

class Bullet : public GameObject
{
public:
	Bullet(float X, float Y, float velX, float velY, int* _xOffset, int* _yOffset, int range, int damage, SDL_Renderer* renderer);
	virtual void Update();
	void Render();
	int getRange(void);
	int getAge(void);

	bool onCollision(GameObject* object);
	bool onCollision2(GameObject* object);
	
	bool destroy;

	bool destructible;
	bool onTheGround;

	CSprite getProjectileSprite(void);

	~Bullet(void);

private:
	CSprite* bulletSprite;

protected:
	CSprite sprite;
	int age;
	int range;
	int damage;
	int* xOffset;
	int* yOffset;
};

