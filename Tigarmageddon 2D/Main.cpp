#include "Main.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Player.h"
#include <time.h>
#include <cstdlib>
#include "MainMenu.h"
#include <SDL_mouse.h>
#include <SDL_main.h>
#include "MercenaryMode.h"
#include "TrainingMode.h"

#define MAX_KILL_COUNT 5
#define STARTING_POINT_X 300
#define STARTING_POINT_Y 230
#define GAP_SIZE 65
#define DEATH_LENGTH 5000

Main::Main(int width, int height, int _lvl, int gameMode) : 
	xOffset(0), 
	yOffset(0), 
	player(400,400,playerSprite, &xOffset, &yOffset,width,height,bullet,_lvl),
	number_of_stones(5),
	restart(true),
	showControls(false),
	deadtimer(-1)
{
	this->gameMode = gameMode;
	lvl = _lvl;
	wave = 0;
	stones.clear();
	tigers.clear();
	tgs.clear();
	killcount = 0;
	points = 0;
	paused = false;
	levelWidth = 3000;
	levelHeight = 3000;
	ammoBoxes.clear();
	running = true;
	w = width;
	h = height;

	screen = new Screen(w,h,&running,true);
	//player.setScreen(screen);
	player.setRenderer(screen->getRenderer());
	player.init(screen->getRenderer());

	game = nullptr;

	switch(gameMode)
	{
	case Survival:
		game = new SurvivalMode(w,h, levelWidth, levelHeight, &xOffset, &yOffset, &player, screen);
		break;
	case Mercenary:
		game = new MercenaryMode(w,h,levelWidth,levelHeight,&xOffset,&yOffset,&player,screen);
		break;
	case Training:
		game = new TrainingMode(w,h,levelWidth,levelHeight,&xOffset,&yOffset,&player,screen);
		break;
	default:
		printf("The game mode passed in from the GameModeMenu is not valid.\n");
		break;
	}

	grassSprite = new CSprite(screen->getRenderer(),"grassTexture0.bmp",0,0,width,height);
	playerSprite = new CSprite(screen->getRenderer(),"testballwhite.png",120,120,32,32);
	gradient = new CSprite(screen->getRenderer(),"gradient.png",0,0,1200,1200);
	//land = new CSprite(screen->getRenderer(),"grassMapTexture0.png",0,0,1000,1000);
	bullet = new CSprite(screen->getRenderer(),"bullet8.png",100,100,8,8);
	paintBallGun = new CSprite(screen->getRenderer(),"paintballGun.png",120,120,24,56);
	glock = new CSprite(screen->getRenderer(),"glock.png",120,120,24,56);
	skorpion = new CSprite(screen->getRenderer(),"skorpion.png",120,120,24,56);
	gatlingGun = new CSprite(screen->getRenderer(), "gatlingGun.png",120,120,24,56);
	spas = new CSprite(screen->getRenderer(),"spas12.png",120,120,48,56);
	emptySprite = new CSprite(screen->getRenderer(),"emptySprite.png",120,120,1,1);
	//stoneSprite = new CSprite(screen->getRenderer(),"stoneTexture.png",0,0,32,32);
	pauseMenuSprite = new CSprite(screen->getRenderer(),"pauseMenu.png",0,0,250,350);
	c4Sprite = new CSprite(screen->getRenderer(), "c4.png", 0,0,24,24);
	paw = new CSprite(screen->getRenderer(),"tigerpaw.png",STARTING_POINT_X,STARTING_POINT_Y,40,40);
	ammoBoxSprite = new CSprite(screen->getRenderer(),"AmmoBox.png",0,0,32,32);
	bloodSprite = new CSprite(screen->getRenderer(),"Blood.png",0,0,w,h);
	youDied = new CSprite(screen->getRenderer(),"YouDied.png",0,0,600,200);
	pacmanGunFull = new CSprite(screen->getRenderer(),"pacmanGunFull.png",0,0,24,56);
	hand = new CSprite(screen->getRenderer(),"hand.png",0,0,24,56);

	Main::CountdownX = w/2 - 64;
	Main::CountdownY = 64;

	//SET LEVEL-SPECIFIC SPRITES
	switch(lvl)
	{
	case Field:
		stoneSprite = new CSprite(screen->getRenderer(),"stoneTexture.png",0,0,32,32);
		land = new CSprite(screen->getRenderer(),"grassMapTexture0.png",0,0,1000,1000);
		break;
	case Siberia:
		stoneSprite = new CSprite(screen->getRenderer(),"ice.png",0,0,32,32);
		land = new CSprite(screen->getRenderer(),"snow0.png",0,0,1000,1000);
		break;
	default:
		printf("Invalid Level\n");
		break;
	}

	std::shared_ptr<CSprite> normalTigerExtended(new CSprite(screen->getRenderer(),"tigerextended.png",100,100,32,80));
	std::shared_ptr<CSprite> normalTigerClosed  (new CSprite(screen->getRenderer(),"tigerclosed.png",100,100,32,80));
	if(normalTigerExtended != NULL) normalTiger.push_back(normalTigerExtended);
	if(normalTigerClosed != NULL) normalTiger.push_back(normalTigerClosed);

	logoSurface = NULL;
	logoSurface = SDL_LoadBMP("tigarmageddon2D.bmp");
	SDL_SetWindowIcon(screen->getWindow(),logoSurface);
	cursorSurface = SDL_LoadBMP("crossHair.bmp");
	cursor = SDL_CreateColorCursor(cursorSurface,8,8);
	SDL_SetCursor(cursor);

	swnow = SDL_GetTicks();
	swlast = SDL_GetTicks();

	mouseX = mouseY = 0;
	player.setSprite(playerSprite);
	player.setPos(float(width/2 - player.getSprite()->getWidth()/2), float(height/2 - player.getSprite()->getHeight()/2));
	player.setSpeed(2);
	player.AddWeaponSprite(paintBallGun);
	player.AddWeaponSprite(skorpion);
	player.AddWeaponSprite(emptySprite);
	player.AddWeaponSprite(spas);
	player.AddWeaponSprite(gatlingGun);
	player.AddWeaponSprite(hand);
	player.AddWeaponSprite(glock);

	lmb = false;
	rmb = false;

	//Creating Stones on the border
	for(int c = 0; c < levelHeight/32; c++)
	{
		stones.push_back(new GameObject(float(c * 32),0.0,stoneSprite));
		stones.push_back(new GameObject(float(c * 32),float(levelHeight - 32),stoneSprite));
		stones.push_back(new GameObject(0.0,float(c * 32),stoneSprite));
		stones.push_back(new GameObject(float(levelWidth - 32),float(c * 32),stoneSprite));
	}

	//Spawning Level-Specific Stones
	int w						= stoneSprite->getWidth();
	int h						= stoneSprite->getHeight();
	int siberiaIglooHalfWidth	= 8;
	int siberiaIglooStartX		= levelWidth/2 - siberiaIglooHalfWidth * stoneSprite->getWidth();
	int siberiaIglooStartY		= levelHeight/2  - 100;

	switch(lvl)
	{
	case Field:
		for(int c = 0; c < number_of_stones; c++)
		{
			float xx = float(rand()%levelWidth);
			float yy = float (rand()%levelHeight);
			if(xx >= player.getX() && xx <= player.getX() + player.getSprite()->getWidth() && yy >= player.getY() && yy <= player.getSprite()->getHeight()) continue;
			stones.push_back(new GameObject(xx,yy,stoneSprite));
		}
		break;
	case Siberia:
		//Add Stones on top half of igloo
		for(int i = siberiaIglooStartX; i < levelWidth/2 + siberiaIglooHalfWidth * w; i += 32) stones.push_back(new GameObject(i,siberiaIglooStartY,stoneSprite));
		//Round Part
		for(auto c = 1; c <= siberiaIglooHalfWidth/2; c++)
		{
			//Left Side
			stones.push_back(new GameObject(siberiaIglooStartX - (c*w),siberiaIglooStartY + c * h,stoneSprite));
			//Right Side
			stones.push_back(new GameObject(siberiaIglooStartX + (2*siberiaIglooHalfWidth * w) + ((c-1)*w),siberiaIglooStartY + c * h,stoneSprite));
		}
		//Round Part 2
		for(auto c = 1; c <= siberiaIglooHalfWidth/2; c++)
		{
			//Left Side
			stones.push_back(new GameObject(siberiaIglooStartX - ((c + siberiaIglooHalfWidth/2)*w),siberiaIglooStartY + (siberiaIglooHalfWidth/2 + 2*c - 1) * h,stoneSprite));
			stones.push_back(new GameObject(siberiaIglooStartX - ((c + siberiaIglooHalfWidth/2)*w),siberiaIglooStartY + (siberiaIglooHalfWidth/2 + 2*c) * h,stoneSprite));
			//Right Side
			stones.push_back(new GameObject(siberiaIglooStartX + (2*siberiaIglooHalfWidth * w) + ((c-1 + siberiaIglooHalfWidth/2)*w),siberiaIglooStartY + (siberiaIglooHalfWidth/2 + 2*c - 1) * h,stoneSprite));
			stones.push_back(new GameObject(siberiaIglooStartX + (2*siberiaIglooHalfWidth * w) + ((c-1 + siberiaIglooHalfWidth/2)*w),siberiaIglooStartY + (siberiaIglooHalfWidth/2 + 2*c) * h,stoneSprite));
		}

		//Four straight stones on each side
		//Left side
		stones.push_back(new GameObject(siberiaIglooStartX - siberiaIglooHalfWidth * w + w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + h,stoneSprite));
		stones.push_back(new GameObject(siberiaIglooStartX - siberiaIglooHalfWidth * w + w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 2 * h,stoneSprite));
		stones.push_back(new GameObject(siberiaIglooStartX - siberiaIglooHalfWidth * w + w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 3 * h,stoneSprite));
		stones.push_back(new GameObject(siberiaIglooStartX - siberiaIglooHalfWidth * w + w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 4 * h,stoneSprite));
		//Right Side
		stones.push_back(new GameObject(siberiaIglooStartX + 3*siberiaIglooHalfWidth * w - 2*w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 1 * h,stoneSprite));
		stones.push_back(new GameObject(siberiaIglooStartX + 3*siberiaIglooHalfWidth * w - 2*w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 2 * h,stoneSprite));
		stones.push_back(new GameObject(siberiaIglooStartX + 3*siberiaIglooHalfWidth * w - 2*w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 3 * h,stoneSprite));
		stones.push_back(new GameObject(siberiaIglooStartX + 3*siberiaIglooHalfWidth * w - 2*w,siberiaIglooStartY + 3  * (siberiaIglooHalfWidth/2) * h + 4 * h,stoneSprite));
		
		//Horizontal Stones
		for(int c = 1; c <= siberiaIglooHalfWidth/2; c++)
		{
			//Left Side
			stones.push_back(new GameObject(siberiaIglooStartX - siberiaIglooHalfWidth*w + (1+c)*w,siberiaIglooStartY + 3*(siberiaIglooHalfWidth/2)*h + 5*h,stoneSprite));
			//Right Side
			stones.push_back(new GameObject(siberiaIglooStartX + 3*siberiaIglooHalfWidth*w - (2+c)*w,siberiaIglooStartY + 3*(siberiaIglooHalfWidth/2)*h + 5*h,stoneSprite));
		}

		//Vertical Stones
		for(int c = 1; c <= siberiaIglooHalfWidth/2; c++)
		{
			//Left Side
			stones.push_back(new GameObject(siberiaIglooStartX - siberiaIglooHalfWidth*w + (1+siberiaIglooHalfWidth/2)*w,siberiaIglooStartY+3*(siberiaIglooHalfWidth/2)*h + (5+c)*h,stoneSprite));
			//Right Side
			stones.push_back(new GameObject(siberiaIglooStartX +3*siberiaIglooHalfWidth*w - (2+siberiaIglooHalfWidth/2)*w,siberiaIglooStartY+3*(siberiaIglooHalfWidth/2)*h + (5+c)*h,stoneSprite));
		}
		
		break;
	default:
		printf("Invalid Level\n");
		break;
	}

	srand(time(NULL));
	for(auto c = 0; c < 1; c++)
	{
		ammoBoxes.push_back(std::shared_ptr<AmmoBox>(new AmmoBox(rand()%levelWidth,rand()%levelHeight,&xOffset,&yOffset,screen->getRenderer())));
	}

	//Spawning random TigerGenerators
	for(int c = 0; c < 4; c++)
	{
		Position p = {rand()%levelWidth, rand()%levelHeight};
		for(int j = 0; j < tgs.size(); j++)
		{
			do
			{
				p.x = rand()%(levelWidth-stoneSprite->getWidth()*3) + stoneSprite->getWidth();
				p.y = rand()%(levelHeight-stoneSprite->getHeight()*3) + stoneSprite->getHeight();
				for(int q = 0; q < Main::stones.size(); q++)
				{
					Position pq = {Main::stones[q]->getX(),Main::stones[q]->getY()};
					while(p == pq)
					{
						p.x = rand()%(levelWidth-stoneSprite->getWidth()*3) + stoneSprite->getWidth();
						p.y = rand()%(levelHeight-stoneSprite->getHeight()*3) + stoneSprite->getHeight();
					}
				}
			}
			while((p == tgs[j].get()->pos));
		}
		if(tgs.size() == 0)
		{
			for(int q = 0; q < Main::stones.size(); q++)
			{
				Position pq = {Main::stones[q]->getX(), Main::stones[q]->getY()};
				while(p == pq)
				{
					p.x = rand()%(levelWidth-stoneSprite->getWidth()*3) + stoneSprite->getWidth();
					p.y = rand()%(levelHeight-stoneSprite->getHeight()*3) + stoneSprite->getHeight();
				}
			}
		}
		tgs.push_back(std::shared_ptr<TigerGenerator>(new TigerGenerator(p.x,p.y,&xOffset,&yOffset,screen->getRenderer(), &player)));
		printf("[TIGER GEN POSITION] %d, %d\n",p.x,p.y);
	}
}

void Main::Start(void)
{
	running = true;
}

Main::~Main(void)
{
	std::cout << "Deleting Main..." << std::endl;
	delete grassSprite;
	delete playerSprite;
	delete gradient;
	delete screen;
	delete bullet;
	delete paintBallGun;
	delete glock;
	delete skorpion;
	delete gatlingGun;
	delete spas;
	delete land;
	delete stoneSprite;
	delete pauseMenuSprite;
	delete c4Sprite;
	delete paw;
	delete ammoBoxSprite;
	delete bloodSprite;
	delete youDied;
	delete game;
	delete pacmanGunFull;
	delete hand;
	for(unsigned int c = 0; c < stones.size(); c++)
	{
		delete stones[c];
	}
	stones.clear();
	SDL_FreeSurface(logoSurface);
	SDL_FreeSurface(cursorSurface);
	SDL_FreeCursor(cursor);
	std::cout << "Done." << std::endl;

	if(restart)
	{
		std::unique_ptr<MainMenu> menu = std::unique_ptr<MainMenu>(new MainMenu(w,h));
		menu->menuLoop();
	}
}

void Main::gameLoop(void)
{
	while(running && screen->getEvent()->type != SDL_QUIT)
	{
		if(!paused) pauseIndex = 0;
		start = SDL_GetTicks();

		screen->refresh();

		//Drawing terrain
		for(int c = 0; c < 3; c++)
		{
			for(int j = 0; j < 3; j++)
			{
				land->draw(j * 1000, c * 1000, xOffset, yOffset);
			}
		}
		for(unsigned int c = 0; c < stones.size(); c++)
		{
			stones[c]->getSprite()->draw(stones[c]->getX(),stones[c]->getY(),xOffset,yOffset);
		}

		if(!paused) player.Update();

		for(auto c = 0; c < ammoBoxes.size(); c++)
		{
			ammoBoxes[c]->Render();
		}

		for(int c = 0; c < tgs.size(); c++)
		{
			if(!paused) tgs[c]->Update();
			if(!paused) tgs[c]->Render();
		}
		//End drawing terrain

		player.Render(xOffset,yOffset);

		//Updating tigers
		for(int c = 0; c < tigers.size(); c++)
		{
			if(!paused) tigers[c]->Update();
			if(!paused) tigers[c]->Render();
			
			if(tigers[c]->getHealth() <= 0)
			{
				tigers[c--] = tigers.back();
				tigers.pop_back();
				killcount++;
				tigersKilledWave++;
				points += MercenaryMode::TigerBounty;
				ammoBoxes.push_back(std::shared_ptr<AmmoBox>(new AmmoBox(rand()%levelWidth,rand()%levelHeight,&xOffset,&yOffset,screen->getRenderer()))); //Remove?
			}
		}
		//End updating tigers

		//Update GameMode
		if(!paused) game->update();


		bool mouseButton1 = false,mouseButton3 = false;

		//Check Quit
		if(screen->getEvent()->type == SDL_QUIT)
		{
			running = false;
			std::cout << "QUIT" << std::endl;
			return;
		}

		//Handle Input
		if(!player.isDead()) handleInput4(screen);

	    if(screen->getEvent()->type == SDL_QUIT)
		{
			running = false;
			std::cout << "QUIT" << std::endl;
			return;
		}

		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = 300;
		r.h = 300;

		//Check Dead
		if(player.isDead())
		{
			bloodSprite->draw(0,0,0,0);
			youDied->draw(w/2 - 300,16,0,0);
			if(deadtimer == -1) deadtimer = SDL_GetTicks();
			else if(SDL_GetTicks() - deadtimer >= DEATH_LENGTH)
			{
				//Close Game
				restart = true;
				running = false;
				break;
			}
		}

		//Check Paused
		if(paused)
		{
			now = SDL_GetTicks();
			pauseMenuSprite->draw(w/2 - pauseMenuSprite->getWidth()/2, h/2 - pauseMenuSprite->getHeight()/2,0,0);
			paw->draw(STARTING_POINT_X  + (w - 800)/2,pauseIndex * (GAP_SIZE) + STARTING_POINT_Y + (h - 600)/2,0,0);
		}

		//Show Controls
		if(showControls)
		{
			std::unique_ptr<CSprite> cs = std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"tigarmageddon2dcontrols.png",0,0,w,h));
			cs->draw(0,0,0,0);
			screen->display();
		}

		//Control Framerate
		if(1000/100 > SDL_GetTicks() - start) SDL_Delay(1000/100 - (SDL_GetTicks() - start));

		screen->display();
		if(tgs.size() == 0) printf("*****NO MORE TIGER GENERATORS*****\n");
	}
}

void Main::handleInput4(Screen* screen)
{
	state = SDL_GetKeyboardState(NULL);
	if(!paused)
	{
		swnow = SDL_GetTicks();
		if(state[SDL_SCANCODE_A]) player.setVelX(-player.getSpeed());
		if(state[SDL_SCANCODE_D]) player.setVelX(player.getSpeed());
		if(!state[SDL_SCANCODE_A] && !state[SDL_SCANCODE_D]) player.setVelX(0);
		if(state[SDL_SCANCODE_W]) player.setVelY(-player.getSpeed());
		if(state[SDL_SCANCODE_S]) player.setVelY(player.getSpeed());
		if(!state[SDL_SCANCODE_W] && !state[SDL_SCANCODE_S]) player.setVelY(0);
		if(state[SDL_SCANCODE_0]) player.setWeapon(player.Glock);
		if(state[SDL_SCANCODE_1]) player.setWeapon(player.PaintballGun);
		if(state[SDL_SCANCODE_2]) player.setWeapon(player.Skorpion);
		if(state[SDL_SCANCODE_3]) player.setWeapon(player.Ballistic_Knife);
		if(state[SDL_SCANCODE_4]) player.setWeapon(player.Spas);
		if(state[SDL_SCANCODE_5]) player.setWeapon(player.GattlingGun);
		if(state[SDL_SCANCODE_Q] && swnow - swlast >= 150) 
		{
				player.setWeapon(player.getWeapon() -1);
				swlast = SDL_GetTicks();
		}
		if(state[SDL_SCANCODE_E] && swnow - swlast >= 150)
		{
				player.setWeapon(player.getWeapon() +1);
				swlast = SDL_GetTicks();
		}

		if(state[SDL_SCANCODE_A] && state[SDL_SCANCODE_W]) //UP AND LEFT
		{
			player.setVelX(-player.getSpeed()/sqrt(2));
			player.setVelY(-player.getSpeed()/sqrt(2));
		}
		else if(state[SDL_SCANCODE_D] && state[SDL_SCANCODE_W]) //UP AND RIGHT
		{
			player.setVelX(player.getSpeed()/sqrt(2));
			player.setVelY(-player.getSpeed()/sqrt(2));
		}
		else if(state[SDL_SCANCODE_A] && state[SDL_SCANCODE_S]) //DOWN AND LEFT
		{
			player.setVelX(-player.getSpeed()/sqrt(2));
			player.setVelY(player.getSpeed()/sqrt(2));
		}
		else if(state[SDL_SCANCODE_D] && state[SDL_SCANCODE_S]) //DOWN AND RIGHT
		{
			player.setVelX(player.getSpeed()/sqrt(2));
			player.setVelY(player.getSpeed()/sqrt(2));
		}

		if(state[SDL_SCANCODE_SPACE])
		{
			if(!player.getC4()->render)
			{
				if(player.getC4()->ReadyToClick() && player.hasC4) {
					player.PlaceC4(screen);
					player.hasC4 = false;
				}
			}else 
			{
				if(player.getC4()->ReadyToClick())player.getC4()->Detonate();
				for(int c  = 0; c < 5; c++)
				{
					particles.push_back(std::unique_ptr<Particle>(new Particle(player.getX(),player.getY(),sin(c * 30), cos(c * 30),screen)));
				}
			}
		}
	}

	if(state[SDL_SCANCODE_ESCAPE] && showControls) showControls = false;
	if((state[SDL_SCANCODE_P] || state[SDL_SCANCODE_ESCAPE]) && paused == false) paused = true;
	if(state[SDL_SCANCODE_UP] && now - last > 125)
	{
		if(pauseIndex != 0)
		{
			pauseIndex--;
		}else {
			pauseIndex = 3;
		}
		last = SDL_GetTicks();
	}
	if(state[SDL_SCANCODE_DOWN] && now - last > 125)
	{
		if(pauseIndex != 3)
		{
			pauseIndex++;
		}else 
		{
			pauseIndex = 0;
		}
		last = SDL_GetTicks();
	}

	if(state[SDL_SCANCODE_RETURN] && paused)
	{
		switch(pauseIndex)
		{
		case 0:
			paused = false;
			break;
		case 1:
			showControls = true;
			break;
		case 2:
			restart = true;
			running = false;
			break;
		case 3:
			running = false;
			exit(0);
			break;
		default:
			break;
		}
	}

	if(state[SDL_SCANCODE_DELETE])
	{
		Mix_HaltMusic();
		playMusic = !playMusic;
	}

	double xcomponent,ycomponent,xVel,yVel;
	double angle;
	SDL_GetMouseState(&mouseX,&mouseY);
	xcomponent = mouseX - (player.getBarrelX() + xOffset);
	ycomponent = mouseY - (player.getBarrelY() + yOffset);
	angle = atan2(ycomponent,xcomponent);
	xVel = cos(angle);
	yVel = sin(angle);

	if(!paused)
	{
		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(1)) player.Shoot(1,xVel,yVel,screen);
		else player.cock(1);
		if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(3)) player.Shoot(3,xVel,yVel,screen);
		else player.cock(3);
		if(!(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(1)) && !(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(3)))
		{
			player.cock(1);
			player.cock(3);
		}
	}
	if(SDL_GetMouseState(NULL,NULL)&SDL_BUTTON(2)) Mix_HaltMusic();
}

int Main::getWidth(void)
{
	return w;
}

int Main::getHeight(void)
{
	return h;
}

int Main::wave = 0;
const int Main::AMOUNT_OF_KEYS = 100;
const int Main::level_width = 3000;
const int Main::level_height = 3000;

const int Main::ExplosionRadius = 50;

GameObject* a[] = {nullptr};
std::vector<GameObject*> Main::stones;
std::vector<std::shared_ptr<Tiger>> Main::tigers;
std::vector<std::shared_ptr<AmmoBox>> Main::ammoBoxes;
std::vector<std::shared_ptr<TigerGenerator>> Main::tgs;

int Main::killcount = 0;
int Main::points = 0;
int Main::tigersAlive = 0;
int Main::tigersKilledWave = 0;
int Main::tigersSpawnedWave = 0;
bool Main::spawning = true;

bool Main::paused = false;
int Main::lvl = Main::Siberia;

int Main::CountdownX;
int Main::CountdownY;

bool Main::playMusic = true;
int Main::gameMode;