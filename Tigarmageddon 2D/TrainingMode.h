#pragma once

#include "SurvivalMode.h"

class TrainingMode : public SurvivalMode
{
public:
	TrainingMode(int width, int height, int levelWidth, int levelHeight, float* xOffset, float* yOffset, Player* player, Screen* screen);
	~TrainingMode(void);

	void update();

	enum Tips {Move,Shoot,Paintball,SelectGlock,SelectWeapons,DetonateC4,AmmoBoxes,TigerGenerators,DestroyTigerGenerator,Survival,Mercenary};

private:
	int step;
	int age;

	SDL_Renderer  *renderer;

	TTF_Font *font;
	SDL_Color fg;
	std::stringstream s;
	std::string message;
	SDL_Surface *instructionSurface;
	SDL_Rect rect;
	SDL_Rect rect2;
	SDL_Rect rect3;
	const int SmallFontSize;
	const int MediumFontSize;
	const int LargeFontSize;

	CSprite *survivalex;
	CSprite *mercenaryex;

	Uint32 start;
};

