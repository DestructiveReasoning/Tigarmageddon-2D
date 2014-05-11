#pragma once

#include "destructive_reasoning.h"
#include "GameObject.h"
#include "Tiger.h"

class Main;

class TigerGenerator : public GameObject
{
public:
	TigerGenerator(float X, float Y, float *xOffset, float *yOffset, SDL_Renderer* _renderer, Player *_player);
	~TigerGenerator(void);

	void Render();
	void Update();

	static const int Probability;

	std::vector<std::shared_ptr<CSprite>> spritevector;

private:
	SDL_Renderer* renderer;
	Player *player;
	float *xOffset;
	float *yOffset;
};

