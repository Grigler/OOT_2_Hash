#include "ErrLog.h"

#include <iostream>



void SQLite3::ErrLog::Print()
{
	std::cerr << "> ! " << std::endl;
}
void SQLite3::ErrLog::Print(char* _msg)
{
	std::cerr << "> ! " << _msg << std::endl;
}
void SQLite3::ErrLog::Print(const char* _msg)
{
	std::cerr << "! " << _msg << std::endl;
}