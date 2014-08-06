#include "Player.h"
#include "destructive_reasoning.h"
#include "Screen.h"
#include "Sprite.h"
#include "Main.h"
#include "BallisticKnife.h"

#define font_reg 32

#define SKORPION_FIRE_RATE 0.015
#define GATTLINGGUN_FIRE_RATE 0.025
#define SPAS_ANGLE_OFFSET 0.1;

#define BULLET_MASS 0.75
#define PLAYER_MASS 70
#define RECOIL BULLET_MASS*PLAYER_MASS

Player::Player(float X, float Y, CSprite* csprite, float* _xOffset, float* _yOffset, int _gameWidth, int _gameHeight, CSprite* bullet, int lvl) : 
	GameObject(X,Y,csprite),
	PaintBallGun_Damage(25),
	Glock_Damage(17),
	Skorpion_Damage(20),
	Spas_Damage(35),
	GattlingGun_Damage(22),
	hasC4(true),
	dead(false),
	audioRate(44100),
	audioFormat(MIX_DEFAULT_FORMAT),
	audioChannels(2),
	audioBuffers(1024),
	hasPacman(false)
{
	/**
	POSITION OPERATOR TEST
	*/
	Position p1, p2;
	p1.x = 0;
	p1.y = 0;
	p2.x = 100;
	p2.y = 100;
	if(p1 == p2) printf("*****POSITION TEST: TRUE\n");
	else         printf("*****POSITION TEST: FALSE\n");
	angle = 0.0;
	lastTime = SDL_GetTicks();
	now = SDL_GetTicks();
	xOffset = _xOffset;
	yOffset = _yOffset;
	gameWidth = _gameWidth;
	gameHeight = _gameHeight;

	//screen = _screen;
	weapon = PaintballGun;
	setDefaultBarrelPosition(21,-8);
	readyToShoot = true;
	readyToShoot3 = true;
	//Paintball Gun
	clips.push_back(Magazine(80,80));
	//Skorpion
	clips.push_back(Magazine(40,40));
	//Ballistic Knife
	clips.push_back(Magazine(1,1));
	//Spas
	clips.push_back(Magazine(12,12));
	//Gattling Gun
	clips.push_back(Magazine(100,100));
	//Pacman Gun
	clips.push_back(Magazine(0,0));
	//Glock
	clips.push_back(Magazine(-1,-1));

	weaponNames.push_back("Paintball Gun");
	weaponNames.push_back("Skorpion SMG");
	weaponNames.push_back("Ballistic Knife");
	weaponNames.push_back("Spas 12");
	weaponNames.push_back("Gatling Gun");
	weaponNames.push_back("Pacman");
	weaponNames.push_back("Glock");

	switch(lvl)
	{
	case Main::Field:
		fg.r = 0xdd;
		fg.g = 0xdd;
		fg.b = 0xff;
		fg.a = 0x33;
		break;
	case Main::Siberia:
		fg.r = 0x44;
		fg.g = 0x44;
		fg.b = 0xff;
		fg.a = 0x33;
		break;
	default:
		printf("Invalid Level for Color Scheme\n");
		break;
	}
	initHUD();
	speed = Default_Player_Speed;

	/*
	if(Mix_OpenAudio(audioRate,audioFormat,audioChannels,audioBuffers))
	{
		printf("Could not load audio device.\n");
	}
	*/

	Mix_AllocateChannels(16);
	gunshot = Mix_LoadWAV("gunshot.wav");
	spasShot = Mix_LoadWAV("spasShot.wav");

	bullets.clear();
	knives.clear();
}

void Player::Update()
{
	if(dead) return;

	//Check Collision With Tigers
	for(auto c = 0; c < Main::tigers.size(); c++)
	{
		if(onCollision(Main::tigers[c].get(),velx,vely) && Main::tigers[c]->getHealth() > 0)
		{
			this->kill();
			return;
		}
	}

	//Check Collision With Stones
	bool colliding = false;
	for(int c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision(Main::stones[c],velx,0))
		{
			colliding = true;	
			break;
		}
	}

	if(!colliding) move2(velx,0);

	colliding = false;
	for(int c = 0; c < Main::stones.size(); c++)
	{
		if(onCollision(Main::stones[c],0,vely))
		{
			colliding = true;	
			break;
		}
	}
	if(!colliding) move2(0,vely);

	//Check Collisions with Ballistic Knives
	for(auto c = 0; c < knives.size(); c++)
	{
		if(knives[c]->hasFallen())
		{
			if(onCollisionBullet(knives[c],0,0))
			{
				(*clips[Ballistic_Knife].getAmmo())++;
				delete knives[c];
				knives[c--] = knives.back();
				knives.pop_back();
			}
		}
	}

	//Check Collision With Ammoboxes
	for(auto c = 0; c < Main::ammoBoxes.size(); c++)
	{
		if(onCollision(Main::ammoBoxes[c].get(),velx,vely))
		{
			collectAmmo(c);
			break;
		}
	}
	
	//Update Bullets
	for(unsigned int c = 0; c < bullets.size(); c++)
	{
		if(c < 0) continue;
		if(bullets[c] == nullptr) continue;
		bullets[c]->Update();
		//if(!bullets[c]->onTheGround) bullets[c]->Render();
		bullets[c]->Render();
		if(bullets[c]->getAge() >= bullets[c]->getRange() || bullets[c]->destroy)
		{
			if(bullets[c]->destructible)
			{
				delete bullets[c];
				bullets[c--] = bullets.back();
				bullets.pop_back();
			}
		}else {
			//++c;
		}
	}

	//Updating Ballistic Knives
	for(auto c = 0; c < knives.size(); c++)
	{
		if(c < 0) continue;
		if(knives[c] == nullptr) continue;
		knives[c]->Update();
		if(!knives[c]->hasFallen()) knives[c]->Render();
		//knives[c]->Render();
		if(knives[c]->getAge() >= knives[c]->getRange() || knives[c]->destroy)
		{
			if(false) //knives[c]->destructible
			{
				delete knives[c];
				knives[c--] = knives.back();
				knives.pop_back();
			}else 
			{
				knives[c]->fall();
			}
		}else {
			//++c;
		}
	}

	//Update Pacman
	if(pacman != nullptr)
	{
		printf("Updating Pacman\n");
		if(pacman->destroy) pacman = nullptr;
		else
		{
			pacman->Update();
			pacman->Render();
		}
	}

	c4->Update();

	renderHUD();   
}

void Player::Shoot(int mouseButton, float velX, float velY, Screen* screen)
{
	switch(weapon)
	{
	case PaintballGun:
		//SEMI AUTO, SHOOTS WITH LEFT AND RIGHT
		if(readyToShoot && *clips[PaintballGun].getAmmo() > 0 && mouseButton == 1)
		{
			//Mix_HaltChannel(-1);
			Mix_PlayChannel(-1,gunshot,0);
			bullets.push_back(new Bullet(barrelX,barrelY,velX * BULLET_SPEED,velY * BULLET_SPEED,xOffset,yOffset,100,PaintBallGun_Damage,screen->getRenderer()));
			(*clips[PaintballGun].getAmmo())--;
			readyToShoot = false;
		}
		if(readyToShoot3 && *clips[PaintballGun].getAmmo() > 0 && mouseButton == 3)
		{
			//Mix_HaltChannel(-1);
			Mix_PlayChannel(-1,gunshot,0);
			bullets.push_back(new Bullet(barrelX,barrelY,velX * BULLET_SPEED,velY * BULLET_SPEED,xOffset,yOffset,100,PaintBallGun_Damage,screen->getRenderer()));
			(*clips[PaintballGun].getAmmo())--;
			readyToShoot3 = false;
		}
		break;
	case Glock:
		if(readyToShoot && mouseButton == 1)
		{
			//Mix_HaltChannel(-1);
			Mix_PlayChannel(-1,gunshot,0);
			bullets.push_back(new Bullet(barrelX,barrelY,velX * BULLET_SPEED,velY * BULLET_SPEED,xOffset,yOffset,100,Glock_Damage,screen->getRenderer()));
			readyToShoot = false;
		}
		break;
	case Skorpion:
		now = SDL_GetTicks();
		if(now - lastTime >= double(1/SKORPION_FIRE_RATE) && *clips[Skorpion].getAmmo() > 0 && mouseButton == 1)
		{
			//Mix_HaltChannel(-1);
			Mix_PlayChannel(-1,gunshot,0);
			bullets.push_back(new Bullet(barrelX,barrelY,velX * BULLET_SPEED,velY * BULLET_SPEED,xOffset,yOffset,100,Skorpion_Damage,screen->getRenderer()));
			(*clips[Skorpion].getAmmo())--;
			lastTime = SDL_GetTicks();
		}
		break;
	case GattlingGun:
		now = SDL_GetTicks();
		if(now - lastTime >= double(1/GATTLINGGUN_FIRE_RATE) && *clips[GattlingGun].getAmmo() > 0 && mouseButton == 1)
		{
			//Mix_HaltChannel(-1);
			Mix_PlayChannel(-1,gunshot,0);
			bullets.push_back(new Bullet(barrelX,barrelY,velX * BULLET_SPEED,velY * BULLET_SPEED,xOffset,yOffset,100,GattlingGun_Damage,screen->getRenderer()));
			(*clips[GattlingGun].getAmmo())--;
			lastTime = SDL_GetTicks();
		}
		break;
	case Spas:
		if(readyToShoot && *clips[weapon].getAmmo() > 0 && mouseButton == 1)
		{
			//Mix_HaltChannel(-1);
			Mix_PlayChannel(-1,spasShot,0);
			double angle = atan2(velY,velX);
			double leftAngle = angle - SPAS_ANGLE_OFFSET;
			float leftVelX = cos(leftAngle) * BULLET_SPEED;
			float leftVelY = sin(leftAngle) * BULLET_SPEED;
			double rightAngle = angle + SPAS_ANGLE_OFFSET;
			float rightVelX = cos(rightAngle) * BULLET_SPEED;
			float rightVelY = sin(rightAngle) * BULLET_SPEED;
			bullets.push_back(new Bullet(barrelX,barrelY,velX * BULLET_SPEED,velY * BULLET_SPEED,xOffset,yOffset,100,Spas_Damage,screen->getRenderer()));
			bullets.push_back(new Bullet(barrelX,barrelY,leftVelX,leftVelY,xOffset,yOffset,100,Spas_Damage,screen->getRenderer()));
			bullets.push_back(new Bullet(barrelX,barrelY,rightVelX,rightVelY,xOffset,yOffset,100,Spas_Damage,screen->getRenderer()));
			(*clips[Spas].getAmmo())--;
			//x -= 20*velX;
			//y -= 20*velY;
			bool hit = false;
			int c;
			bool colliding = false;
			for(int c = 0; c < Main::stones.size(); c++)
			{
				if(onCollision(Main::stones[c],-RECOIL*velX,0))
				{
					colliding = true;	
					break;
				}
			}

			if(!colliding) move2(-RECOIL*velX,0);

			colliding = false;
			for(int c = 0; c < Main::stones.size(); c++)
			{
				if(onCollision(Main::stones[c],0,-RECOIL*velY))
				{
					colliding = true;	
					break;
				}
			}
			if(!colliding) move2(0,-RECOIL*velY);
			for(c = 0; c <= 0; c++)             //CHANGED 20 to 0 TO TEST ***NOW USELESS CODE
			{
				for(int j = 0; j < Main::stones.size(); j++)
				{
					if(onCollision(Main::stones[j],-velX *c, -velY * c))
					{
						x -= c * velX;
						*xOffset += c * velX;
						y -= c * velY;
						*yOffset += c * velY;
						hit = true;
						break;
					}
				}
				if(hit) std::cout<<"Hit"<<std::endl;
				if(hit) break;
				if(c == 20)
				{
					x -= 20*velX;
					*xOffset += 20 * velX;
					y -= 20*velY; 
					*yOffset += 20 * velY;
				}
			}
			readyToShoot = false;
			std::cout<<c<<std::endl;
		}
		break;
	case PacmanGun:
		if(*clips[PacmanGun].getAmmo() > 0)
		{
			pacman = new Pacman(barrelX,barrelY,velX * Pacman::Speed,velY*Pacman::Speed,xOffset,yOffset,screen->getRenderer());
			clips[PacmanGun].setAmmo(0);
			pacmanWave = Main::wave;
			weaponSprites[PacmanGun] = hand;
		}
		break;
	case Ballistic_Knife:
		if(readyToShoot && mouseButton == 1 && *clips[weapon].getAmmo() > 0)
		{
			knives.push_back(new BallisticKnife(centerX,centerY,velX * BallisticKnife::Speed,velY * BallisticKnife::Speed,xOffset,yOffset,screen->getRenderer()));
			readyToShoot = false;
			(*clips[Ballistic_Knife].getAmmo())--;
		}
		break;
	default:
		break;
	}
}

void Player::collectAmmo(int c)
{
	clips[weapon].setAmmo(*clips[weapon].getCapacity());
	hasC4 = true;
	Main::ammoBoxes[c] = Main::ammoBoxes.back();
	Main::ammoBoxes.pop_back();
}

void Player::PlaceC4(Screen* screen)
{
	c4->setRenderer(screen->getRenderer());
	c4->setX(x);
	c4->setY(y + c4->getSprite()->getHeight());
	c4->render = true;
	c4->SetTimer(SDL_GetTicks());
}

void Player::Render(int _xOffset, int _yOffset)
{
	if(!Main::paused)
	{
		SDL_GetMouseState(&mouseX,&mouseY);
		double xcomponent, ycomponent;
		xcomponent = mouseX - barrelX - _xOffset;
		ycomponent = mouseY - barrelY - _yOffset;
		angle = (atan2(ycomponent,xcomponent)) * 180 / M_PI + 90;
		//x' = x * cos(angle) - y * sin(angle)
		//y' = y * cos(angle) + x * sin(angle);
		toRadians(&angle);
		barrelX = centerX + (defaultBarrelX * cos(angle) - defaultBarrelY * sin(angle));
		barrelY = centerY + (defaultBarrelY * cos(angle) + defaultBarrelX * sin(angle));
		toDegrees(&angle);
	}
	if(c4->render)
	{
		SDL_Rect r;
		r.x = c4->getX() + *xOffset;
		r.y = c4->getY() + *yOffset;
		r.w = 24;
		r.h = 24;
		SDL_RenderCopy(c4->getSprite()->getRenderer(),c4->getSprite()->getTexture(),NULL, &r);
	}
	SDL_Point center = {sprite->getWidth()/2,sprite->getHeight()/2};
	sprite->getRectByReference()->x = int(x) + _xOffset;
	sprite->getRectByReference()->y = int(y) + _yOffset;
	//SDL_RenderCopyEx(renderer,sprite->getTexture(),NULL,sprite->getRectByReference(),angle,&center,SDL_FLIP_NONE);
	//if(abs(mouseX - barrelX) < 30 && abs(mouseY - barrelY) < 30) angle = 0;
	if(mouseX >= sprite->getRect().x && mouseX <= sprite->getRect().x + sprite->getRect().w 
		&& mouseY >= sprite->getRect().y && mouseY <= sprite->getRect().y + sprite->getRect().h) angle = 0;
	//if(abs(mouseX - centerX) <= 56 && abs(mouseY - centerX) <= 56) angle = 0;
	weaponSprites[weapon]->getRectByReference()->x = int(x) + 24 + _xOffset;
	weaponSprites[weapon]->getRectByReference()->y = int(y) + 32 - weaponSprites[weapon]->getRectByReference()->h + _yOffset;
	weaponSprites[GattlingGun]->getRectByReference()->x = int(x) + sprite->getWidth()/2 - weaponSprites[GattlingGun]->getWidth()/2 + _xOffset;
	weaponSprites[GattlingGun]->getRectByReference()->y = int(y) - 16 + _yOffset;
	weaponSprites[Spas]->getRectByReference()->x = int(x) + _xOffset;
	SDL_Point weaponcenter = {-weaponSprites[weapon]->getRectByReference()->x + sprite->getRectByReference()->x + sprite->getWidth()/2,
		                      -weaponSprites[weapon]->getRectByReference()->y + sprite->getRectByReference()->y + sprite->getHeight()/2
	                         };
	//weaponSprites[0]->draw(int(x + 24),int(y + 32 - weaponSprites[0]->getRectByReference()->h),_xOffset,_yOffset);
	SDL_RenderCopyEx(renderer,weaponSprites[weapon]->getTexture(),NULL,weaponSprites[weapon]->getRectByReference(),angle,&weaponcenter,SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer,sprite->getTexture(),NULL,sprite->getRectByReference(),angle,&center,SDL_FLIP_NONE);
}

bool Player::onCollision(GameObject* object, float velX, float velY)
{
	if(((x + sprite->getWidth() + velX) >= object->getX()) && ((x + velX) < (object->getX() + object->getSprite()->getWidth())))
	{
		if(((y + sprite->getHeight() + velY) >= object->getY()) && ((y + velY) < (object->getY() + object->getSprite()->getHeight())))
		{
			return true;
		}else return false;
	}else return false;
}

bool Player::onCollisionBullet(Bullet* object, float velX, float velY)
{
	if(((x + sprite->getWidth() + velX) >= object->getX()) && ((x + velX) < (object->getX() + object->getProjectileSprite().getWidth())))
	{
		if(((y + sprite->getHeight() + velY) >= object->getY()) && ((y + velY) < (object->getY() + object->getProjectileSprite().getHeight())))
		{
			return true;
		}else return false;
	}else return false;
}

void Player::move2(float velX, float velY)
{
	//OFFSET STOPS AT -2200
	x += velX;
	if(velX > 0)
	{
		if(*xOffset - velX < -Main::level_width + gameWidth && x + velX >= gameWidth/2 - sprite->getWidth()/2) *xOffset =  -Main::level_width + gameWidth;
		else if (x + velX >= gameWidth/2 - sprite->getWidth()/2) *xOffset -= velX;
	}
	else if(velX != 0)
	{
		if(*xOffset - velX > 0 && x + *xOffset + velX <= gameWidth/2) *xOffset = 0;
		else if(x + *xOffset + velX <= gameWidth/2) *xOffset -= velX;
	}

	y += velY;
	if(velY > 0)
	{
		if(*yOffset - velY < -Main::level_height + gameHeight && y + velY >= gameHeight/2 - sprite->getHeight()/2) *yOffset =  -Main::level_height + gameHeight;
		else if (y + velY >= gameHeight/2 - sprite->getHeight()/2) *yOffset -= velY;
	}
	else if(velY != 0)
	{
		if(*yOffset - velY > 0 && y + *yOffset + velY <= gameHeight/2) *yOffset = 0;
		else if(y + *yOffset + velY <= gameHeight/2) *yOffset -= velY;
	}

	centerX = x + sprite->getRect().w/2;
	centerY = y + sprite->getRect().h/2;
}

void Player::move(float velX, float velY)
{
	x += velX;
	if(velX < 0)
	{
		for(int c = int(velX); c < 0; c++)
		{
			if(*xOffset - c <= 0 && x + *xOffset < gameWidth/2 - sprite->getWidth()/2)
			{
				*xOffset -= c;
				break;
			}
		}
	}else
	{
		for(int c = int(velX); c > 0; c--)
		{
			if(*xOffset - c > -Main::level_width + gameWidth && x + *xOffset > gameWidth/2 - sprite->getWidth()/2)
			{
				*xOffset -= c;
				break;
			}
		}
	}
	y += velY;
	if(velY < 0)
	{
		for(int c = int(velY); c < 0; c++)
		{
			if(*yOffset - c <= 0 && y + *yOffset < gameHeight/2 - sprite->getHeight()/2)
			{
				*yOffset -= c;
				break;
			}
		}
	}else
	{
		for(int c = int(velY); c > 0; c--)
		{
			if(*yOffset - c > -Main::level_height + gameHeight && y + *yOffset > gameHeight/2 - sprite->getHeight()/2)
			{
				*yOffset -= c;
				break;
			}
		}
	}
	centerX = x + sprite->getRect().w/2;
	centerY = y + sprite->getRect().h/2;

	//printf("%d\n", *xOffset);
	//std::cout<<x<<std::endl;
}

void Player::setWeapon(int w)
{
	if (w < 0) w = Weapon_END - 1;
	if (w >= Weapon_END) w = 0;

	if(w == PacmanGun)
	{
		if(*clips[0].getAmmo() == 1 && *clips[1].getAmmo() == 3 && *clips[2].getAmmo() == 3 && *clips[3].getAmmo() == 7 && Main::wave - pacmanWave >= 2) 
		{
			weaponSprites[PacmanGun] = pacmanGun;
			clips[PacmanGun].setAmmo(1);
		}
	}

	weapon = Weapon(w);
	if(w == GattlingGun || w == Ballistic_Knife)
	{
		setDefaultBarrelPosition(0,-sprite->getHeight()/2 - 16);
	}else
	{
		setDefaultBarrelPosition(21,-8);
	}

	if(weapon == GattlingGun)
	{
		speed = 1;
	}else if(weapon == Glock || weapon == Skorpion)
	{
		speed = Default_Player_Speed + 0.5;
	}else if(weapon == Ballistic_Knife)
	{
		speed = Default_Player_Speed + 0.8;
	}else if(weapon == PaintballGun)
	{
		speed = Default_Player_Speed + 0.2;
	}
	else speed = Default_Player_Speed;
}

void Player::cock(int mouseButton)
{
	switch(mouseButton)
	{
	case 1:
		readyToShoot = true;
		break;
	case 3:
		readyToShoot3 = true;
		break;
	default:
		break;
	}
}

void Player::initHUD()
{
	if(TTF_Init() != 0)
	{
		std::cout << "Error Initializing the TTF Library" << std::endl;
		std::cout << "Make sure the following are in this game's directory:" << std::endl;
		std::cout << "SDL2_ttf.dll" << std::endl;
		SDL_Quit();
		return;
	}

	font = TTF_OpenFont("imagine_font.ttf",24);

	//SDL_Color fg = {255,0,0};

	ammoSurface = NULL;
	weaponSurface = NULL;
	//killedSurface = NULL;
	waveSurface = NULL;
	//ammoText = NULL;
	//weaponText = NULL;
	//killedText = NULL;
	//waveText = NULL;

	ammoRect.x = 0;
	ammoRect.y = gameHeight - 30;
	//ammoRect.w = 64;
	ammoRect.h = 32;

	weaponRect.x = 0;
	weaponRect.y = 0;
	weaponRect.h = 16;

	waveRect.h = 16;
	waveRect.y = 0;
}

void Player::renderHUD()
{
	//SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
	s.str(std::string());
	s.clear();

	if(*clips[weapon].getAmmo() != -1) s << *clips[weapon].getAmmo();
	else s << "-";
	ammoRect.w = s.str().size() * 32;
	ammoSurface = TTF_RenderText_Solid(font,s.str().c_str(),fg);
	SDL_Texture* ammoText = SDL_CreateTextureFromSurface(renderer,ammoSurface);
	SDL_FreeSurface(ammoSurface);
	SDL_RenderCopy(renderer,ammoText,NULL,&ammoRect);

	s.str(std::string());
	s.clear();
	s << weaponNames[weapon];
	weaponRect.w = s.str().size() * 16;
	weaponSurface = TTF_RenderText_Solid(font,s.str().c_str(),fg);
	SDL_Texture* weaponText = SDL_CreateTextureFromSurface(renderer,weaponSurface);
	SDL_FreeSurface(weaponSurface);
	SDL_RenderCopy(renderer,weaponText,NULL,&weaponRect);

	s.str(std::string());
	s.clear();
	s << "Wave: " << Main::wave;
	waveRect.w = s.str().size() * 16;
	waveSurface = TTF_RenderText_Solid(font,s.str().c_str(),fg);
	waveRect.x = gameWidth - waveRect.w;
	SDL_Texture* waveText = SDL_CreateTextureFromSurface(renderer,waveSurface);
	SDL_FreeSurface(waveSurface);
	SDL_RenderCopy(renderer,waveText,NULL,&waveRect);

	SDL_DestroyTexture(ammoText);
	SDL_DestroyTexture(weaponText);
	
	SDL_DestroyTexture(waveText);
}

void Player::setScreen(Screen* _screen)
{
	screen = _screen;
}

void Player::setRenderer(SDL_Renderer* _renderer)
{
	renderer = _renderer;
	if(renderer == NULL) setVelX(-3);
}

void Player::setDefaultBarrelPosition(float _barrelX, float _barrelY)
{
	defaultBarrelX = _barrelX;
	defaultBarrelY = _barrelY;
}

void Player::AddWeaponSprite(CSprite* weaponSprite)
{
	weaponSprites.push_back(weaponSprite);
}

double Player::toDegrees(double* angle)
{
	*angle *= 180/M_PI;
	return *angle;
}

double Player::toRadians(double* angle)
{
	*angle *= M_PI/180;
	return *angle;
}

float Player::getBarrelX(void)
{
	return barrelX;
}

float Player::getBarrelY(void)
{
	return barrelY;
}

C4* Player::getC4()
{
	return c4;
}

void Player::init(SDL_Renderer* renderer)
{
	c4 = new C4(0,0,xOffset,yOffset,this,renderer);
	pacmanGun = new CSprite(renderer,"pacmanGunFull.png",0,0,24,56);
	hand = new CSprite(renderer,"hand.png",0,0,24,56);
}

bool Player::isDead(void)
{
	return dead;
}

void Player::kill(void)
{
	dead = true;
}

int Player::getWeapon()
{
	return (int)weapon;
}

Player::~Player(void)
{
	std::cout << "Deleting Player..." << std::endl;
	//delete xOffset  --- Not Deleting because it is a passed pointer, this variable is used in other classes;
	//delete yOffset  ---                                        ""                                          ;
	//delete sprite;  --- Not deleting because when deleted, I get a Access violation reading location 0xFEEEFEEE.;
	                    //I don't mind not deleting this because there is only one instance of this class, and I don't think
					   //this is what's causing the memory leak
	//delete screen;  --- Not deleting this because screen is used by most other classes, and is deleted in my main class
	for(unsigned int c = 0; c < bullets.size(); c++)
	{
		delete bullets[c];
	}

	for(auto c = 0; c < knives.size(); c++)
	{
		delete knives[c];
	}

	delete c4;
	delete pacman;
	delete pacmanGun;
	delete hand;

	TTF_CloseFont(font);
	if(ammoSurface != NULL) SDL_FreeSurface(ammoSurface);
	//SDL_DestroyTexture(ammoText);
	if(weaponSurface != NULL) SDL_FreeSurface(weaponSurface);
	//SDL_DestroyTexture(weaponText);
	//SDL_FreeSurface(killedSurface); ---I commented out every line of code involving this surface, it doesn't actually exist
	//SDL_DestroyTexture(killedText); ---I commented out every line of code involving this texture, it doesn't actually exist
	if(waveSurface != NULL) SDL_FreeSurface(waveSurface);
	//SDL_DestroyTexture(waveText);
	TTF_Quit();
	std::cout << "Done." << std::endl;
	Mix_FreeChunk(gunshot);
	Mix_FreeChunk(spasShot);
}

const float Player::Default_Player_Speed = 2.0;
