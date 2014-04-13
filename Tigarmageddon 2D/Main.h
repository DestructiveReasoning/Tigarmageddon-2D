#pragma once

#include "destructive_reasoning.h"
#include "Screen.h"
#include "Sprite.h"
#include "Player.h"
#include "Tiger.h"
#include "Particle.h"

class Main
{
public:
	Main(int width, int height);
	~Main(void);

	void gameLoop(void);
	int getWidth(void);
	int getHeight(void);

	static int wave; //Initialized

	CSprite* bullet;

	void handleInput4(Screen* screen);
	void handlePauseMenuInput(void);

	void Start(void);

	static const int level_width; //No need to initialize
	static const int level_height; //No need to initialize
	static std::vector<GameObject*> stones; //Initialized
	
	static std::vector<std::shared_ptr<Tiger>> tigers; //Initialized
	std::vector<std::unique_ptr<Particle>> particles;

	std::vector<std::shared_ptr<CSprite>> normalTiger; //No need to initialize

	static int killcount; //Initialized

	static bool paused; //Initialized

	static const int ExplosionRadius;
	
	bool restart;

private:
	bool running;
	int w;
	int h;
	Uint32 start;
	CSprite* grassSprite;
	CSprite* playerSprite;
	CSprite* gradient;
	CSprite* paintBallGun;
	CSprite* glock;
	CSprite* skorpion;
	CSprite* gatlingGun;
	CSprite* spas;
	CSprite* emptySprite;
	CSprite* stoneSprite;
	CSprite* pauseMenuSprite;
	CSprite* c4Sprite;
	CSprite* ammoBoxSprite;

	//Map Stuff
	CSprite* land;

	Screen* screen;

	int mouseX;
	int mouseY;

	Player player;

	int xOffset;
	int yOffset;

	SDL_Surface* logoSurface;
	SDL_Surface* cursorSurface;
	SDL_Cursor* cursor;

	bool lmb;
	bool rmb;
	static const int AMOUNT_OF_KEYS;
	//bool keys[];
	const Uint8* state;

	int levelWidth;
	int levelHeight;

	const int number_of_stones;
	std::stringstream s;

	int pauseIndex;
	CSprite* paw;
	Uint32 now;
	Uint32 last;

	bool showControls;
};
