#pragma once
#include "destructive_reasoning.h"
#include "Sprite.h"
#include "GameObject.h"

#define TIME_BUFFER 500

class Player;

typedef struct{
	float x;
	float y;
} coordinate;

class C4 : public GameObject
{
public:
	C4(float X, float Y, float* _xOffset, float* _yOffset, Player* player, SDL_Renderer* renderer);
	~C4(void);

	void Update();
	void Detonate();
	void CheckExplosion();

	void setRenderer(SDL_Renderer* renderer);

	static CSprite c4sprite;

	bool render;

	bool ReadyToClick();

	void SetTimer(Uint32 time);

	static const int PARTICLE_SIZE;
	static const int AMOUNT_OF_PARTICLES;
	static const int PARTICLE_SPEED;
	static const int PARTICLE_LIFE;
	static const int EXPLOSION_RADIUS;

private:
	Uint32 timer;
	float* xOffset;
	float* yOffset;

	int exptime;

	Player* player;

	std::vector<SDL_Rect> particles;
	std::vector<coordinate> particleCoordinates;

	SDL_Renderer* renderer;

	Mix_Chunk* explosion;
};


