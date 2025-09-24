#ifndef COLUMN_H
#define COLUMN_H

#include <string>
#include <iostream>
//表示表中的一列
class Column{
public:
    std::string name;
    std::string type;
    Column(const std::string& name,const std::string& type)
    :name(name),type(type){}
};
#endif