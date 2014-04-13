#pragma once

#include "GameObject.h"
#include "Player.h"
#include "Sprite.h"
#include "destructive_reasoning.h"

class Main;

class Tiger : public GameObject
{
public:
	Tiger(SDL_Renderer* _renderer,std::vector<std::shared_ptr<CSprite>> _spriteVector,float X, float Y, int* _xOffset, int* _yOffset, Player* _player);
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

private:
	Player* player;
	int* xOffset;
	int* yOffset;

	SDL_Renderer* renderer;

	double angle;

	int animationSpeed;

	std::vector<std::shared_ptr<CSprite>> spriteVector;

	Uint32 last;
	Uint32 now;

	SDL_Rect squareRect;

	int health;

	SDL_Rect rect;

	bool col;
};

