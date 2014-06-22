#ifndef __SF__SQLITE__STRUCTURES__
#define __SF__SQLITE__STRUCTURES__

#include <string>
#include <list>

namespace SF_SQLITE_CODES
{
    //0-28 are native Sqlite errors. They were presented as macros, but I have wrapped them into an enum.
    //1000+ are SF_Sqlite errors
    enum ERRORS
    {
        NO_ERROR     =0,
        ERROR        =1,   /* SQL error or missing database */
        INTERNAL     =2,   /* Internal logic error in SQLite */
        PERM         =3,   /* Access permission denied */
        ABORT        =4,   /* Callback routine requested an abort */
        BUSY         =5,   /* The database file is locked */
        LOCKED       =6,   /* A table in the database is locked */
        NOMEM        =7,   /* A malloc() failed */
        READONLY     =8,   /* Attempt to write a readonly database */
        INTERRUPT    =9,   /* Operation terminated by sqlite3_interrupt()*/
        IOERR       =10,   /* Some kind of disk I/O error occurred */
        CORRUPT     =11,   /* The database disk image is malformed */
        NOTFOUND    =12,   /* Unknown opcode in sqlite3_file_control() */
        FULL        =13,   /* Insertion failed because database is full */
        CANTOPEN    =14,   /* Unable to open the database file */
        PROTOCOL    =15,   /* Database lock protocol error */
        EMPTY       =16,   /* Database is empty */
        SCHEMA      =17,   /* The database schema changed */
        TOOBIG      =18,   /* String or BLOB exceeds size limit */
        CONSTRAINT  =19,   /* Abort due to constraint violation */
        MISMATCH    =20,   /* Data type mismatch */
        MISUSE      =21,   /* Library used incorrectly */
        NOLFS       =22,  /* Uses OS features not supported on host */
        AUTH        =23,   /* Authorization denied */
        FORMAT      =24,  /* Auxiliary database format error */
        RANGE       =25,  /* 2nd parameter to sqlite3_bind out of range */
        NOTADB      =26,   /* File opened that is not a database file */
        NOTICE      =27,   /* Notifications from sqlite3_log() */
        WARNING     =28,   /* Warnings from sqlite3_log() */
        
        //CUSTOM ERROR CODES
        
        NOT_CONNECTED =1000,
        ALREADY_CONNECTED=1001,
        QUERY_FAIL = 1002,
    };
};

namespace SF_COLLECTION_CODES
{
    enum ERRORS
    {
        NO_ERROR = 0,
        ITEM_NOT_FOUND = 1
    };
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
	SF_Sqlite_Parameter(int& param, std::string& paramData)
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
	SF_Sqlite_Column_Type_Pair(const std::string& column, const std::string& type, const int flags = SF_COLUMN_FLAGS::FLAG::NONE)
		: columnName(column), columnType(type), columnFlags(flags)
	{

	}
};

struct SF_Sqlite_Column_Data_Pair
{
	std::string columnName;
	std::string columnData;

	SF_Sqlite_Column_Data_Pair() { }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const bool& data)
        : columnName(column), columnData((data == true ? "1" : "0" ))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const int& data)
        : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const float& data)
        : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const double& data)
    : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const long& data)
    : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const long long& data)
    : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const unsigned long& data)
    : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
    SF_Sqlite_Column_Data_Pair(const std::string& column, const unsigned long long& data)
    : columnName(column), columnData(std::to_string(data))
    {
        
    }
    
	SF_Sqlite_Column_Data_Pair(const std::string& column, const std::string& data)
		: columnName(column), columnData("'" + data + "'")
	{

	}
    
    
};

struct SF_Sqlite_Cell
{
	std::string columnName;
	std::string data;
};

/*
struct SF_Sqlite_Row
{
	std::list<SF_Sqlite_Cell> columns;
};*/


#endif