#ifndef __SF__SQLITE__HELPER__
#define __SF__SQLITE__HELPER__

#include "sqlite3.h"
#include "SF_Sqlite_Structures.h"
#include <string>
#include <list>
#include <vector>
#include <assert.h>

namespace SF_SQLITE_QUERY_STRINGS
{
	std::string SELECT(" SELECT ");
	std::string FROM(" FROM ");
	std::string WHERE(" WHERE ");
}

void sf_sqlite_buildEquals(std::string& output, const std::string& left, const std::string& right)
{
	output += left;
	output += " = ";
	output += right;
}

void sf_sqlite_buildFinalSelectNoWhereQuery(std::string& output, const std::string& table, const std::string& selectColumns)
{
	output += SF_SQLITE_QUERY_STRINGS::SELECT;
	output += selectColumns;
	output += SF_SQLITE_QUERY_STRINGS::FROM;
	output += table;
}

void sf_sqlite_buildFinalSelectNoWhereNoColumnsQuery(std::string& output, 
												     const std::string& table)
{
	output += SF_SQLITE_QUERY_STRINGS::SELECT;
	output += " * ";
	output += SF_SQLITE_QUERY_STRINGS::FROM;
	output += table;
}

void sf_sqlite_buildFinalSelectAllQuery(std::string& output, 
									 const std::string& table, 
									 const std::string& selectColumns,
									 const std::string& whereValues)
{
	output += SF_SQLITE_QUERY_STRINGS::SELECT;
	output += selectColumns;
	output += SF_SQLITE_QUERY_STRINGS::FROM;
	output += table;
	output += whereValues;
}

void sf_sqlite_buildWhereColumnList(std::string& output, const std::vector<SF_Sqlite_Column_Data_Pair>& whereValues)
{
	if (whereValues.size() > 0)
	{
		output += SF_SQLITE_QUERY_STRINGS::WHERE;

		std::vector<SF_Sqlite_Column_Data_Pair>::const_iterator iterator;
		for (iterator = whereValues.begin(); iterator != whereValues.end(); ++iterator)
		{
			sf_sqlite_buildEquals(output, (*iterator).columnName, (*iterator).columnData);
			output += ",";
		}

		output.resize(output.size() - 1);
	}
}

void sf_sqlite_buildSelectColumnList(std::string& output)
{
	output += " * ";
}

void sf_sqlite_buildSelectColumnList(std::string& output, const std::vector<std::string>& columns)
{

	if (columns.size() > 0)
	{
		std::vector<std::string>::const_iterator columnIterator;
		for (columnIterator = columns.begin(); columnIterator != columns.end(); ++columnIterator)
		{
			output += (*columnIterator);
			output += ",";
		}

		output.resize(output.size() - 1);
	}
	else
	{
		output = " * ";
	}
}

void sf_sqlite_buildFinalDropTable(std::string& output, const std::string& table)
{
	output += std::string("DROP TABLE ");
	output += table;
}

void sf_sqlite_buildCreateTableColumns(std::string& output, std::vector<SF_Sqlite_Column_Type_Pair> pairs)
{
	output += "(";

	std::vector<SF_Sqlite_Column_Type_Pair>::const_iterator iterator;
	for (iterator = pairs.begin(); iterator != pairs.end(); ++iterator)
	{
		output += (*iterator).columnName;
		output += std::string(" ");
		output += (*iterator).columnType;

		if (((*iterator).columnFlags & SF_COLUMN_FLAGS::PRIMARY_KEY) > 0)
		{
			output += std::string(" ");
			output += std::string("PRIMARY KEY");
		}

		if (((*iterator).columnFlags & SF_COLUMN_FLAGS::AUTOINCREMENT) > 0)
		{
			output += std::string(" ");
			output += std::string("AUTOINCREMENT");
		}

		output += std::string(",");
	}

	output.resize(output.size() - 1);

	output += ")";
}

void sf_sqlite_buildCreateTable(std::string& output, const std::string& table)
{
	output += std::string("CREATE TABLE ");
	output += table;
}

void sf_sqlite_buildInsertIntoTableHeader(std::string& output, const std::string& table)
{
	output += std::string("INSERT INTO ");
	output += table;
}

void sf_sqlite_buildInsertValues(std::string& ouput, const std::vector<std::string>& values)
{
	ouput += " values(";

	std::vector<std::string>::const_iterator iterator;
	for (iterator = values.begin(); iterator != values.end(); ++iterator)
	{
		ouput += (*iterator);
		ouput += ",";
	}

	ouput.resize(ouput.size() - 1);

	ouput += ")";
}

void sf_sqlite_buildInsertValues(std::string& ouput, const std::vector<SF_Sqlite_Column_Data_Pair>& values)
{
	std::string valueString = "(";
	std::string dataString = " values(";

	std::vector<SF_Sqlite_Column_Data_Pair>::const_iterator iterator;
	for (iterator = values.begin(); iterator != values.end(); ++iterator)
	{
		valueString += (*iterator).columnName;
		valueString += ",";

		dataString += (*iterator).columnData;
		dataString += ",";
	}

	valueString.resize(valueString.size() - 1);
	dataString.resize(dataString.size() - 1);

	valueString += ")";
	dataString += ")";

	ouput += valueString;
	ouput += dataString;
}

void sf_sqlite_buildParamedQuery(std::string& output, const std::string& paramedString, const std::vector<SF_Sqlite_Parameter>& params)
{
	output = paramedString;

	std::vector<SF_Sqlite_Parameter>::const_iterator iterator;
	for (iterator = params.begin(); iterator != params.end(); ++iterator)
	{
		int paramterID = (*iterator).paramNum;
		std::string parameterIDStringed;
		parameterIDStringed += "{";
		parameterIDStringed += std::to_string(paramterID);
		parameterIDStringed += "}";

		int paramPosition = output.find(parameterIDStringed);

		if (paramPosition != -1)
		{
			output.replace(paramPosition, parameterIDStringed.size(), (*iterator).data);
		}
	}
}

#define SF_SQLITE_WRITE_ERROR(ERROR_TEXT) \
		printf("SQLITE ERROR: %s \n", ERROR_TEXT); \

#define SF_SQLITE_IS_CONNECTED_ESCAPE(X) \
		if(X != 0) { SF_SQLITE_WRITE_ERROR("Connection Is Open"); return SF_CODES::ERROR::ALREADY_CONNECTED; } 

#define SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(X) \
		if(X == 0) { SF_SQLITE_WRITE_ERROR("Connection Is Closed"); return SF_CODES::ERROR::NOT_CONNECTED; } 

#define SF_SQLITE_RETURN_QUERY_ERROR(ERROR_C_STRING) \
		if (ERROR_C_STRING) { return SF_CODES::ERROR::QUERY_FAIL; } \
		else { return SF_CODES::ERROR::NO_ERROR; }
	
#if DEBUG_SQLITE

#define SF_SQLITE_PRINT_QUERY_ERROR(ERROR_C_STRING) \
		if (ERROR_C_STRING) printf("SQLITE ERROR: %s \n", ERROR_C_STRING); \

#else
#define SF_SQLITE_PRINT_QUERY_ERROR(ERROR_C_STRING)
#endif



#endif