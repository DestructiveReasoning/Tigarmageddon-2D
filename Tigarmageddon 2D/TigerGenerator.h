#pragma once

#include "destructive_reasoning.h"
#include "GameObject.h"
#include "Tiger.h"

#define POSITION_BUFFER 300

class Main;

typedef struct Position;

struct Position
{
	int x;
	int y;

	bool operator==(Position p)
	{
		if(abs(x - p.x) <= POSITION_BUFFER && abs(y - p.y) <= POSITION_BUFFER) return true;
		return false;
	}

	bool operator!=(Position p)
	{
		if(abs(x - p.x) <= POSITION_BUFFER && abs(y - p.y) <= POSITION_BUFFER) return false;
		return true;
	}
};

class TigerGenerator : public GameObject
{
public:
	TigerGenerator(float X, float Y, float *xOffset, float *yOffset, SDL_Renderer* _renderer, Player *_player);
	~TigerGenerator(void);

	void Render();
	void Update();

	Position pos;

	static const int Probability;

	std::vector<std::shared_ptr<CSprite>> normalvector;
	std::vector<std::shared_ptr<CSprite>> siberianvector;
	std::vector<std::shared_ptr<CSprite>> sumatranvector;

	std::vector<std::vector<std::shared_ptr<CSprite>>> spritevector;

private:
	SDL_Renderer* renderer;
	Player *player;
	float *xOffset;
	float *yOffset;
};

