#pragma once

#include "GameMode.h"
#include "Main.h"
#include "Screen.h"

class SurvivalMode : public GameMode
{
public:
	SurvivalMode(int width, int height, int levelWidth, int levelHeight, float* xOffset, float* yOffset, Player* player, Screen* screen);
	~SurvivalMode(void);

	void update();

	std::vector<std::unique_ptr<CSprite>> numbers;

private:
	void nextWave();

	unsigned int maxAlive;
	unsigned int maxWave;
	unsigned int killquota;
	unsigned int minGenerators;
	unsigned int waveGenerators;
	int counter;
	int width;
	int height;
	int levelWidth;
	int levelHeight;
	bool draw;

	float* xOffset;
	float* yOffset;

	Player* player;

	Screen* screen;
};

