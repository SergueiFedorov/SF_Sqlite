#include "SF_Sqlite.h"
#include "SF_Sqlite_Helper.h"

enum EXECUTION_TYPE { ROWS, ROWS_COUNT, SCALAR };

//Used in communication to the call back.
//Pointer points to usable data storage
struct Execution_Package
{
	EXECUTION_TYPE type;
	char* pointer;
	int resultCount;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Forward Declarations///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
static int callback(void *data, int argc, char **argv, char **ColumnNames);

namespace INTERNAL
{

static const SF_CODES::ERRORS executeScalar(sqlite3* connection, const std::string& query, char** result);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Internal Use Only//////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//Internal functions
//Hides implementation. Instead of exposing it as private or using the pimpl idiom, I just use
//a namespace with static functions. 
//NOTE: This breaks polymorphism but this class is not supposed be inherited. Use aggregation instead.

namespace INTERNAL
{

static const SF_CODES::ERRORS executeScalar(sqlite3* connection, const std::string& query, char** result)
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	//This is OK, but hacky. Too much forward dependence on how the call back works
	char holder;

	//Package to send to the callback
	Execution_Package execution;
	execution.type = EXECUTION_TYPE::SCALAR;
	execution.pointer = &holder;

	char* error = 0;
	sqlite3_exec(connection, query.c_str(), callback, &execution, &error);
	*result = execution.pointer;

	SF_SQLITE_PRINT_QUERY(query);
	SF_SQLITE_PRINT_QUERY_ERROR(error);

	return SF_CODES::ERRORS::NO_ERROR;
}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Main Implementation////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//This will not open connection as it is too risky that it may
//cause issues in the constructor. Never run code which can
//throw exeptions or cause errors in the constructors
SF_Sqlite::SF_Sqlite( const std::string & name)
	: connection(0), databaseName(name)
{

}

SF_Sqlite::SF_Sqlite( const SF_Sqlite& copy)
    : connection(copy.connection), databaseName(copy.databaseName)
{
    
}

const SF_CODES::ERRORS SF_Sqlite::connect()
{
	SF_SQLITE_IS_CONNECTED_ESCAPE(connection);

	int nativeError = sqlite3_open(this->databaseName.c_str(), &this->connection);

    SF_CODES::ERRORS sf_error = SF_SQLITE_PROCESS_SQLITE_NATIVE_ERROR(nativeError);
    
    if (sf_error == SF_CODES::ERRORS::CANTOPEN)
    {
        printf("Database failed to open \n");
    }
    
	return sf_error;
}

const SF_CODES::ERRORS SF_Sqlite::disconnect()
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	int error = sqlite3_close(this->connection);

	return SF_SQLITE_PROCESS_SQLITE_NATIVE_ERROR(error);
}

static int callback(void *data, int argc, char **argv, char **ColumnNames)
{
	Execution_Package* package = (Execution_Package*)data;
	package->resultCount = argc;

	if (argc == 0)
	{
		return 0;
	}

	if (package->type == EXECUTION_TYPE::ROWS)
	{
		std::list<SF_Sqlite_Row>* rows = (std::list<SF_Sqlite_Row>*)package->pointer;

		SF_Sqlite_Row row;
		for (int x = 0; x < argc; ++x)
		{
			SF_Sqlite_Cell cell;
			cell.columnName = ColumnNames[x];
			cell.data = argv[x];

			row.columns.push_back(cell);
		}

		rows->push_back(row);
	}
	else
	{
		*package->pointer = **argv;
	}

	return 0;
}

const SF_CODES::ERRORS SF_Sqlite::execute(const std::string& query, std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	//Package to send to the callback
	Execution_Package execution;
	execution.type = EXECUTION_TYPE::ROWS;
	execution.pointer = (char*)&result;

    char* error = 0;
	int nativeError = sqlite3_exec(this->connection, query.c_str(), callback, &execution, &error);

	SF_SQLITE_PRINT_QUERY(query);
	SF_SQLITE_PRINT_QUERY_ERROR(error);

	return SF_SQLITE_PROCESS_SQLITE_NATIVE_ERROR(nativeError);
}

const SF_CODES::ERRORS SF_Sqlite::execute(const std::string& query,
										 const std::vector<SF_Sqlite_Parameter>& params,
										 std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string finalQuery;
	sf_sqlite_buildParamedQuery(finalQuery, query, params);

	return this->execute(finalQuery, result);
}

const SF_CODES::ERRORS SF_Sqlite::execute(const std::string& query) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	char* error = 0;
	int nativeError = sqlite3_exec(this->connection, query.c_str(), 0, 0, &error);

	SF_SQLITE_PRINT_QUERY(query);
	SF_SQLITE_PRINT_QUERY_ERROR(error);

	return SF_SQLITE_PROCESS_SQLITE_NATIVE_ERROR(nativeError);
}


const SF_CODES::ERRORS SF_Sqlite::executeScalar(const std::string& query, int& result) const
{
	char* resultReturn = 0;
	SF_CODES::ERRORS error = INTERNAL::executeScalar(this->connection, query, &resultReturn);
	result = (int)*resultReturn - '0';
	return error;
}

const SF_CODES::ERRORS SF_Sqlite::executeScalar(const std::string& query, char& result) const
{
	char* resultReturn = 0;
	SF_CODES::ERRORS error = INTERNAL::executeScalar(this->connection, query, &resultReturn);
	result = (char)*resultReturn;
	return error;
}

const SF_CODES::ERRORS SF_Sqlite::executeCount(const std::string& query, int& result)
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::list<SF_Sqlite_Row> returnResults;
	SF_CODES::ERRORS error = this->execute(query, returnResults);

	if (SF_SQLITE_SUCCESS(error))
	{
		result = (int)returnResults.size();
	}

	return error;
}

const SF_CODES::ERRORS SF_Sqlite::insertRecord(const std::string& table, const std::vector<std::string>& values) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string insertIntoString;
	sf_sqlite_buildInsertIntoTableHeader(insertIntoString, table);

	std::string insertValuesString;
	sf_sqlite_buildInsertValues(insertValuesString, values);

	std::string query;
	query += insertIntoString;
	query += insertValuesString;

	SF_SQLITE_PRINT_QUERY(query);

	return this->execute(query);
}

const SF_CODES::ERRORS SF_Sqlite::insertRecord(const std::string& table, const std::vector<SF_Sqlite_Column_Data_Pair>& values) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string insertIntoString;
	sf_sqlite_buildInsertIntoTableHeader(insertIntoString, table);

	std::string insertValuesString;
	sf_sqlite_buildInsertValues(insertValuesString, values);

	std::string query;
	query += insertIntoString;
	query += insertValuesString;

	return this->execute(query);
}

const SF_CODES::ERRORS SF_Sqlite::getRecords(const std::string& table,
											std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string selectFrom;
	
	std::string columnSelectString;
	sf_sqlite_buildSelectColumnList(columnSelectString);

	std::string query;
	sf_sqlite_buildFinalSelectNoWhereQuery(query, table, columnSelectString) ;

	return this->execute(query, result);
}

const SF_CODES::ERRORS SF_Sqlite::getRecords(const std::string& table,
											const std::vector<std::string>& columns,
											std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string columnSelectString;
	sf_sqlite_buildSelectColumnList(columnSelectString, columns);

	std::string query;
	query += columnSelectString;

	return this->execute(query, result);

}

const SF_CODES::ERRORS SF_Sqlite::getRecords(const std::string& table,
											const std::vector<std::string>& columns,
											const std::vector<SF_Sqlite_Column_Data_Pair>& whereValues,
											std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string whereListString;
	sf_sqlite_buildWhereColumnList(whereListString, whereValues);

	std::string selectListString = " ";
	sf_sqlite_buildSelectColumnList(selectListString, columns);

	std::string query;
	sf_sqlite_buildFinalSelectAllQuery(query, table, selectListString, whereListString);

	return this->execute(query, result);
}

const SF_CODES::ERRORS SF_Sqlite::dropTable(const std::string& table) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string query;
	sf_sqlite_buildFinalDropTable(query, table);

	return this->execute(query);
}

const SF_CODES::ERRORS SF_Sqlite::createTable(const std::string& name,
											const std::vector<SF_Sqlite_Column_Type_Pair>& columns) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string createString;
	sf_sqlite_buildCreateTable(createString, name);

	std::string columnString;
	sf_sqlite_buildCreateTableColumns(columnString, columns);

	std::string query;
	query += createString;
	query += columnString;

	return this->execute(query);
}

const SF_CODES::ERRORS SF_Sqlite::tableExists(const std::string& table, SF_Bool& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	int returnValue;
	SF_CODES::ERRORS error = this->executeScalar("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='" + table + "';", returnValue);
	
	if (SF_SQLITE_SUCCESS(error))
	{
		result = returnValue > 0;
	}

	return error;
}

SF_Sqlite::~SF_Sqlite()
{
	if (connection)
	{
		this->disconnect();
	}
}