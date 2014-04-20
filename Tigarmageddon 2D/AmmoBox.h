#pragma once

#include "GameObject.h"
#include "destructive_reasoning.h"

class AmmoBox : public GameObject
{
public:
	AmmoBox(float X, float Y, int* _xOffset, int* _yOffset, SDL_Renderer* renderer);
	~AmmoBox(void);

	void Render();

private:
	float x;
	float y;
	int* xOffset;
	int* yOffset;
	SDL_Renderer* renderer;
};

