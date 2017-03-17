#include <SDL.h>
#include <glm.hpp>

#include "SQLite3 Wrap\SQLite3Wrap.h"
#include "HashTableSystem.h"

#include <iostream>
#include <time.h>

#define DATA_PUSH_TIME 1500

#undef main

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
   int i;
   for(i=0; i<argc; i++)
	 {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

struct FpsDataPacket
{
	int   m_pCount;
	float m_timeStamp;
	float m_fps;

	FpsDataPacket(int _pC, float _t, float _fps)
	{
		m_pCount = _pC;
		m_timeStamp = _t;
		m_fps = _fps;
	}

	friend std::ostream& operator<<(std::ostream& os, const FpsDataPacket& packet)
	{
		os << packet.m_pCount << "," << packet.m_timeStamp << "," << packet.m_fps;
		return os;
	}
};

int main()
{
	srand(time(NULL));

	//Initialisation
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Fail" << std::endl;
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Graham Rigler - OOT 2", 250, 100, S_WIDTH, S_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(renderer);


	SQLite3::DB db(":memory:");
	//db.RunSQL("PRAGMA synchronous=OFF;");
	db.ToggleSuccesPrint();

	char* fields[] = {"P_COUNT INT NOT NULL", "TIME_STAMP FLOAT NOT NULL", "FPS FLOAT NOT NULL"};//Making creating a table easier
	try
	{
		db.CreateTable("FPS_DATA", fields, 3);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	//delete [] fields;//Cleaning fields

	bool isRunning = true;
	float dT = 0.0f, prevTStamp = SDL_GetTicks();
	float startTime = prevTStamp, lastDataPush = prevTStamp;
	long unsigned int fc = 0;

	std::vector<FpsDataPacket> dataSet;

	ParticleSystem p;
	int pCount = 5000;

	std::cout << "> Starting Sim. With " << pCount << " Particles\n";
	
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
			SQLite3::Input::InsertVecToTable(&db, db.m_tables.at(0), dataSet);
			dataSet.clear();

			lastDataPush = t;
		}
		if(1.0f/dT <= 1000) //Fixing some 1.#INF issue with v. high framerates
			dataSet.push_back(FpsDataPacket(pCount, t - startTime, 1.0f/dT));

		//Recording time for data
		if(SDL_GetTicks() - startTime >= 12*1000)
		{
			startTime = SDL_GetTicks();

			//Pushing all left-over data before going into next amount
			SQLite3::Input::InsertVecToTable(&db, db.m_tables.at(0), dataSet);
			dataSet.clear();

			pCount += 5000;
			if(pCount >= 50000)
				isRunning = false;
			else
			{
				p.InitWith(pCount);

				std::cout << "> Starting Sim. With " << pCount << " Particles\n";

				dT = 0;
				startTime = SDL_GetTicks();
				lastDataPush = startTime;
			}
		}

		fc++;
	}

	std::cout << "> Dumping Data to 'ProfileData.db3'\n";

	db.InMemoryToFile("ProfileData.db3");

	//Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}