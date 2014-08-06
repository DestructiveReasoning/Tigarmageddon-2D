#pragma once

#include "destructive_reasoning.h";
#include "Bullet.h"

class Pacman : public Bullet
{
public:
	Pacman(float X, float Y, float velX, float velY, float* _xOffset, float* _yOffset, SDL_Renderer* renderer);
	~Pacman(void);

	void Render();
	void Update();

	const static float Speed;

private:
	CSprite *pacmanOpen;
	CSprite *pacmanClosed;

	float originalVelX;
	float originalVelY;

	unsigned int animationSpeed;
};

