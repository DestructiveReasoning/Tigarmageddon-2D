#pragma once

#include "GameObject.h"
#include "destructive_reasoning.h"

class AmmoBox : public GameObject
{
public:
	AmmoBox(float X, float Y, float* _xOffset, float* _yOffset, SDL_Renderer* renderer);
	~AmmoBox(void);

	void Render();

private:
	float x;
	float y;
	float* xOffset;
	float* yOffset;
	SDL_Renderer* renderer;
};

