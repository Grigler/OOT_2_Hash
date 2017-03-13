#include <SDL.h>

#include <iostream>

//#include "Particle.h"
#include "ParticleSystem.h"

#define WIDTH 1280
#define HEIGHT 720

#undef main

int main()
{
	//Initialisation
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Fail" << std::endl;
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Graham Rigler - OOT 2", 250, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);

	bool isRunning = true;
	int dT = 0, prevTStamp = SDL_GetTicks();
	
	ParticleSystem p;
	p.InitWithCount(10000, WIDTH, HEIGHT);

	while(isRunning)
	{
		//Hash particle positions
		
		//Check collision - Updating Particle Positions + velocity
		p.Update(dT);

		//Draw Particles
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		p.Draw(renderer);

		//SwapWindow
		SDL_RenderPresent(renderer);

		//Clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//Time stuff
		dT = SDL_GetTicks() - prevTStamp;
		prevTStamp = SDL_GetTicks();

		std::cout << 1000.0f/dT << std::endl;
	}

	//Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}