#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Database.h"

class CommandParser{
public:
    static void execute(Database& db,const std::string& input);

private:
    static void parseCreateTable(Database& db,const std::istringstream iss);
    static void parseInsertInto(Database& db,const std::istringstream iss);
    static void parseSelection(Database& db,const std::istringstream iss);
    static std::vector<std::string> splitString(const std::string& s, char delimiter);
    static std::string trim(const std::string& s);
}; 
#endif