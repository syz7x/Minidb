#include "../include/minidb/Table.h"

//添加列定义
void Table::addColumn(const std::string& name,const std::string& type){
        columns.emplace_back(name,type);
    }

//插入一行数据
void Table::insertRow(const Row& row){
        if(row.values.size() != columns.size()){
            std::cerr<<"Error: Row has"<<row.values.size()
                     <<" Values,but table has "<<columns.size()<<" columns."<<std::endl;
            return;
        }
        rows.push_back(row);

        saveToFile();
    }    
    
void Table::display() const{
        for(const auto& col : columns){
            std::cout<<col.name<<"\t";
        }
        std::cout<<std::endl;

        for (const auto& row : rows){
            for (const auto& value : row.values){
                std::cout<<value<<"\t";
            }
            std::cout<<std::endl;
        }
    }
    
void saveToFile(){
    saveSchema();
    saveData();
}

bool loadFromFile(){
     return loadSchema() && loadData();
}

void saveSchema(){
    std::ofstream schemaFile(name + ".schema");
    if (!schemaFile){
        std::cout<<"Error,can't create schame file for table"<<name<<std::endl;
        return;
    }

    for (const auto& column : columns){
        schemaFile<<column.name<<" "<<column.type<<"\n";
    }
    schemaFile.close();

}

void saveData(){

}

bool loadSchema(){

}

bool loadData(){

}