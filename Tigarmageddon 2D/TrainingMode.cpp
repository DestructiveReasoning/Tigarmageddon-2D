#include "TrainingMode.h"


TrainingMode::TrainingMode(int width, int height, int levelWidth, int levelHeight, float* xOffset, float* yOffset, Player* player, Screen* screen) :
	SurvivalMode(width,height,levelWidth,levelHeight,xOffset,yOffset,player,screen),
	SmallFontSize(8),
	MediumFontSize(12),
	LargeFontSize(16)
{
	step = 0;
	age = 0;
	TTF_Init();
	font = TTF_OpenFont("imagine_font.ttf",MediumFontSize);
	instructionSurface = nullptr;
	rect.x = 0;
	rect.y = 32;

	rect2.x = 0;
	rect2.y = rect.y + LargeFontSize + 2;

	rect3.x = 0;
	rect3.y = rect2.y + LargeFontSize + 2;

	fg.a = 0xFF;
	fg.r = 0;
	fg.g = 0;
	fg.b = 0;

	renderer = player->getRenderer();

	start = SDL_GetTicks();
	Main::tgs.clear();
	Main::ammoBoxes.clear();

	survivalex = new CSprite(renderer,"SurvivalExplanation.png",0,0,width,height);
	mercenaryex = new CSprite(renderer,"MercenaryExplanation.png",0,0,width,height);
}

void TrainingMode::update()
{
	/*
	STEPS
	0: How to move
	1: How to shoot
	2: Paintball Gun
	3: How to select weapons - Glock
	4: How to select weapons
	5: How to use C4
	6: Show Ammobox & how to use it
	7: Show Tiger Generator, kill tiger
	8: Show how to destroy tiger generator
	9: Explain Survival Mode
	10: Explain Mercenary Mode
	*/

	if(Main::ammoBoxes.size() > 0 && step != AmmoBoxes) Main::ammoBoxes.clear();
	if(Main::tgs.size() > 0 && step!= TigerGenerators && step != DestroyTigerGenerator) Main::tgs.clear();

	if(step != TigerGenerators) Main::spawning = false;
	else Main::spawning = true;

	SDL_Texture *tex = nullptr;

	switch(step)
	{
	case Move:
		message = "Move with the WASD keys";
		rect.h = LargeFontSize;
		rect.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		if(SDL_GetTicks() - start > 3000)
		{
			step++;
			start = SDL_GetTicks();
			SDL_DestroyTexture(tex);
		}
		break;
	case Shoot:
		message = "Shoot with the Left Mouse Button";
		rect.h = LargeFontSize;
		rect.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		if(SDL_GetTicks() - start > 3000)
		{
			step++;
			start  = SDL_GetTicks();
			SDL_DestroyTexture(tex);
		}
		break;
	case Paintball:
		message = "The weapon you are holding is the Paintball Gun.";
		rect.h = MediumFontSize;
		rect.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "With the Paintball Gun,";
		rect2.h = MediumFontSize;
		rect2.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		message = "you can shoot with the Right Mouse Button too.";
		rect3.h = MediumFontSize;
		rect3.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect3);
		if(SDL_GetTicks() - start > 6000)
		{
			step++;
			start  = SDL_GetTicks();
			SDL_DestroyTexture(tex);
		}
		break;
	case SelectGlock:
		message = "Select Weapons by hitting number keys.";
		rect.h = LargeFontSize;
		rect.w = message.size() *LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "Select the Glock by pressing 0.";
		rect2.h = LargeFontSize;
		rect2.w = message.size() *LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		message = "The Glock has infinite ammo.";
		rect3.h = LargeFontSize;
		rect3.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect3);
		if(SDL_GetTicks() - start > 6000)
		{
			step++;
			start  = SDL_GetTicks();
			SDL_DestroyTexture(tex);
		}
		break;
	case SelectWeapons:
		message = "Try playing around with other weapons";
		rect.h = LargeFontSize;
		rect.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		if(SDL_GetTicks() - start > 10000)
		{
			step++;
			start = SDL_GetTicks();
			SDL_DestroyTexture(tex);
		}
		break;
	case DetonateC4:
		message = "Press SPACE to plant a C4.";
		rect.h = LargeFontSize;
		rect.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "Press SPACE again to detonate it.";
		rect2.h = LargeFontSize;
		rect2.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		message = "Don't kill yourself!";
		rect.h = 24;
		rect.w = message.size() * 24;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect3);
		if(SDL_GetTicks() - start > 8000)
		{
			step++;
			start = SDL_GetTicks();
			SDL_DestroyTexture(tex);
			Main::spawning = false;
		}
		break;
	case AmmoBoxes:
		if(Main::ammoBoxes.size() == 0)
		{
			Main::ammoBoxes.push_back(std::shared_ptr<AmmoBox>(new AmmoBox(player->getX() + 100, player->getY() + 100,xOffset,yOffset,renderer)));
		}
		message = "This is an ammobox.";
		rect.h = LargeFontSize;
		rect.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "Step on the ammobox to fill your current weapon with ammo.";
		rect2.h = MediumFontSize;
		rect2.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		if(SDL_GetTicks() - start > 60000 || (player->hasFullAmmo() && SDL_GetTicks() - start > 2000))
		{
			step++;
			start = SDL_GetTicks();
			SDL_DestroyTexture(tex);
			Main::tgs.push_back(std::shared_ptr<TigerGenerator>(new TigerGenerator(player->getX() + 200,player->getY() + 200,xOffset,yOffset,renderer,player)));
			Main::spawning = true;
		}
		break;
	case Tips::TigerGenerators:
		message = "This is a tiger generator. Tigers spawn from Tiger Generators. Kill the tiger(s).";
		rect.h = LargeFontSize;
		rect.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "Tigers spawn from Tiger Generators.";
		rect2.h = LargeFontSize;
		rect2.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		message = "Kill the tiger(s).";
		rect3.h = LargeFontSize;
		rect3.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect3);
		if(Main::killcount > 0)
		{
			step++;
			start = SDL_GetTicks();
			SDL_DestroyTexture(tex);
			Main::spawning = false;
		}
		break;
	case DestroyTigerGenerator:
		message = "Tiger Generators can be destroyed by ballistic knives.";
		rect.h = MediumFontSize;
		rect.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "Select the ballistic knife by pressing 3,";
		rect2.h = LargeFontSize;
		rect2.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		message = "and destroy the Tiger Generator.";
		rect3.h = LargeFontSize;
		rect3.w = message.size() * LargeFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect3);
		if(Main::tgs.size() == 0)
		{
			step++;
			start = SDL_GetTicks();
			SDL_DestroyTexture(tex);
			Main::spawning = false;
		}
		break;
	case Survival:
		survivalex->draw(0,0,0,0);
		if(SDL_GetTicks() - start > 30000)
		{
			step++;
			start = SDL_GetTicks();
		}
		break;
	case Mercenary:
		mercenaryex->draw(0,0,0,0);
		if(SDL_GetTicks() - start > 30000)
		{
			step++;
			start = SDL_GetTicks();
		}
		break;
	default:
		message = "Congrats! You have finished training.";
		rect.h = MediumFontSize;
		rect.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect);
		message = "To Exit, open the pause menu by pressing ESC.";
		rect2.h = MediumFontSize;
		rect2.w = message.size() * MediumFontSize;
		instructionSurface = TTF_RenderText_Solid(font,message.c_str(),fg);
		tex = SDL_CreateTextureFromSurface(renderer,instructionSurface);
		SDL_FreeSurface(instructionSurface);
		SDL_RenderCopy(renderer,tex,NULL,&rect2);
		break;
	}
}

TrainingMode::~TrainingMode(void)
{
	TTF_CloseFont(font);
	if(instructionSurface != nullptr) SDL_FreeSurface(instructionSurface);
	TTF_Quit();

	delete survivalex;
	delete mercenaryex;
}
