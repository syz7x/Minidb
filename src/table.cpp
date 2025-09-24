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
    
void Table::saveToFile(){
    saveSchema();
    saveData();
}

bool Table::loadFromFile(){
     return loadSchema() && loadData();
}
//保存表结构
void Table::saveSchema(){
    std::ofstream schemaFile(name + ".schema");
    if (!schemaFile){
        std::cerr<<"Error,can't create schame file for table"<<name<<std::endl;
        return;
    }

    for (const auto& column : columns){
        schemaFile<<column.name<<" "<<column.type<<"\n";
    }
    schemaFile.close();

}
//保存数据
void Table::saveData(){
    std::ofstream dataFile(name + ".data");
    if (!dataFile){
        std::cerr<<"Error,can't create data file for the table"<<name<<std::endl;
        return;
    }
    for (const auto& row : rows) {
            for (size_t i = 0; i < row.values.size(); ++i) {
                dataFile << row.values[i];
                if (i < row.values.size() - 1) {
                    dataFile << ",";
                }
            }
            dataFile << "\n";
        }
        dataFile.close();
}

 bool Table::loadSchema() {
        std::ifstream schemaFile(name + ".schema");
        if (!schemaFile) {
            // 文件不存在是正常的（新表）
            return false;
        }

        columns.clear();
        std::string line;
        while (std::getline(schemaFile, line)) {
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos) {
                std::string colName = line.substr(0, spacePos);
                std::string colType = line.substr(spacePos + 1);
                addColumn(colName, colType);
            }
        }
        schemaFile.close();
        return true;
    }

    // 加载数据
    bool Table::loadData() {
        std::ifstream dataFile(name + ".data");
        if (!dataFile) {
            // 文件不存在是正常的（新表）
            return false;
        }

        rows.clear();
        std::string line;
        while (std::getline(dataFile, line)) {
            Row newRow;
            std::istringstream lineStream(line);
            std::string value;
            
            while (std::getline(lineStream, value, ',')) {
                newRow.addValue(value);
            }
            
            if (newRow.values.size() == columns.size()) {
                rows.push_back(newRow);
            }
        }
        dataFile.close();
        return true;
    }