#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Row.h"
#include "Column.h"

class Table{
public:
    std::string name;
    std::vector<Column> columns;
    std::vector<Row> rows;

    Table(const std::string& name):name(name){}

    void addColumn(const std::string& name,const std::string& type);
    void insertRow(const Row& row);
    void display() const;
    void saveToFile();
    bool loadFromFile();

private:
    void saveSchema();
    void saveData();
    bool loadSchema();
    bool loadData();
};
#endif