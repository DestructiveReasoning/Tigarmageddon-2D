#pragma once

#include "GameObject.h"
#include "Pacman.h"
#include "BallisticKnife.h"
#include "destructive_reasoning.h"
#include "Screen.h"
#include "Magazine.h"
#include "C4.h"
#include <SDL_ttf.h>
#include <sstream>
#include <memory>

class Main;

class Player : public GameObject
{
public:
	Player(float X, float Y, CSprite* csprite, float* _xOffset, float* _yOffset, int _gameWidth, int _gameHeight, CSprite* bulletSprite, int lvl);
	~Player(void);

	void Update();
	void Render(int _xOffset, int _yOffset);
	void move(float velX, float velY);
	void move2(float velX, float velY);

	void setScreen(Screen* _screen);
	void setRenderer(SDL_Renderer* _renderer);

	void Shoot(int mouseButton, float velX, float velY, Screen* screen);
	void PlaceC4(Screen* screen);
	void setWeapon(int w);
	void AddWeaponSprite(CSprite* weaponSprite);

	void cock(int mouseButton);

	//enum Weapon{PaintballGun,Glock,Skorpion,GattlingGun,Spas,Ballistic_Knife,Weapon_END};
	enum Weapon{PaintballGun,Skorpion,Ballistic_Knife,Spas,GattlingGun,PacmanGun,Glock,Weapon_END};

	void initHUD();
	void renderHUD();

	void initSprites(void);

	double toDegrees(double* angle);
	double toRadians(double* angle);

	void setDefaultBarrelPosition(float _defaultBarrelX, float _defaultBarrelY);
	float getBarrelX(void);
	float getBarrelY(void);

	bool onCollision(GameObject* object, float velX, float velY);
	bool onCollisionBullet(Bullet* object, float velX, float velY);

	const static float Default_Player_Speed;

	void init(SDL_Renderer* renderer);

	C4* getC4();
	bool hasC4;

	void collectAmmo(int c);

	bool isDead(void);
	void kill(void);

	int getWeapon();

	SDL_Renderer* getRenderer();

	bool hasFullAmmo();

private:
	float* xOffset;
	float* yOffset;
	int gameWidth;
	int gameHeight;

	float barrelX;
	float barrelY;

	float defaultBarrelX;
	float defaultBarrelY;

	double angle;

	Screen* screen;

	SDL_Renderer* renderer;

	std::vector<Bullet*> bullets;
	std::vector<BallisticKnife*> knives;
	Pacman *pacman;

	std::vector<Magazine> clips;
	std::vector<std::string> weaponNames;
	std::vector<CSprite*> weaponSprites;

	bool readyToShoot;
	bool readyToShoot3;

	Weapon weapon;
	C4* c4;

	TTF_Font* font;
	SDL_Color fg;
	std::stringstream s;
	SDL_Surface* ammoSurface;
	//SDL_Texture* ammoText;
	SDL_Rect ammoRect;
	SDL_Surface* weaponSurface;
	//SDL_Texture* weaponText;
	SDL_Rect weaponRect;
	//SDL_Surface* killedSurface;
	//SDL_Texture* killedText;
	//SDL_Rect killedRect;
	SDL_Surface* waveSurface;
	//SDL_Texture* waveText;
	SDL_Rect waveRect;

	//SDL_Texture* waveText;

	double timer;
	double lastTime;
	double now;

	int mouseX;
	int mouseY;

	//CSprite* paintBallGunSprite;
	//CSprite* bulletSprite;

	const int PaintBallGun_Damage;
	const int Glock_Damage;
	const int Skorpion_Damage;
	const int Spas_Damage;
	const int GattlingGun_Damage;

	//SOUND EFFECTS
	int audioRate;
	Uint16 audioFormat;
	int audioChannels;
	int audioBuffers;

	Mix_Chunk* gunshot;
	Mix_Chunk* spasShot;

	bool dead;
	bool hasPacman;
	int pacmanWave;

	CSprite *pacmanGun;
	CSprite *hand;
};

