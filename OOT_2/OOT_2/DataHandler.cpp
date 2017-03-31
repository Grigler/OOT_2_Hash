#include "DataHandler.h"

//Assignment of static variables needed for the callback function
float DataHandler::s_fpsTotal = 0.0f;
unsigned int DataHandler::s_sampleCount = 0;

DataHandler::DataHandler(bool _isDrawing)
{
	//Opening a new data base connection, stored in virtual memory for speed
	m_db = new SQLite3::DB(":memory:");
	m_db->ToggleSuccesPrint();
	m_db->m_tables.reserve(2);

	char* fields[] = {"P_COUNT INT NOT NULL", "TIME_STAMP FLOAT NOT NULL", "FPS FLOAT NOT NULL"};//Making creating a table easier
	char* avgFields[] = {fields[0], fields[2]};

	m_db->CreateTable("FPS_DATA", fields, 3);
	m_fpsTable = m_db->m_tables[0]; //Assigning reference to new table

	m_db->CreateTable("AVG_FPS_DATA", avgFields, 2);
	m_avgFpsTable = m_db->m_tables[1]; //Assigning reference to new table

	if(_isDrawing)
	{
		m_drawInterface = new DataDrawInterface();
	}
	else
	{
		m_drawInterface = NULL;
	}
}
DataHandler::~DataHandler()
{
	delete m_db;
	delete m_drawInterface;
}

void DataHandler::SampleFPS(int _pCount, float _tStamp, float _fps)
{
	m_fpsDataVec.push_back(FpsDataPacket(_pCount, _tStamp, _fps));
}
void DataHandler::SampleAverageFPS(int _pCount)
{
	//Building SQL querry for given pCount
	std::stringstream querry;
	querry << "SELECT * FROM " << m_fpsTable->m_name << " WHERE P_COUNT = " << _pCount << ";";

	//Running selection with callback function to process data
	m_db->RunSQL((char*)querry.str().c_str(),AverageFPSCallback);
	
	//Print for user
	std::cout << "\t> Average FPS: " << s_fpsTotal/s_sampleCount << "fps\n";

	//Using that processed data to calculate a quick average and insert it into table
	SQLite3::Input::InsertToTable(m_db, m_avgFpsTable, AverageFPSDataPacket(_pCount, s_fpsTotal/s_sampleCount));
	
	//Resetting static vars for next avg
	s_fpsTotal = 0.0f;
	s_sampleCount = 0;
}
void DataHandler::PushDataVec()
{
	SQLite3::Input::InsertVecToTable(m_db, m_fpsTable, m_fpsDataVec);
	m_fpsDataVec.clear();
}
void DataHandler::PushDataVecWithDraw()
{
	SQLite3::Input::InsertVecToTable(m_db, m_fpsTable, m_fpsDataVec);

	if(m_drawInterface != NULL) //Only not null if it drawing is enabled
	{
		float bufferAverage = 0.0f;
		for (size_t i = 0; i < m_fpsDataVec.size(); i++)
		{
			bufferAverage += m_fpsDataVec.at(i).m_fps;
		}
		bufferAverage /= m_fpsDataVec.size();
		m_drawInterface->PlotFPSPoint(bufferAverage);
	}
	m_fpsDataVec.clear();
}
void DataHandler::DumpToFile(char* _to)
{
	m_db->InMemoryToFile(_to);
}
//Static callback function
int DataHandler::AverageFPSCallback(void* _na, int argc, char** argv, char** azColName)
{
	//Take all FPS collumn data and add it together and average then print at end (use atof to convert to float)
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(azColName[i], "FPS") == 0) //strcmp returns 0 for equal Cstrings
		{
			s_fpsTotal += atof(argv[i]);
			s_sampleCount++;
		}
	}
	return 0;
}