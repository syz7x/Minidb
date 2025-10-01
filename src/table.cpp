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

        size_t rowIndex = rows.size();//新行的索引
        rows.push_back(row);

        //为新行更新所有已存在的索引
        updateIndexesForNewRow(rowIndex,row);
        //保存到文件
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

    void Table::createIndex(const std::string& columnName){
        int columnIndex = -1;
        //查找列索引
        for (int i = 0;i<columns.size();++i){
            if (columns[i].name == columnName){
                columnIndex = i;
                break;
            }
        }

        if (columnIndex = -1){
            std::cerr<<"Error:Column'"<<columnName<<"'does not exist."<<std::endl;
            return;
        }

        //检查是否已存在该列的索引
        if (indexes.find(columnIndex) != indexes.end()){
            std::cout<<"Index on column'"<<columnName<<"'already exists"<<std::endl;
            return;
        }

        //创建新的索引
        indexes[columnIndex] = std::unordered_map<std::string,std::vector<size_t>>();

        //为所有现有索引构建索引
        for (size_t i = 0;i<rows.size();++i){
            if (columnIndex < rows[i].values.size()){
                    std::string key = rows[i].values[columnIndex];
                    indexes[columnIndex][key].push_back(i);
                }
            }
            std::cout<<"Index created on column'"<<columnName<<"'"<<std::endl;

            //保存索引消息
            saveIndexInfo();
    }

//使用索引查询
void Table::selectWithIndex(const std::string& columnName,const std::string& value) {
    int columnIndex = -1;

    //查找列索引
    for (int i = 0;i < columns.size();++i){
        if (columns[i].name == columnName){
            columnIndex = i;
            break;
        }
    }

    if (columnIndex = -1){
         std::cerr << "Error: Column '" << columnName << "' does not exist." << std::endl;
        return;
    }

    //检查是否有该列的索引 
    auto indexIt = indexes.find(columnIndex);
    if (indexIt == indexes.end()){
        std::cout<<"No index on column'"<<columnName<<"'.Performing full table scan..."<<std::endl;
        //如果没有索引，执行全表扫描
        selectWithFullScan(columnIndex,value);
        return;
    }

    //使用索引查询
    auto valueIt = indexIt->second.find(value);
    if (valueIt == indexIt->second.end()){
        std::cout<<"No rows found with"<<columnName<<" = "<<value<<std::endl;
        return;
    }
    //显示匹配的行
    std::cout<<"Using index on column'"<<columnName<<"'"<<std::endl;
    std::cout<<"Found"<<valueIt->second.size()<<"row(s):"<<std::endl;
    //显示列名
    for (const auto& col : columns){
        std::cout<<col.name<<"\t";
    }
    std::cout<<std::endl;

    //显示匹配的行
    for (size_t rowIndex : valueIt->second) {
        if (rowIndex < rows.size()){
            const Row& row = rows[rowIndex];
            for (const auto& val : row.values){
                std::cout<<val<<"\t";
            }
            std::cout<<std::endl;
        }
    }
}
void Table::saveToFile(){
    saveSchema();
    saveData();
}

bool Table::loadFromFile(){
     return loadSchema() && loadData();
}
//为新插入的行更新所有索引
void Table::updateIndexesForNewRow(size_t rowIndex,const Row& row){
    for (auto& index : indexes){
        int colIndex = index.first;
        if (colIndex < row.values.size()){
            std::string key = row.values[colIndex];
            index.second[key].push_back(rowIndex);
        }
    }
}

//全表扫描查询（没有索引时使用）
void Table::selectWithFullScan(int columnIndex,const std::string& value){
    std::vector<size_t> matchingRows;

    for (size_t i = 0; i<rows.size();++i){
        if (columnIndex < rows[i].values.size()&&rows[i].values[columnIndex] == value){
            matchingRows.push_back(i);
        }
    }
    if (matchingRows.empty()){
        std::cout<<"No rows found with"<<columns[columnIndex].name<<" = "<<value<<std::endl;
        return;
    }

    std::cout<<"Found"<<matchingRows.size()<<"row(s):"<<std::endl;
    
    //显示列名
    for (const auto& col : columns){
        std::cout<<col.name<<"\t";
    }
    std::cout<<std::endl;

    //显示匹配的行
    for (size_t rowIndex : matchingRows) {
        const Row& row  = rows[rowIndex];
        for (const auto& val : row.values){
            std::cout<<val<<"\t";
        }
        std::cout<<std::endl;
    }
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

//保存索引消息
void Table::saveIndexInfo() {
    std::ofstream indexFile(name + ".index");
    if (!indexFile) return;

    for (const auto& index : indexes){
        int colIndex = index.first;
        if(colIndex <columns.size()){
            indexFile <<columns[colIndex].name<<"\n";
        }
    }
    indexFile.close();
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
//加载索引消息并重建索引
    void Table::loadIndexInfo(){
        std::ifstream indexFile(name + ".index");
        if (!indexFile) return;

        std::string columnName;
        while (std::getline(indexFile,columnName)){
            createIndex(columnName);
        }
        indexFile.close();
    }
