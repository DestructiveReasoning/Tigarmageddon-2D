#pragma once

#include "destructive_reasoning.h"
#include "Sprite.h"
#include <memory>
#include <sstream>

class GameMode
{
public:
	GameMode(void);
	~GameMode(void);

	virtual void update();

	Uint32 last;

	static std::vector<Mix_Music*> playList;

protected:
	std::stringstream s;
};
