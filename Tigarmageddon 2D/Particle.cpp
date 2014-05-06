#include "Particle.h"
#include "destructive_reasoning.h"
#include <time.h>

Particle::Particle(float X, float Y, float velx, float vely, Screen* screen) :
	x(X),
	y(Y),
	velX(velx),
	velY(vely),
	renderer(screen->getRenderer()),
	window(screen->getWindow()),
	timer(0)
{
	srand(time(NULL));

	Uint32 r = rand() % 0xFF000000;
	Uint32 g = rand() % r + 1;
	Uint32 b = rand() % (g/2);
	Uint32 a = rand() % 0x000000FF;

	surface = SDL_CreateRGBSurface(SDL_SWSURFACE,8,8,32,0xFF000000,0x00FF0000,0x0000FF00,0x000000FF);
	texture = SDL_CreateTextureFromSurface(renderer,surface);
}

void Particle::Update()
{
	timer++;
	if(timer >= life) return;
	x += velX;
	y += velY;
}

void Particle::Render()
{
	if(timer >= life || timer == 0) 
	{
		return;
	}
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	//rect.w = Particle::Particle_Size;
	//rect.h = Particle::Particle_Size;
	rect.w = 8;
	rect.h = 8;
	//surface = SDL_CreateRGBSurface(SDL_SWSURFACE,8,8,32,0,0,0,0);
	//if(SDL_FillRect(SDL_GetWindowSurface(window),&rect,SDL_MapRGB(surface->format,0xFF,0,0)) != 0) printf("Error");
	//SDL_RenderCopy(renderer,texture,NULL,&rect);
	//SDL_UpdateWindowSurface(window);
}

void Particle::Render2()
{
	if(timer >= life || timer == 0) 
	{
		return;
	}
}

void Particle::setTimer(int t)
{
	timer = t;
}

Particle::~Particle(void)
{
	delete texture;
	delete surface;
}

const int Particle::life = 100;
const int Particle::Particle_Size = 16;