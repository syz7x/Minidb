#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <filesystem>
#include <unordered_map>
#include "Table.h"

class Database{
public:
    std::unordered_map<std::string,Table> tables;
    void createTable(const std::string& name,
        const std::vector<std::pair<std::string,std::string>>& columns);  
    Table* getTable(const std::string& name);
    void loadAllTables();
};
#endif