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
	float m_timeStamp;
	float m_fps;

	FpsDataPacket(float _t, float _fps)
	{
		m_timeStamp = _t;
		m_fps = _fps;
	}

	friend std::ostream& operator<<(std::ostream& os, const FpsDataPacket& packet)
	{
		os << packet.m_timeStamp << "," << packet.m_fps;
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
	//Ensuring single instance of data
	db.RunSQL("DROP TABLE FPS_DATA;");
	db.ToggleSuccesPrint();

	char* fields[] = {"TIME_STAMP FLOAT NOT NULL", "FPS FLOAT NOT NULL"};//Making creating a table easier
	db.CreateTable("FPS_DATA", fields, 2);
	//delete [] fields;//Cleaning fields

	bool isRunning = true;
	float dT = 0.0f, prevTStamp = SDL_GetTicks();
	float startTime = prevTStamp, lastDataPush = prevTStamp;
	long unsigned int fc = 0;

	std::vector<FpsDataPacket> dataSet;

	ParticleSystem p;
	p.InitWith(10000);

	while(isRunning)
	{
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

		//std::cout << prevTStamp << ": " << dT << " " << 1.0f/dT;
		float t = prevTStamp;
		if(t - lastDataPush >= DATA_PUSH_TIME)
		{
			//std::cout << " PUSH: " << dataSet.size();
			SQLite3::Input::InsertVecToTable(&db, db.m_tables.at(0), dataSet);
			dataSet.clear();
			
			lastDataPush = t;
		}

		dataSet.push_back(FpsDataPacket(t - startTime, 1.0f/dT));

		//std::cout << std::endl;
		//Recording time for data
		if(SDL_GetTicks() >= 120*1000)
		{
			isRunning = false;
		}

		fc++;
	}

	//db.RunSQL("SELECT * FROM FPS_DATA", callback);
	db.InMemoryToFile("FromMemory.db3");

	//getchar();

	//Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}