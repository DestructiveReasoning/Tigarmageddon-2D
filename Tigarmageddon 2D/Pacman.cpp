#include "Pacman.h"
#include "Main.h"

Pacman::Pacman(float X, float Y, float velX, float velY, float* _xOffset, float* _yOffset, SDL_Renderer* renderer) :
	Bullet(X,Y,velX,velY,_xOffset,_yOffset,1000000,1000000,renderer),
	originalVelX(velX),
	originalVelY(velY),
	animationSpeed(40)
{
	pacmanOpen = new CSprite(renderer,"pacmanOpen.png",0,0,24,24);
	pacmanClosed = new CSprite(renderer,"pacmanClosed.png",0,0,24,24);
	sprite = *pacmanOpen;
}

void Pacman::Update()
{
		velx = originalVelX;
		vely = originalVelY;
		for(int c = 0; c < Main::stones.size(); c++)
		{
			if(onCollision2(Main::stones[c])) destroy = true;
	    }
		x += velx;
		y += vely;
		age++;
		for(int c = 0; c < Main::tigers.size(); c++)
		{
			if(onCollision2(Main::tigers[c].get()))
			{
				Main::tigers[c]->Damage(damage);
			}
		}
}

void Pacman::Render()
{
	if(age%animationSpeed == 0) (sprite == pacmanOpen) ? sprite = *pacmanClosed : sprite = *pacmanOpen;
	sprite.getRectByReference()->x = int(x);
	sprite.getRectByReference()->y = int(y);
	sprite.draw(int(x),int(y),*xOffset,*yOffset);
}

Pacman::~Pacman(void)
{
	delete pacmanOpen;
	delete pacmanClosed;
}

const float Pacman::Speed = BallisticKnife::Speed/2;
