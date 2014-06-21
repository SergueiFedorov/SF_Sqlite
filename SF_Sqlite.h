#ifndef __SF__SQLITE__
#define __SF__SQLITE__

#include <vector>

#include "sqlite3.h"
#include "SF_Sqlite_Container.h"

#include <assert.h>
#include <vector>
#include <list>

#define DEBUG_SQLITE 1

typedef bool SF_Bool;

class SF_Sqlite
{
private:
	sqlite3* connection;
	std::string databaseName;
    
    //If this is true, the connection is not automatically disconnected on destruction or when "disconnect" is called.
    //The connection is not orignally owned by this object, and so it is not it's reponsibility to handle
    bool isExernalConnection;
    
public:

	//DOES: Saves the name of the database. Does not automatically open a connection to the database.
	SF_Sqlite( const std::string& dbName );
    
    //DOES: Assumes the connection is not owned by this object.
    //WARNING: This object will not close this connection as it does not own it. You are responsble for this.
    SF_Sqlite( sqlite3* externalConnection );
    SF_Sqlite( const SF_Sqlite& copy);

	//DOES: Connects to the datase. Constructor does not open a connection to the database.
	//ERRORS: ALREADY_CONNECTED
	const SF_SQLITE_CODES::ERRORS connect();

	//DOES: Disconnects the database. Destructor will attempt to disconnect from the database
	//ERRORS: NOT_CONNECTED
	const SF_SQLITE_CODES::ERRORS disconnect();

	//DOES: Creates a table based on the SF_Sqlite_Column_Type_Pair data
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: CREATE TABLE Example ( ID PRIMARY KEY AUTOINCREMENT, NAME TEXT )
	const SF_SQLITE_CODES::ERRORS createTable(const std::string& name,
									  const std::vector<SF_Sqlite_Column_Type_Pair>& columns) const;

	//DOES: Drops a particular table.
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: DROP TABLE Example
	const SF_SQLITE_CODES::ERRORS dropTable(const std::string& name) const;

	//DOES: Executes the query without packaging the result
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: INSERT INTO Example (Name) values ('Serguei Fedorov')
	const SF_SQLITE_CODES::ERRORS execute(const std::string& query) const;

	//DOES: Executes a query and adds the results to the list
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//INPUT QUERY: SELECT ID, NAME FROM Example
	const SF_SQLITE_CODES::ERRORS execute(const std::string& query, SF_Sqlite_Result& result) const;

	//DOES: Processes a query based on the parameters passed in. Executes the resulting query and adds the results to the list
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//INPUT QUERY: SELECT ID, NAME FROM Example WHERE NAME = {0}
	//OUTPUT QUERY: SELECT ID, NAME FROM Example WHERE NAME = 'Serguei Fedorov'
	const SF_SQLITE_CODES::ERRORS execute(const std::string& query, const std::vector<SF_Sqlite_Parameter>& params, SF_Sqlite_Result& result) const;
	
	//DOES: Executes a scalar query which returns a single int result
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//INPUT QUERY: SELECT count(*) FROM Example
	const SF_SQLITE_CODES::ERRORS executeScalar(const std::string& query, int& result) const;

	//DOES: Executes a scalar query which returns a single int result
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//INPUT QUERY: SELECT count(*) FROM Example
	const SF_SQLITE_CODES::ERRORS executeScalar(const std::string& query, char& result) const;

	//DOES: Executes a scalar query which returns a single int result
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//INPUT QUERY: SELECT Name FROM Example LIMIT 1
	const SF_SQLITE_CODES::ERRORS executeScalar(const std::string& query, char*& result) const;

	//DOES: Executes a query and returns the row count
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: SELECT * FROM Example
	const SF_SQLITE_CODES::ERRORS executeCount(const std::string& query, int& result);

	//DOES: Inserts values into table
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: INSERT INTO Example VALUES ( 0, 'Serguei Fedorov')
	const SF_SQLITE_CODES::ERRORS insertRecord(const std::string& table, const std::vector<std::string>& values) const;

	//DOES: Inserts values into the table based on the column/data pairs
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: INSERT INTO Example (ID, Name) VALUES ( 0, 'Serguei Fedorov')
	const SF_SQLITE_CODES::ERRORS insertRecord(const std::string& table, const std::vector<SF_Sqlite_Column_Data_Pair>& values) const;

	//DOES: Gets all results from table
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: SELECT * FROM Example
	const SF_SQLITE_CODES::ERRORS getRecords(const std::string& table,
                                             SF_Sqlite_Result& result) const;

	//DOES: Gets all results from table for the particular columns
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: SELECT ID, Name FROM Example
	const SF_SQLITE_CODES::ERRORS getRecords(const std::string& table,
									const std::vector<std::string>& columns,
									SF_Sqlite_Result& result) const;

	//DOES: Gets all results from table for the particular columns and where clause
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: SELECT ID, Name FROM Example where Name = 'Serguei Fedorov'
	const SF_SQLITE_CODES::ERRORS getRecords(const std::string& table,
									  const std::vector<std::string>& columns,
									  const std::vector<SF_Sqlite_Column_Data_Pair>& whereValues,
									  SF_Sqlite_Result& result) const;

	//DOES: Returns a bool for whether or not a table exists
	//ERRORS: NOT_CONNECTED, QUERY_FAIL
	//OUTPUT QUERY: SELECT count(*) FROM sqlite_master WHERE type='table' AND name='Example'
	const SF_SQLITE_CODES::ERRORS tableExists(const std::string& table, SF_Bool& result) const;
    
     SF_Sqlite& operator=(const SF_Sqlite& other);
    
    
	~SF_Sqlite();
};

#endif