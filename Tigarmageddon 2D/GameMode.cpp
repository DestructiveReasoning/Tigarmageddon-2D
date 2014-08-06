#include "GameMode.h"


GameMode::GameMode(void)
{
	if(playList.size() == 0)
	{
		playList.push_back(Mix_LoadMUS("music/MyOathToMadness.mp3"));
		playList.push_back(Mix_LoadMUS("music/Swamphell.mp3"));
		playList.push_back(Mix_LoadMUS("music/FailuresConquest.mp3"));
		playList.push_back(Mix_LoadMUS("music/Gateways.mp3"));
		playList.push_back(Mix_LoadMUS("music/LakeBodom.mp3"));
		playList.push_back(Mix_LoadMUS("music/SleepingStars.mp3"));
		playList.push_back(Mix_LoadMUS("music/WeAreTheNightmare.mp3"));
		playList.push_back(Mix_LoadMUS("music/TheyWillReturn.mp3"));
		playList.push_back(Mix_LoadMUS("music/ForVictoryOrDeath.mp3"));
		playList.push_back(Mix_LoadMUS("music/12Gauge.mp3"));
		playList.push_back(Mix_LoadMUS("music/BeyondTheDarkSun.mp3"));
		playList.push_back(Mix_LoadMUS("music/DeadByDawn.mp3"));

		for(int c = 0; c < playList.size(); c++)
		{
			if(playList[c] == nullptr)
			{
				printf("Music file(s) not found.\n");
				playList[c] = playList.back();
				playList.pop_back();
			}
		}

		if(playList.size() == 0)
		{
			printf("No music files were found.\n");
		}
	}
}

void GameMode::update()
{
}

GameMode::~GameMode(void)
{
}

std::vector<Mix_Music*> GameMode::playList;