#ifndef __SF__SQLITE__
#define __SF__SQLITE__

#include <vector>

#include "sqlite3.h"
#include "SF_Sqlite_Structures.h"

#include <assert.h>
#include <vector>
#include <list>

#define DEBUG_SQLITE 1

class SF_Sqlite
{
private:
	sqlite3* connection;
	std::string databaseName;

	std::list<SF_Sqlite_Row> lastResult;

public:

	const SF_CODES::ERROR connect();
	const SF_CODES::ERROR disconnect();

	const SF_CODES::ERROR createTable(const std::string& name,
								const std::vector<SF_Sqlite_Column_Type_Pair>& columns) const;

	const SF_CODES::ERROR dropTable(const std::string& name) const;

	const SF_CODES::ERROR execute(const std::string& query, const std::vector<SF_Sqlite_Parameter>& params, std::list<SF_Sqlite_Row>& result) const;
	const SF_CODES::ERROR execute(const std::string& query) const;
	const SF_CODES::ERROR execute(const std::string& query, std::list<SF_Sqlite_Row>& result) const;
	

	const SF_CODES::ERROR insertRecord(const std::string& table, const std::vector<std::string>& values) const;
	const SF_CODES::ERROR insertRecord(const std::string& table, const std::vector<SF_Sqlite_Column_Data_Pair>& values) const;

	const SF_CODES::ERROR getRecords(const std::string& table,
									 std::list<SF_Sqlite_Row>& result) const;

	const SF_CODES::ERROR getRecords(const std::string& table,
									const std::vector<std::string>& columns,
									std::list<SF_Sqlite_Row>& result) const;

	const SF_CODES::ERROR getRecords(const std::string& table,
							  const std::vector<std::string>& columns,
							  const std::vector<SF_Sqlite_Column_Data_Pair>& whereValues,
							  std::list<SF_Sqlite_Row>& result) const;

	SF_Sqlite( const std::string& dbName );
	~SF_Sqlite();
};

#endif