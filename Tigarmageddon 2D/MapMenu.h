#pragma once

#include "destructive_reasoning.h"
#include "Menu.h"

class MapMenu : public Menu
{
public:
	MapMenu(int width, int height, int GameMode);
	~MapMenu(void);

	void menuLoop();
	void movePaw(int dir);

	enum Direction{Up,Down};

private:
	bool back;
	bool field;
	bool siberia;
	bool random;

	int gameMode;
};

