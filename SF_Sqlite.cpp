#include "SF_Sqlite.h"
#include "SF_Sqlite_Helper.h"

//This will not open connection as it is too risky that it may
//cause issues in the constructor. Never run code which can
//throw exeptions or cause errors in the constructors
SF_Sqlite::SF_Sqlite( const std::string & name)
	: connection(0), databaseName(name)
{

}

//Opens a connection to the database file specified through the constructor
//RETURNS: returns ALREADY_CONNECTED if a connection is already open. Otherwise returns NO_ERROR
const SF_CODES::ERROR SF_Sqlite::connect()
{
	SF_SQLITE_IS_CONNECTED_ESCAPE(connection);

	sqlite3_open(this->databaseName.c_str(), &this->connection);

	return SF_CODES::ERROR::NO_ERROR;
}

const SF_CODES::ERROR SF_Sqlite::disconnect() 
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	sqlite3_close(this->connection);

	return SF_CODES::ERROR::NO_ERROR;
}

static int callback(void *data, int argc, char **argv, char **ColumnNames)
{
	std::list<SF_Sqlite_Row>* rows = (std::list<SF_Sqlite_Row>*)data;

	SF_Sqlite_Row row;
	for (int x = 0; x < argc; ++x)
	{
		SF_Sqlite_Cell cell;
		cell.columnName = ColumnNames[x];
		cell.data = argv[x];

		row.columns.push_back(cell);
	}

	rows->push_back(row);

	return 0;
}

const SF_CODES::ERROR SF_Sqlite::execute(const std::string& query, std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	char* error;
	sqlite3_exec(this->connection, query.c_str(), callback, &result, &error);

	SF_SQLITE_PRINT_QUERY_ERROR(error);

	return SF_CODES::ERROR::NO_ERROR;
}

const SF_CODES::ERROR SF_Sqlite::execute(const std::string& query, 
										 const std::vector<SF_Sqlite_Parameter>& params,
										 std::list<SF_Sqlite_Row>& result) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string finalQuery;
	sf_sqlite_buildParamedQuery(finalQuery, query, params);

	return this->execute(finalQuery, result);
}

const SF_CODES::ERROR SF_Sqlite::execute(const std::string& query) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	char* error;
	sqlite3_exec(this->connection, query.c_str(), 0, 0, &error);

	SF_SQLITE_PRINT_QUERY_ERROR(error);

	return SF_CODES::ERROR::NO_ERROR;
}

const SF_CODES::ERROR SF_Sqlite::insertRecord(const std::string& table, const std::vector<std::string>& values) const
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

const SF_CODES::ERROR SF_Sqlite::insertRecord(const std::string& table, const std::vector<SF_Sqlite_Column_Data_Pair>& values) const
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

const SF_CODES::ERROR SF_Sqlite::getRecords(const std::string& table,
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

const SF_CODES::ERROR SF_Sqlite::getRecords(const std::string& table,
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

const SF_CODES::ERROR SF_Sqlite::getRecords(const std::string& table,
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

const SF_CODES::ERROR SF_Sqlite::dropTable(const std::string& table) const
{
	SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(connection);

	std::string query;
	sf_sqlite_buildFinalDropTable(query, table);

	return this->execute(query);
}

const SF_CODES::ERROR SF_Sqlite::createTable(const std::string& name, 
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

SF_Sqlite::~SF_Sqlite()
{
	if (connection)
	{
		this->disconnect();
	}
}