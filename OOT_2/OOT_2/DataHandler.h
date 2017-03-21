#ifndef __DATA_HANDLER__
#define __DATA_HANDLER__

#include <SQLite3Wrap.h>

#include <iostream>
#include <vector>

//Structures to allow easier formatting of data into 'packets'
//with overloaded '<<' to write data 
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
struct AverageFPSDataPacket
{
	int m_pCount;
	float m_fps;

	AverageFPSDataPacket(int _pC, float _fps)
	{
		m_pCount = _pC;
		m_fps = _fps;
	}

	friend std::ostream& operator<<(std::ostream& os, const AverageFPSDataPacket& packet)
	{
		os << packet.m_pCount << "," << packet.m_fps;
		return os;
	}
};

//Providing an interface with the SQLite3 Wrapper
class DataHandler
{
public:
	DataHandler();
	~DataHandler();

	//Buffer in memory of data to be pushed to the db in batches
	std::vector<FpsDataPacket> m_fpsDataVec;

	//Connection to In-memory database
	SQLite3::DB* m_db;
	//References to tables in db - cleaned up in database
	SQLite3::Table* m_fpsTable;
	SQLite3::Table* m_avgFpsTable;

	//Adds a new sample to the local data vector
	void SampleFPS(int _pCount, float _tStamp, float _fps);
	//Uses Private callback function to calculate and sample average fps for a given particle count
	void SampleAverageFPS(int _pCount);
	//Pushes local data vec to DB and clears it
	void PushDataVec();
	//Dumps DB to the given filepath (must be .db3 extension)
	void DumpToFile(char* _to);

protected:
	//callback must be static (due to how SQLite3 works), so must be interacted with static vars
	static float s_fpsTotal;
	static unsigned int s_sampleCount;
	static int AverageFPSCallback(void* _na, int argc, char** argv, char** azColName);
};



#endif