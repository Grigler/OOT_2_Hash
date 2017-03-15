#ifndef __DB__
#define __DB__

#include <vector>
#include <sstream>

//Forward declare as struct as c-library
struct sqlite3;

namespace SQLite3
{
	

	struct Table
	{
		char* m_name;
		std::vector<char*> m_columnNames;

		//Print Field names for use in SQL statement
		const char* PrintFields()
		{
			std::stringstream s;
			for(unsigned int i = 0; i < m_columnNames.size(); i++)
			{
				s << m_columnNames[i];
				if(i != m_columnNames.size() - 1)
					s << ',';
			}
			return s.str().c_str();
		}
		void PrintFieldsToSStream(std::stringstream& _s)
		{
			for(unsigned int i = 0; i < m_columnNames.size(); i++)
			{
				_s << m_columnNames[i];
				if(i != m_columnNames.size() - 1)
					_s << ',';
			}
		}

	};

	class DB
	{
	public:
		DB(); //Generic Constructor
		DB(char* _filePath);

		std::vector<Table*> m_tables;
		//CURRENTLY A DEBUG FUNCTION
		void CreateTable() { RunSQL("CREATE TABLE TEST(ID INT NOT NULL, DATA INT NOT NULL);"); }
		void CreateTable(char* _name, char* _fields[], int _fieldNum);

		//Opens Database Connection - Closes any open connection
		bool Open(char* _filePath);

		//Runs any sql input - as C-String
		bool RunSQL(char* _sql);
		void RunSQL(char* _sql, int (*_callback)(void*,int,char**,char**)); //TODO - Make these return the sql error/success as bool
		bool RunSQL(const char* _sql) {return RunSQL((char*)_sql);}

		//Toggling Succes Print
		void ToggleSuccesPrint() {m_isPrintingSuccess = !m_isPrintingSuccess;}

	private:
		sqlite3* m_db;
		bool m_isPrintingSuccess;
	};
}
#endif