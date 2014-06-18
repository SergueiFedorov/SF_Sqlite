#ifndef __SF__SQLITE__STRUCTURES__
#define __SF__SQLITE__STRUCTURES__

#include <string>
#include <list>

namespace SF_CODES
{
	enum ERROR { NO_ERROR = 0x0000, FAIL, QUERY_FAIL, ALREADY_CONNECTED, NOT_CONNECTED, CANNOT_CONNECT };
};

namespace SF_COLUMN_FLAGS
{
	enum FLAG { NONE = 0x0000, PRIMARY_KEY = 1, AUTOINCREMENT = 2 };
};

namespace SF_COLUMN_TYPES
{
	const std::string TEXT("TEXT");
	const std::string NUMERIC("NUMERIC");
	const std::string INTEGER("INTEGER");
	const std::string REAL("REAL");
};

struct SF_Sqlite_Parameter
{
	SF_Sqlite_Parameter() { }
	SF_Sqlite_Parameter(int param, std::string paramData)
		: paramNum(param), data(paramData)
	{

	}

	int paramNum;
	std::string data;
};

struct SF_Sqlite_Column_Type_Pair
{
	std::string columnName;
	std::string columnType;
	int columnFlags;

	SF_Sqlite_Column_Type_Pair() { }
	SF_Sqlite_Column_Type_Pair(const std::string column, const std::string type, const int flags = SF_COLUMN_FLAGS::FLAG::NONE)
		: columnName(column), columnType(type), columnFlags(flags)
	{

	}
};

struct SF_Sqlite_Column_Data_Pair
{
	std::string columnName;
	std::string columnData;

	SF_Sqlite_Column_Data_Pair() { }
	SF_Sqlite_Column_Data_Pair(const std::string column, const std::string data)
		: columnName(column), columnData(data)
	{

	}
};

struct SF_Sqlite_Cell
{
	std::string columnName;
	std::string data;
};

struct SF_Sqlite_Row
{
	std::list<SF_Sqlite_Cell> columns;
};


#endif