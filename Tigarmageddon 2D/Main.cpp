#include "Main.h"
#include "GameObject.h"
#include "Bullet.h"
#include "Player.h"
#include <time.h>
#include <cstdlib>
#include "MainMenu.h"

#define MAX_KILL_COUNT 5
#define STARTING_POINT_X 300
#define STARTING_POINT_Y 230
#define GAP_SIZE 65
#define DEATH_LENGTH 5000

Main::Main(int width, int height) : 
	xOffset(0), 
	yOffset(0), 
	player(400,400,playerSprite, &xOffset, &yOffset,width,height,bullet),
	number_of_stones(5),
	restart(true),
	showControls(false),
	deadtimer(-1)
{
	wave = 1;
	stones.clear();
	tigers.clear();
	killcount = 0;
	paused = false;
	levelWidth = 3000;
	levelHeight = 3000;
	ammoBoxes.clear();
	running = true;
	w = width;
	h = height;

	screen = new Screen(w,h,&running);
	//player.setScreen(screen);
	player.setRenderer(screen->getRenderer());
	player.init(screen->getRenderer());

	grassSprite = new CSprite(screen->getRenderer(),"grassTexture0.bmp",0,0,width,height);
	playerSprite = new CSprite(screen->getRenderer(),"testballwhite.png",120,120,32,32);
	gradient = new CSprite(screen->getRenderer(),"gradient.png",0,0,1200,1200);
	land = new CSprite(screen->getRenderer(),"grassMapTexture0.png",0,0,1000,1000);
	bullet = new CSprite(screen->getRenderer(),"bullet8.png",100,100,8,8);
	paintBallGun = new CSprite(screen->getRenderer(),"paintballGun.png",120,120,24,56);
	glock = new CSprite(screen->getRenderer(),"glock.png",120,120,24,56);
	skorpion = new CSprite(screen->getRenderer(),"skorpion.png",120,120,24,56);
	gatlingGun = new CSprite(screen->getRenderer(), "gatlingGun.png",120,120,24,56);
	spas = new CSprite(screen->getRenderer(),"spas12.png",120,120,48,56);
	emptySprite = new CSprite(screen->getRenderer(),"emptySprite.png",120,120,1,1);
	stoneSprite = new CSprite(screen->getRenderer(),"stoneTexture.png",0,0,32,32);
	pauseMenuSprite = new CSprite(screen->getRenderer(),"pauseMenu.png",0,0,250,350);
	c4Sprite = new CSprite(screen->getRenderer(), "c4.png", 0,0,24,24);
	paw = new CSprite(screen->getRenderer(),"tigerpaw.png",STARTING_POINT_X,STARTING_POINT_Y,40,40);
	ammoBoxSprite = new CSprite(screen->getRenderer(),"AmmoBox.png",0,0,32,32);
	bloodSprite = new CSprite(screen->getRenderer(),"Blood.png",0,0,800,800);
	youDied = new CSprite(screen->getRenderer(),"YouDied.png",0,0,600,200);

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

	mouseX = mouseY = 0;
	player.setSprite(playerSprite);
	player.setPos(float(width/2 - player.getSprite()->getWidth()/2), float(height/2 - player.getSprite()->getHeight()/2));
	player.setSpeed(2);
	player.AddWeaponSprite(paintBallGun);
	player.AddWeaponSprite(glock);
	player.AddWeaponSprite(skorpion);
	player.AddWeaponSprite(gatlingGun);
	player.AddWeaponSprite(spas);
	player.AddWeaponSprite(emptySprite);

	wave = 1;

	lmb = false;
	rmb = false;

	for(int c = 0; c < AMOUNT_OF_KEYS; c++)
	{
		//keys[c] = false;
	}

	for(int c = 0; c < levelHeight/32; c++)
	{
		stones.push_back(new GameObject(float(c * 32),0.0,stoneSprite));
		stones.push_back(new GameObject(float(c * 32),float(levelHeight - 32),stoneSprite));
		stones.push_back(new GameObject(0.0,float(c * 32),stoneSprite));
		stones.push_back(new GameObject(float(levelWidth - 32),float(c * 32),stoneSprite));
	}

	srand(time(NULL));
	for(int c = 0; c < number_of_stones; c++)
	{
		float xx = float(rand()%levelWidth);
		float yy = float (rand()%levelHeight);
		if(xx >= player.getX() && xx <= player.getX() + player.getSprite()->getWidth() && yy >= player.getY() && yy <= player.getSprite()->getHeight()) continue;
		stones.push_back(new GameObject(xx,yy,stoneSprite));
	}

	//tigers.push_back(std::unique_ptr<Tiger>(new Tiger(screen->getRenderer(),normalTiger,300,300,&xOffset,&yOffset,&player)));
	tigers.push_back(std::shared_ptr<Tiger>(new Tiger(screen->getRenderer(),normalTiger,100,100,&xOffset,&yOffset,&player)));

	for(auto c = 0; c < 1; c++)
	{
		ammoBoxes.push_back(std::shared_ptr<AmmoBox>(new AmmoBox(rand()%levelWidth,rand()%levelHeight,&xOffset,&yOffset,screen->getRenderer())));
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

		player.Render(xOffset,yOffset);

		for(int c = 0; c < tigers.size(); c++)
		{
			if(!paused) tigers[c]->Update();
			tigers[c]->Render();
			
			if(tigers[c]->getHealth() <= 0)
			{
				tigers[c--] = tigers.back();
				tigers.pop_back();
				killcount++;
				if(killcount < 20)
				{
					tigers.push_back(std::shared_ptr<Tiger>(new Tiger(screen->getRenderer(),normalTiger,rand()%2700 + 80, rand()%2700 + 80,&xOffset,&yOffset,&player)));
				}
				ammoBoxes.push_back(std::shared_ptr<AmmoBox>(new AmmoBox(rand()%levelWidth,rand()%levelHeight,&xOffset,&yOffset,screen->getRenderer())));
			}
		}

		bool mouseButton1 = false,mouseButton3 = false;

		if(screen->getEvent()->type == SDL_QUIT)
		{
			running = false;
			std::cout << "QUIT" << std::endl;
			return;
		}

		handleInput4(screen);

	    if(screen->getEvent()->type == SDL_QUIT)
		{
			running = false;
			std::cout << "QUIT" << std::endl;
			return;
		}

		for(int c = 0; c < particles.size(); c++)
		{
			//particles[c]->Update();
			//particles[c]->Render();
		}

		SDL_Rect r;
		r.x = 0;
		r.y = 0;
		r.w = 300;
		r.h = 300;

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

		if(paused)
		{
			now = SDL_GetTicks();
			pauseMenuSprite->draw(w/2 - pauseMenuSprite->getWidth()/2, h/2 - pauseMenuSprite->getHeight()/2,0,0);
			paw->draw(STARTING_POINT_X,pauseIndex * GAP_SIZE + STARTING_POINT_Y,0,0);
		}

		if(showControls)
		{
			std::unique_ptr<CSprite> cs = std::unique_ptr<CSprite>(new CSprite(screen->getRenderer(),"tigarmageddon2dcontrols.png",0,0,w,h));
			cs->draw(0,0,0,0);
			screen->display();
		}
		if(1000/100 > SDL_GetTicks() - start) SDL_Delay(1000/100 - (SDL_GetTicks() - start));
		s.str(std::string());
		s.clear();
		s<<"Tigers Killed: "<<killcount;
		SDL_SetWindowTitle(screen->getWindow(),s.str().c_str());

		screen->display();
	}
}

void Main::handleInput4(Screen* screen)
{
	state = SDL_GetKeyboardState(NULL);
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

	double xcomponent,ycomponent,xVel,yVel;
	double angle;
	SDL_GetMouseState(&mouseX,&mouseY);
	xcomponent = mouseX - (player.getBarrelX() + xOffset);
	ycomponent = mouseY - (player.getBarrelY() + yOffset);
	angle = atan2(ycomponent,xcomponent);
	xVel = cos(angle);
	yVel = sin(angle);

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

int Main::getWidth(void)
{
	return w;
}

int Main::getHeight(void)
{
	return h;
}

int Main::wave = 1;
const int Main::AMOUNT_OF_KEYS = 100;
const int Main::level_width = 3000;
const int Main::level_height = 3000;

const int Main::ExplosionRadius = 50;

GameObject* a[] = {nullptr};
std::vector<GameObject*> Main::stones;
std::vector<std::shared_ptr<Tiger>> Main::tigers;
std::vector<std::shared_ptr<AmmoBox>> Main::ammoBoxes;
int Main::killcount = 0;
bool Main::paused = false;