#include "DB.h"

//sqlite3 include
#include "sqlite3.h"

//stdlib includes
#include <iostream>
#include <sstream>

//Custom Error Logging Class
#include "ErrLog.h"

//Simply initialises variables
SQLite3::DB::DB()
{
	m_db = NULL;

	#ifndef SQLITE3_NO_SUCCES_PRINT
	m_isPrintingSuccess = true;
	#else
	m_isPrintingSuccess = false;
	#endif
}
//Initialises variables as well opening DB at filepath
SQLite3::DB::DB(char* _filePath)
{
	m_db = NULL;

	#ifndef SQLITE3_NO_SUCCES_PRINT
	m_isPrintingSuccess = true;
	#else
	m_isPrintingSuccess = false;
	#endif

	if(Open(_filePath) == false)
	{
		std::stringstream s;
		//Building Error message
		s << "> ! Failed To Open DB: " << _filePath << std::endl;
		//Printing with static function
		ErrLog::Print(s.str().c_str());

		s.str("");
	}
	else
	{
		std::cout << "> Opened DB: " << _filePath << std::endl;
	}

}

void SQLite3::DB::CreateTable(char* _name, char* _fields[], int _fieldNum)
{
	std::stringstream s;

	Table t;

	s << "CREATE TABLE " << _name << "(";

	//Sanitising input of data type and any previous data by adding only until white-space is found
	for (int i = 0; i < _fieldNum; i++)
	{
		s << _fields[i];

		char c[2] = {NULL}; //Space of 2 to store character and null
		int cC = 0; //Counter to iterate through fieldname
		char f[200] = {NULL}; //initialises with enough space for 199 characters (and 1 null)
		while(c[0] != ' ')
		{
			c[0] = _fields[i][cC];
			if(c[0] == 0 || c[0] == ' ') //Breaks on null pointer or white-space
				break;

			strcat(f, c); //Appending new character and null

			cC++;
		}

		//Creates a new char* pointer to store in the columnNames vector as previous will go out of scope
		char* fName = new char;
		strcpy(fName, f);
		t.m_columnNames.push_back(fName);

		if(i != _fieldNum - 1)
			s << ", ";
		else
			s << ");";
	}

	if(RunSQL(s.str().c_str()))
	{
		t.m_name = _name;
		m_tables.push_back(new Table(t));
	}
}

//Opens/Makes the database at that filepath, closes the currently open database if it is open
bool SQLite3::DB::Open(char* _filePath)
{
	if(m_db != NULL)
	{
		sqlite3_close(m_db);
	}

	return !sqlite3_open(_filePath, &m_db);
}

bool SQLite3::DB::RunSQL(char* _sql)
{
	//Checking for null pointer
	if(m_db)
	{
		char* err = 0;
		
		//Runs without callback function as it is not needed (I think)
		if(sqlite3_exec(m_db,	_sql, 0, 0, &err) == SQLITE_OK)
		{
			if(m_isPrintingSuccess)
			{
				std::cout << "> Success - " << _sql << std::endl;
			}
			return true;
		}
		else
		{
			if(err)
				ErrLog::Print(err);
			else
				ErrLog::Print("Unknown Error");

			return false;
		}
	}
	else
	{
		ErrLog::Print("No Database Open");
		return false;
	}
	

}
void SQLite3::DB::RunSQL(char* _sql, int (*_callback)(void*,int,char**,char**))
{
	//Checking for null pointer
	if(m_db)
	{
		char* err = 0;
		
		//Runs with passed callback function
		if(sqlite3_exec(m_db,	_sql, _callback, 0, &err) == SQLITE_OK)
		{
			if(m_isPrintingSuccess)
			{
				std::cout << "> Success - " << _sql << std::endl;
			}
		}
		else
		{
			if(err)
				ErrLog::Print(err);
			else
				ErrLog::Print("Unknown Error");
		}
	}
	else
	{
		ErrLog::Print("No Database Open");
	}
}

void SQLite3::DB::InMemoryToFile(char* _fileName)
{
	sqlite3* fDB;
	sqlite3_open(_fileName, &fDB);

	sqlite3_backup* backupOBJ = sqlite3_backup_init(fDB, "main", m_db, "main");
	if(backupOBJ)
	{
		(void)sqlite3_backup_step(backupOBJ, -1);
		(void)sqlite3_backup_finish(backupOBJ);
	}
	else
		ErrLog::Print("Failed To Write To File");
}
