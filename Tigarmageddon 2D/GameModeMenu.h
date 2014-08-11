#pragma once

#include "destructive_reasoning.h"
#include "Menu.h"
#include "Main.h"
#include "MapMenu.h"

class GameModeMenu : public Menu
{
public:
	GameModeMenu(int width, int height);
	~GameModeMenu(void);

	void menuLoop();
	void movePaw(int dir);

	enum Directions {Up,Down};

private:
	bool mainMenu;
	bool survival;
	bool mercenary;
	bool training;
};

