#ifndef __DATA__
#define __DATA__

//stdlib icnludes
#include <iostream>
#include <sstream>
#include <vector>

#include "SQLite3 Wrap\DB.h"

namespace SQLite3
{
	//struct Table;
	/*
	//Requires Override of << operator to convert data into SQL format
	//for dataT
	template <class dataT> 
	class Data
	{
		public:
			Data();

			void InsertToTable(DB* _db, Table* _table);
			//DEBUG FUNCTION
			void InsertToTable(DB* _db);

			void Assign(dataT _d);
			
		private:

			dataT* m_data;
			//Member variable to avoid scope GC overhead
			std::stringstream m_s;
	};

	template<class dataT>
	Data<dataT>::Data()
	{
		m_data = NULL;
	}
	
	template<class dataT> 
	void Data<dataT>::InsertToTable(DB* _db, Table* _table)
	{
		m_s.str("");

		m_s << "INSERT INTO " << _table.m_name << "(" << _table.PrintFields() << "); VALUES(" << *m_data << ");" << std::endl;
		
		_db->RunSQL(m_s.str().c_str());
	}
	//DEBUG FUNCTION
	template<class dataT>
	void Data<dataT>::InsertToTable(DB* _db)
	{
		m_s.str("");

		m_s << "INSERT INTO TEST(DATA) VALUES(" << *m_data << ");" << std::endl;

		_db->RunSQL(m_s.str().c_str());
	}

	template<class dataT>
	void Data<dataT>::Assign(dataT _d)
	{
		if(m_data == NULL)
			m_data = new dataT;
		
		*m_data = _d;
	}
	*/
	namespace Input
	{
		template<class dataT>
		void InsertToTable(DB* _db, Table* _table, dataT _inp)
		{
			std::stringstream s;

			s << "INSERT INTO " << _table->m_name << "("; 
			_table->PrintFieldsToSStream(s); //Printing field names directly to stream
			s << ") VALUES(" << _inp << ");" << std::endl;

			#ifdef SQLITE3_DEBUG_PRINT
			std::cout << "Debug: " << s.str().c_str() << std::endl;
			#endif

			_db->RunSQL(s.str().c_str());
		}

		template<class dataT>
		void InsertVecToTable(DB* _db, Table* _table, std::vector<dataT>& _inp)
		{
			std::stringstream s;
			for (unsigned int i = 0; i < _inp.size(); i++)
			{
				s << "INSERT INTO " << _table->m_name << "(";
				_table->PrintFieldsToSStream(s);
				s << ") VALUES(" << _inp.at(i) << ");";
			}

			#ifdef SQLITE3_DEBUG_PRINT
			std::cout << "Debug: " << s.str().c_str() << std::endl;
			#endif
			_db->RunSQL(s.str().c_str());
		}
	}
}
#endif