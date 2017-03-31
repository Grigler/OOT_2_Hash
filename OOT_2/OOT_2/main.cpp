#include <SDL.h>
#include <glm.hpp>

#include "DataHandler.h"
#include "HashTableSystem.h"

#include <iostream>
#include <time.h>

#define DATA_PUSH_TIME 1000

//needed for SDL
#undef main

void PrintArgFormat();
int RunSystem(int _startP, int _endP, int _deltaP, int _timePerSim, bool _isDrawingData);

int main(int argc, char* argv[])
{
	if(strcmp(argv[1], "-a") == 0 && argc == 6) //strcmp returns 0 on success
	{
		int startP = atoi(argv[2]);
		int endP = atoi(argv[3]);
		int deltaP = atoi(argv[4]);
		int timePerSim = atoi(argv[5]);
		return RunSystem(startP, endP, deltaP, timePerSim, true);
	}
	else
	{
		if(argc == 5) //Num arguments + 1
		{
			int startP = atoi(argv[1]);
			int endP = atoi(argv[2]);
			int deltaP = atoi(argv[3]);
			int timePerSim = atoi(argv[4]);
			return RunSystem(startP, endP, deltaP, timePerSim, false);
		}
		else if(argc == 1) //1 with no arguments
		{
			//Run with generic values
			std::cout << "> Runnning with default: 2,000->40,000 at 4000 deltaP for 5000ms/sim.\n\n";
			return RunSystem(2000, 40000, 4000, 5000, true);
		}
		else
		{
			PrintArgFormat();
			return 0;
		}
	}
}

void PrintArgFormat()
{
	std::cout << "CommandLine Format - [optional -a] [Start P] [End P] [P rate] [Time per sim. (ms)]" << std::endl;
}

int RunSystem(int _startP, int _endP, int _deltaP, int _timePerSim, bool _isDrawingData)
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

	DataHandler dHandle(_isDrawingData);

	ParticleSystem pSys;
	int pCount = _startP;
	const int pDelta = _deltaP;

	std::cout << "\n> Starting Sim. With " << pCount << " Particles\n";
	
	pSys.InitWith(pCount);

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
		pSys.Update(dT);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF); 
		pSys.Draw(renderer);

		float t = prevTStamp;
		if(t - lastDataPush >= DATA_PUSH_TIME)
		{
			dHandle.PushDataVec();

			lastDataPush = t;
		}
		if(1.0f/dT <= 1000) //Fixing some 1.#INF issue with v. high framerates
			dHandle.SampleFPS(pCount, t - startTime, 1.0f/dT);

		//Recording time for data
		if(SDL_GetTicks() - startTime >= _timePerSim)
		{
			startTime = SDL_GetTicks();

			//Pushing all left-over data before going into next amount
			if(_isDrawingData)
				dHandle.PushDataVecWithDraw();
			else
				dHandle.PushDataVec();

			//Calculating and storing average fps for that simulation
			dHandle.SampleAverageFPS(pCount);

			pCount += pDelta;
			if(pCount > _endP)
				isRunning = false;
			else
			{
				std::cout << "\t> Clearing...\n";
				pSys.InitWith(pCount);
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
	std::cout << "\n> Dumping Data to 'ProfileData.db3'\n";
	dHandle.DumpToFile("ProfileData.db3");
	std::cout << "\t> Done\n\n";

	std::cout << "> Any key to exit...\n";
	getchar();
	std::cout << "> Cleaning up\n";

	//Cleanup
	pSys.Clear(); //Deletes all particle data
	delete pSys.m_table; //Deleting hash-table
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}