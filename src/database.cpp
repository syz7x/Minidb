#include <filesystem>
#include "../include/minidb/Database.h"

void Database::createTable(const std::string& name,
        const std::vector<std::pair<std::string,std::string>>& columns){
        Table newTable(name);
        for (const auto& col : columns){
            newTable.addColumn(col.first,col.second);
        }
        tables[name] = newTable;
        std::cout<<"Table '"<<name<<"'created successfully."<<std::endl;
    }


Table* Database::getTable(const std::string& name){
        auto it = tables.find(name);
        if (it == tables.end()){
            std::cerr<<"Error: Table'"<<name<<"'does not exist."<<std::endl;
            return nullptr;
        }
        return &(it->second);
    }        


void Database::loadAllTables(){
    for (const auto& entry : std::filesystem::directory_iterator(".")){
        if (entry.path().extension() == ".schema"){
            std::string tableName = entry.path().stem().string();

            Table newTable(tableName);
            if (newTable.loadFromFile()){
                tables[tableName] = newTable;
                std::cout << "Loaded existing table: " << tableName << std::endl;
            }
        }
    }
}