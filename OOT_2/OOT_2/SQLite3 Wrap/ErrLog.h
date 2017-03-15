#ifndef __ERR_LOG__
#define __ERR_LOG__

#include <iostream>

namespace SQLite3
{
	class ErrLog
	{
	public:
		static void Print();
		static void Print(char* _msg);
		static void Print(const char* _msg);
	};
}
#endif