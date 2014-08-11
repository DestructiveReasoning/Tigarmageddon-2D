#pragma once

#include "SurvivalMode.h"

class MercenaryMode : public SurvivalMode
{
public:
	MercenaryMode(int width, int height, int levelWidth, int levelHeight, float* xOffset, float* yOffset, Player* player, Screen* screen);
	~MercenaryMode(void);

	void update();

	static std::vector<const int> AmmoPrices;
	static const int TigerGeneratorBounty;
	static const int TigerBounty;
};

