#include <SDL.h>
#include <glm.hpp>

#include "DataHandler.h"
#include "HashTableSystem.h"

#include <iostream>
#include <time.h>

#define DATA_PUSH_TIME 1000

//needed for SDL
#undef main

int main()
{
	//Seeding rng
	srand(time(NULL));

	//Initialisation
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL Init Failure" << std::endl;
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Graham Rigler - OOT 3", 250, 100, S_WIDTH, S_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	bool isRunning = true;
	float dT = 0.0f, prevTStamp = SDL_GetTicks();
	float startTime = prevTStamp, lastDataPush = prevTStamp;
	long unsigned int fc = 0;

	DataHandler dHandle;

	ParticleSystem p;
	int pCount = 2000;
	const int pDelta = 2000;

	std::cout << "\n> Starting Sim. With " << pCount << " Particles\n";
	
	p.InitWith(pCount);

	SDL_Event e;

	while(isRunning)
	{

		while(SDL_PollEvent(&e) != 0)
		{
			//Occurs when window's [X] is clicked
			if(e.type == SDL_QUIT) 
			{
				isRunning = false;
			}
		}  

		//SwapWindow
		SDL_RenderPresent(renderer);

		//Clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//Time stuff
		dT = (float)((SDL_GetTicks()- prevTStamp) / 1000.0f);
		prevTStamp = SDL_GetTicks();

		//Program happens here
		p.Update(dT);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); 
		p.Draw(renderer);

		float t = prevTStamp;
		if(t - lastDataPush >= DATA_PUSH_TIME)
		{
			dHandle.PushDataVec();

			lastDataPush = t;
		}
		if(1.0f/dT <= 1000) //Fixing some 1.#INF issue with v. high framerates
			dHandle.SampleFPS(pCount, t - startTime, 1.0f/dT);

		//Recording time for data
		if(SDL_GetTicks() - startTime >= 5000)
		{
			startTime = SDL_GetTicks();

			//Pushing all left-over data before going into next amount
			dHandle.PushDataVec();

			pCount += pDelta;
			if(pCount >= 30000)
				isRunning = false;
			else
			{
				//Calculating and storing average fps for that simulation
				dHandle.SampleAverageFPS(pCount - pDelta);

				p.InitWith(pCount);
				std::cout << "\n> Starting Sim. With " << pCount << " Particles\n";

				//Reseting timers to account for data processing time
				dT = 0;
				startTime = SDL_GetTicks();
				lastDataPush = startTime;
			}
		}

		fc++;
	}
	//Dumping data from memory to file
	std::cout << "> Dumping Data to 'ProfileData.db3'\n";
	dHandle.DumpToFile("ProfileData.db3");

	//Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}