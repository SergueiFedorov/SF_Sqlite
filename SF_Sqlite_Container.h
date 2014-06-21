#ifndef __PocketRitual__SF_Sqlite_Container__
#define __PocketRitual__SF_Sqlite_Container__

#include <iostream>
#include <list>
#include "SF_Sqlite_Structures.h"
#include <map>

class SF_Sqlite_Row : public std::map<std::string, std::string>
{
    //Extra features
    //For future development
};

class SF_Sqlite_Result : public std::list<SF_Sqlite_Row>
{
    //Extra features
    //For future development
};

#endif
