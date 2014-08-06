#pragma once
#include "destructive_reasoning.h"

class CSprite
{
public:
	CSprite(SDL_Renderer* render, std::string path, int x, int y, int w, int h);
	~CSprite(void);
	void draw(int x, int y, int _xOffset, int _yOffset);
	void setX(float x);
	void setY(float y);
	void setPos(float x, float y);
	float getX();
	float getY();
	int getCenterX(void);
	int getCenterY(void);
	int getWidth(void);
	int getHeight(void);
	SDL_Rect getRect(void);
	SDL_Rect* getRectByReference(void);
	SDL_Texture* getTexture(void);
	SDL_Renderer* getRenderer(void);

	bool operator==(CSprite *s);

private:
	SDL_Texture* texture;
	SDL_Rect rect;
	float x;
	float y;
	int centerX;
	int centerY;
	int width;
	int height;
	int speed;
	SDL_Renderer* renderer;
	std::string path;
};

