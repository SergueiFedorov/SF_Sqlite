#ifndef __SF__SQLITE__HELPER__
#define __SF__SQLITE__HELPER__

#include "sqlite3.h"
#include "SF_Sqlite_Structures.h"

#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include <assert.h>



void sf_sqlite_buildColumnType(std::string& output, const std::string& column, const std::string& type);
void sf_sqlite_buildColumnDataPairStrings(std::string& nameOutput, std::string& columnOutput, const std::vector<SF_Sqlite_Column_Data_Pair>& values);
void sf_sqlite_buildCommaList(std::string& output, const std::vector<std::string> values);
void sf_sqlite_buildEqualsCommaList (std::string& output, const std::vector<SF_Sqlite_Column_Data_Pair>& pairs);
void sf_sqlite_buildEquals(std::string& output, const std::string& left, const std::string& right);
void sf_sqlite_buildFinalSelectNoWhereQuery(std::string& output, const std::string& table, const std::string& selectColumns);
void sf_sqlite_buildFinalSelectNoWhereNoColumnsQuery(std::string& output, const std::string& table);
void sf_sqlite_buildFinalSelectAllQuery(std::string& output, const std::string& table, const std::string& selectColumns, const std::string& whereValues);
void sf_sqlite_buildWhereColumnList(std::string& output, const std::vector<SF_Sqlite_Column_Data_Pair>& whereValues);
void sf_sqlite_buildSelectColumnList(std::string& output);
void sf_sqlite_buildSelectColumnList(std::string& output, const std::vector<std::string>& columns);
void sf_sqlite_buildFinalDropTable(std::string& output, const std::string& table);
void sf_sqlite_buildCreateTableColumns(std::string& output, std::vector<SF_Sqlite_Column_Type_Pair> pairs);
void sf_sqlite_buildCreateTable(std::string& output, const std::string& table);
void sf_sqlite_buildInsertIntoTableHeader(std::string& output, const std::string& table);
void sf_sqlite_buildInsertValues(std::string& output, const std::vector<std::string>& values);
void sf_sqlite_buildInsertValues(std::string& ouput, const std::vector<SF_Sqlite_Column_Data_Pair>& values);
void sf_sqlite_buildParamedQuery(std::string& output, const std::string& paramedString, const std::vector<SF_Sqlite_Parameter>& params);

namespace SF_SQLITE_QUERY_STRINGS
{
	const std::string SELECT(" SELECT ");
	const std::string FROM(" FROM ");
	const std::string WHERE(" WHERE ");
	const std::string DROP_TABLE(" DROP TABLE ");
	const std::string CREATE_TABLE(" CREATE TABLE ");
	const std::string AUTOINCREMENT(" AUTOINCREMENT ");
	const std::string PRIMARY_KEY(" PRIMARY KEY ");
	const std::string INSERT_INTO(" INSERT INTO ");
	const std::string VALUES(" VALUES ");
}

void sf_sqlite_buildColumnType(std::string& output, const std::string& column, const std::string& type)
{
	output += column + " " + type;
}

void sf_sqlite_buildColumnDataPairStrings(std::string& nameOutput, std::string& columnOutput, const std::vector<SF_Sqlite_Column_Data_Pair>& values)
{
	std::vector<SF_Sqlite_Column_Data_Pair>::const_iterator iterator;
	for (iterator = values.begin(); iterator != values.end(); ++iterator)
	{
		nameOutput += (*iterator).columnName;
		nameOutput += ",";

		columnOutput += (*iterator).columnData;
		columnOutput += ",";
	}

	nameOutput.resize(nameOutput.size() - 1);
	columnOutput.resize(columnOutput.size() - 1);
}

void sf_sqlite_buildCommaList(std::string& output, const std::vector<std::string> values)
{
	std::vector<std::string>::const_iterator iterator;
	for (iterator = values.begin(); iterator != values.end(); ++iterator)
	{
		output += (*iterator);
		output += ",";
	}

	output.resize(output.size() - 1);
}

void sf_sqlite_buildEqualsCommaList (std::string& output, const std::vector<SF_Sqlite_Column_Data_Pair>& pairs)
{
		std::vector<SF_Sqlite_Column_Data_Pair>::const_iterator iterator;
		for (iterator = pairs.begin(); iterator != pairs.end(); ++iterator)
		{
			sf_sqlite_buildEquals(output, (*iterator).columnName, (*iterator).columnData);
			output += ",";
		}

		output.resize(output.size() - 1);
}

void sf_sqlite_buildEquals(std::string& output, const std::string& left, const std::string& right)
{
	output += left + " = " + right;
}

void sf_sqlite_buildFinalSelectNoWhereQuery(std::string& output, const std::string& table, const std::string& selectColumns)
{
	output += SF_SQLITE_QUERY_STRINGS::SELECT + selectColumns + SF_SQLITE_QUERY_STRINGS::FROM + table;
}

void sf_sqlite_buildFinalSelectNoWhereNoColumnsQuery(std::string& output, 
												     const std::string& table)
{
	output += SF_SQLITE_QUERY_STRINGS::SELECT + " * " +  SF_SQLITE_QUERY_STRINGS::FROM + table;
}

void sf_sqlite_buildFinalSelectAllQuery(std::string& output, 
									 const std::string& table, 
									 const std::string& selectColumns,
									 const std::string& whereValues)
{
	output += SF_SQLITE_QUERY_STRINGS::SELECT + selectColumns + table +  whereValues;
}

void sf_sqlite_buildWhereColumnList(std::string& output, const std::vector<SF_Sqlite_Column_Data_Pair>& whereValues)
{
	if (whereValues.size() > 0)
	{
		output += SF_SQLITE_QUERY_STRINGS::WHERE;
		sf_sqlite_buildEqualsCommaList(output, whereValues);
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
		sf_sqlite_buildCommaList(output, columns);
	}
	else
	{
		output = " * ";
	}
}

void sf_sqlite_buildFinalDropTable(std::string& output, const std::string& table)
{
	output += SF_SQLITE_QUERY_STRINGS::DROP_TABLE;
	output += table;
}

void sf_sqlite_buildCreateTableColumns(std::string& output, std::vector<SF_Sqlite_Column_Type_Pair> pairs)
{
	output += "(";

	std::vector<SF_Sqlite_Column_Type_Pair>::const_iterator iterator;
	for (iterator = pairs.begin(); iterator != pairs.end(); ++iterator)
	{
		sf_sqlite_buildColumnType(output, (*iterator).columnName, (*iterator).columnType);

		if (((*iterator).columnFlags & SF_COLUMN_FLAGS::PRIMARY_KEY) > 0)
		{
			output += SF_SQLITE_QUERY_STRINGS::PRIMARY_KEY;
		}

		if (((*iterator).columnFlags & SF_COLUMN_FLAGS::AUTOINCREMENT) > 0)
		{
			output += SF_SQLITE_QUERY_STRINGS::AUTOINCREMENT;
		}

		output += std::string(",");
	}

	output.resize(output.size() - 1);

	output += ")";
}

void sf_sqlite_buildCreateTable(std::string& output, const std::string& table)
{
	
	output += SF_SQLITE_QUERY_STRINGS::CREATE_TABLE;
	output += table;
}

void sf_sqlite_buildInsertIntoTableHeader(std::string& output, const std::string& table)
{
	output += SF_SQLITE_QUERY_STRINGS::INSERT_INTO;
	output += table;
}

void sf_sqlite_buildInsertValues(std::string& output, const std::vector<std::string>& values)
{
	output += SF_SQLITE_QUERY_STRINGS::VALUES + "(";
	sf_sqlite_buildCommaList(output, values);
	output += ")";
}

void sf_sqlite_buildInsertValues(std::string& ouput, const std::vector<SF_Sqlite_Column_Data_Pair>& values)
{
	std::string nameString = "(";
	std::string columnString = SF_SQLITE_QUERY_STRINGS::INSERT_INTO + "(";

	sf_sqlite_buildColumnDataPairStrings(nameString, columnString, values);

	nameString += ")";
	columnString += ")";

	ouput += nameString;
	ouput += columnString;
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

		size_t paramPosition = output.find(parameterIDStringed);

		if (paramPosition != -1)
		{
			output.replace(paramPosition, parameterIDStringed.size(), (*iterator).data);
		}
	}
}


#define SF_SQLITE_WRITE_ERROR(ERROR_TEXT) \
		printf("SQLITE ERROR: %s \n", ERROR_TEXT); \

#define SF_SQLITE_IS_CONNECTED_ESCAPE(X) \
		if(X != 0) { SF_SQLITE_WRITE_ERROR("Connection Is Open"); return SF_CODES::ERRORS::ALREADY_CONNECTED; }

#define SF_SQLITE_IS_NOT_CONNECTED_ESCAPE(X) \
		if(X == 0) { SF_SQLITE_WRITE_ERROR("Connection Is Closed"); return SF_CODES::ERRORS::NOT_CONNECTED; }

#define SF_SQLITE_RETURN_QUERY_ERROR(ERROR_C_STRING) \
		if (ERROR_C_STRING) { return SF_CODES::ERRORS::QUERY_FAIL; } \
		else { return SF_CODES::ERROR::NO_ERROR; }

#define SF_SQLITE_PROCESS_SQLITE_NATIVE_ERROR(ERROR) \
        (SF_CODES::ERRORS) ERROR;


#if DEBUG_SQLITE

#define SF_SQLITE_PRINT_QUERY(QUERY) \
		printf("SQLITE QUERY: %s \n", QUERY.c_str());\

#define SF_SQLITE_PRINT_QUERY_ERROR(ERROR_C_STRING) \
	if (ERROR_C_STRING) printf("SQLITE ERROR: %s \n", ERROR_C_STRING); return SF_CODES::ERRORS::QUERY_FAIL; \

#define SF_SQLITE_SUCCESS(ERROR_CODE) \
		ERROR_CODE == SF_CODES::ERRORS::NO_ERROR

#define SF_SQLITE_UNUSED(X)

#else
#define SF_SQLITE_PRINT_QUERY_ERROR(ERROR_C_STRING)
#define SF_SQLITE_PRINT_QUERY(QUERY) 
#endif



#endif