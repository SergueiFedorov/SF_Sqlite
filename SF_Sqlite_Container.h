//
//  SF_Sqlite_Container.h
//  PocketRitual
//
//  Created by Serguei on 6/21/14.
//  Copyright (c) 2014 Serguei. All rights reserved.
//

#ifndef __PocketRitual__SF_Sqlite_Container__
#define __PocketRitual__SF_Sqlite_Container__

#include <iostream>
#include <list>
#include "SF_Sqlite_Structures.h"
#include <map>

class SF_Sqlite_Row : public std::map<std::string, std::string>
{
    //Extra features
};

class SF_Sqlite_Result : public std::list<SF_Sqlite_Row>
{
    //Extra features
};

#endif /* defined(__PocketRitual__SF_Sqlite_Container__) */
