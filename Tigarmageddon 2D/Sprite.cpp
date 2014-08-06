#include "Sprite.h"

CSprite::CSprite(SDL_Renderer* render, std::string path, int xp, int yp, int w, int h)
{
	this->path = path;
	renderer = render;
	x = xp;
	y = yp;
	width = w;
	height = h;
	texture = nullptr;
	texture = IMG_LoadTexture(render,path.c_str());
	if(texture == nullptr)
	{
		std::cout<<"Error creating sprite texture. Make sure the following are in this game's directory:" << std::endl;
		std::cout << path.c_str() <<"\nSDL2_image.dll\nlibpng16-16.dll"<<std::endl; 
		system("PAUSE");
	}

	rect.x = xp;
	rect.y = yp;
	rect.w = w;
	rect.h = h;
	speed = 0;

	centerX = rect.x + rect.w/2;
	centerY = rect.y + rect.h/2;
}

void CSprite::draw(int X, int Y, int _xOffset, int _yOffset)
{
	rect.x = X + _xOffset;
	rect.y = Y + _yOffset;
	centerX = rect.x + rect.w/2;
	centerY = rect.y + rect.h/2;
	SDL_RenderCopy(renderer,texture,NULL,&rect);
	//SDL_RenderCopy(renderer,IMG_LoadTexture(renderer,"grassTexture0.bmp"),NULL,&rect);
}

bool CSprite::operator==(CSprite *s)
{
	if(path == s->path) return true;
	return false;
}

void CSprite::setX(float X)
{
	x = X;
}

void CSprite::setY(float Y)
{
	y = Y;
}

void CSprite::setPos(float X, float Y)
{
	x = X;
	y = Y;
}

int CSprite::getWidth(void)
{
	return width;
}

int CSprite::getHeight(void)
{
	return height;
}

float CSprite::getX()
{
	return x;
}

float CSprite::getY()
{
	return y;
}

int CSprite::getCenterX(void)
{
	return centerX;
}

int CSprite::getCenterY(void)
{
	return centerY;
}

SDL_Rect CSprite::getRect(void)
{
	return rect;
}

SDL_Rect* CSprite::getRectByReference(void)
{
	return &rect;
}

SDL_Texture* CSprite::getTexture(void)
{
	return texture;
}

SDL_Renderer* CSprite::getRenderer(void)
{
	return renderer;
}

CSprite::~CSprite(void)
{
	SDL_DestroyTexture(texture);
}
