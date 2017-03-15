#include <SDL.h>
#include <glm.hpp>

#include "SQLite3 Wrap\SQLite3Wrap.h"

#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

#define DATA_PUSH_TIME 64

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
	int m_timeStamp;
	float m_fps;

	FpsDataPacket(int _t, float _fps)
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

	

	SQLite3::DB db("Test.db3");
	db.RunSQL("PRAGMA synchronous=OFF;");
	//Ensuring single instance of data
	db.RunSQL("DROP TABLE FPS_DATA;");
	db.ToggleSuccesPrint();

	char* fields[] = {"TIME_STAMP INT NOT NULL", "FPS FLOAT NOT NULL"};//Making creating a table easier
	db.CreateTable("FPS_DATA", fields, 2);
	//delete [] fields;//Cleaning fields

	bool isRunning = true;
	int dT = 0, prevTStamp = SDL_GetTicks();
	int startTime = prevTStamp, lastDataPush = prevTStamp;

	while(isRunning)
	{
		//SwapWindow
		SDL_RenderPresent(renderer);

		for (int i = 0; i < rand()%10; i++)
		{
			for (int j = 0; j < rand()%10; j++)
			{
				int a;
			}
		}

		//Clear
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		//Time stuff
		dT = SDL_GetTicks() - prevTStamp;
		prevTStamp = SDL_GetTicks();

		std::cout << 1000.0f/dT << std::endl;
		int t = prevTStamp;
		if(t - lastDataPush >= DATA_PUSH_TIME)
		{
			SQLite3::Input::InsertToTable(&db, db.m_tables.at(0), FpsDataPacket(t - startTime, (1000.0f/dT)));
			lastDataPush = t;
		}

		if(SDL_GetTicks() >= 15*1000)
		{
			isRunning = false;
		}
	}

	db.RunSQL("SELECT * FROM FPS_DATA", callback);

	getchar();

	//Cleanup
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}