#pragma once

#include "GameObject.h"
#include "Player.h"
#include "Sprite.h"
#include "destructive_reasoning.h"

#define TIGER_NORMAL 0
#define TIGER_SIBERIAN 1
#define TIGER_SUMATRAN 2

class Main;

class Tiger : public GameObject
{
public:
	Tiger(SDL_Renderer* _renderer,std::vector<std::shared_ptr<CSprite>> _spriteVector,float X, float Y, float* _xOffset, float* _yOffset, Player* _player, const int tiger_type);
	~Tiger(void);

	void Update(void);
	void Render(void);

	void setRenderer(SDL_Renderer* _renderer);

	void switchSprites(void);

	void setSpriteVector(std::vector<std::shared_ptr<CSprite>> _spriteVector);

	double getAngle(void);

	bool onCollision(GameObject* object, float velX, float velY);
	bool onCollision(void);

	int getHealth(void);
	void Damage(int damage);

	static const float NORMAL_TIGER_SPEED_CONSTANT;
	static const float SIBERIAN_TIGER_SPEED;
	static const float SUMATRAN_TIGER_SPEED;

private:
	Player* player;
	float* xOffset;
	float* yOffset;

	SDL_Renderer* renderer;

	double angle;

	unsigned int animationSpeed;

	std::vector<std::shared_ptr<CSprite>> spriteVector;

	Uint32 last;
	Uint32 now;

	SDL_Rect squareRect;

	int health;

	SDL_Rect rect;

	bool col;

	const int BounceVel;
};

