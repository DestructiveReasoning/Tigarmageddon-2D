#pragma once

#include "destructive_reasoning.h"
#include "Screen.h"

class Particle
{
public:
	Particle(float X, float Y, float velx, float vely, Screen* screen);
	~Particle(void);

	void Update();
	void Render();
	void Render2();

	static const int life;

	static const int Particle_Size;

	void setTimer(int t);

private:
	int timer;

	float x;
	float y;
	float velX;
	float velY;

	SDL_Surface* surface;
	SDL_Texture* texture;

	SDL_Renderer* renderer;
	SDL_Window* window;
};

