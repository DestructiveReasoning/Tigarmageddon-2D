#pragma once

#include "Bullet.h"
#include "GameObject.h"

class BallisticKnife : public Bullet
{
public:
	BallisticKnife(float X, float Y, float velX, float velY, float* _xOffset, float* _yOffset,SDL_Renderer* renderer);
	~BallisticKnife(void);

	void Update(void);
	void Render(void);

	const static int Speed;

	bool hasFallen();
	void fall();

private:
	CSprite _sprite;
	double angle;
	bool onTheGround;
};

