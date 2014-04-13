#include "C4.h"
#define pi M_PI

C4::C4(float X, float Y, int* _xOffset, int* _yOffset, SDL_Renderer* _renderer) :
	GameObject(X,Y,new CSprite(_renderer,"c4.png",X,Y,24,24)),
	xOffset(_xOffset),
	yOffset(_yOffset),
	render(false),
	timer(0),
	renderer(_renderer),
	exptime(-1)
{
	for(unsigned int c = 0; c < AMOUNT_OF_PARTICLES; c++)
	{
		SDL_Rect rect;
		rect.x = x + sprite->getWidth()/2 - PARTICLE_SIZE/2;
		rect.y = y + sprite->getHeight()/2 - PARTICLE_SIZE/2;
		rect.w = PARTICLE_SIZE;
		rect.h = PARTICLE_SIZE;
		particles.push_back(rect);

		coordinate cd;
		cd.x = (float)x;
		cd.y = (float)y;
		particleCoordinates.push_back(cd);
	}
}

void C4::Update()
{
	if(exptime > -1 && exptime < PARTICLE_LIFE)
	{
		for(unsigned int c = 0; c < AMOUNT_OF_PARTICLES; c++)
		{
			if(c < AMOUNT_OF_PARTICLES/3)
			{
				double angle = (double)c * (1080.0/AMOUNT_OF_PARTICLES) * pi / 180; //Converting 'c' value into fraction of circle (in degrees), then converting to radians
				particleCoordinates[c].x += PARTICLE_SPEED * cos(angle);
				particleCoordinates[c].y += PARTICLE_SPEED * sin(angle);
				particles[c].x = particleCoordinates[c].x + *xOffset;
				particles[c].y = particleCoordinates[c].y + *yOffset;
				SDL_SetRenderDrawColor(renderer,0x22,0x22,0x22,0x55);
				SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
				SDL_RenderFillRect(renderer,&particles[c]);
			}
			else if(c < 2 * AMOUNT_OF_PARTICLES/3)
			{
				double angle = (double)(c - AMOUNT_OF_PARTICLES/3) * (1080.0/AMOUNT_OF_PARTICLES) * pi / 180;
				particleCoordinates[c].x += 4 * PARTICLE_SPEED * cos(angle) / 5;
				particleCoordinates[c].y += 4 * PARTICLE_SPEED * sin(angle) / 5;
				particles[c].x = particleCoordinates[c].x + *xOffset;
				particles[c].y = particleCoordinates[c].y + *yOffset;
				SDL_SetRenderDrawColor(renderer,0x88,0x44,0,0x66);
				SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
				SDL_RenderFillRect(renderer,&particles[c]);
			}
			else
			{
				double angle = (double)(c - 2 * AMOUNT_OF_PARTICLES/3) * (1080.0/AMOUNT_OF_PARTICLES) * pi / 180;
				particleCoordinates[c].x += 3 * PARTICLE_SPEED * cos(angle) / 5;
				particleCoordinates[c].y += 3 * PARTICLE_SPEED * sin(angle) / 5;
				particles[c].x = particleCoordinates[c].x + *xOffset;
				particles[c].y = particleCoordinates[c].y + *yOffset;
				SDL_SetRenderDrawColor(renderer,0x88,0x88,0x00,0x66);
				SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
				SDL_RenderFillRect(renderer,&particles[c]);
			}
		}
		exptime++;
	}
	else
	{
		exptime = -1;
	}
}

void C4::Detonate()
{
	if(SDL_GetTicks() - timer < TIME_BUFFER) 
	{
		render = true;
		return;
	}
	render = false;
	timer = SDL_GetTicks();
	exptime = 0;
	for(unsigned int c = 0; c < AMOUNT_OF_PARTICLES; c++)
	{
		particleCoordinates[c].x = x;
		particleCoordinates[c].y = y;
	}
}

void C4::setRenderer(SDL_Renderer* renderer)
{
	sprite = new CSprite(renderer,"c4.png",0,0,24,24);
}

void C4::SetTimer(Uint32 time)
{
	timer = time;
}

bool C4::ReadyToClick()
{
	if(SDL_GetTicks() - timer >= TIME_BUFFER)
	{
		return true;
	}
	return false;
}

C4::~C4(void)
{
}

const int C4::PARTICLE_SIZE = 16;
const int C4::AMOUNT_OF_PARTICLES = 120;
const int C4::PARTICLE_SPEED = 2;
const int C4::PARTICLE_LIFE = 64;
const int C4::EXPLOSION_RADIUS = PARTICLE_SPEED * PARTICLE_LIFE;
