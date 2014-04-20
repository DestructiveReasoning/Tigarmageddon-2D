#include "AmmoBox.h"


AmmoBox::AmmoBox(float X, float Y, int* _xOffset, int* _yOffset, SDL_Renderer* _renderer) :
	GameObject(X,Y,new CSprite(_renderer,"AmmoBox.png",X,Y,32,32)),
	x(X),
	y(Y),
	xOffset(_xOffset),
	yOffset(_yOffset),
	renderer(_renderer)
{
}

void AmmoBox::Render()
{
	sprite->draw(x,y,*xOffset,*yOffset);
}

AmmoBox::~AmmoBox(void)
{
	delete sprite;
}
