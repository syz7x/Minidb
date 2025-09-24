#include "../include/minidb/CommandParser.h"
#include <iostream>

void CommandParser::execute(Database& db,
    const std::string& input){
    std::istringstream iss(input);
    std::string command;
    iss>>command;
    
    std::transform(command.begin(),command.end(),command.begin(), ::tolower);

    if (command == "create"){
        parseCreateTable(db,iss);
    }
    else if (command == "insert"){
        parseInsertInto(db,iss);
    }
    else if (command == "select"){
        parseSelect(db,iss);
    }
    else if (command == ".exit"){
        std::cout<<"Goodbye!\n";
        exit(0);
    }
    else if (command == "hello"){
        std::cout<<"Hello from the MiniDb.This is your REPl working\n";
    }

    else {
         std::cout << "Unrecognized command: '" << input << "'\n";
        std::cout << "Supported commands: CREATE TABLE, INSERT INTO, SELECT, .exit\n";
    }
}


void CommandParser::parseCreateTable(Database& db,
    std::istringstream& iss){
        std::string tablekeyword;
        iss>>tablekeyword;
        std::transform(tablekeyword.begin(),tablekeyword.end(),tablekeyword.begin(),::tolower);

        if (tablekeyword == "table"){
            std::string tableName;
            iss>>tableName;
            //读取列定义
            std::string columnsStr;
            std::getline(iss,columnsStr);
            //读取括号内的内容
            size_t start = columnsStr.find('(');
            size_t end = columnsStr.find(')');

            if (start == std::string::npos ||end == std::string::npos) {
                std::cout << "Error: Invalid CREATE TABLE syntax. Use: CREATE TABLE name (col1 type, col2 type, ...)\n";
                return;
            }
            std::string columnsPart = columnsStr.substr(start + 1,end-start-1);
            std::vector<std::pair<std::string, std::string>> columns;
            std::vector<std::string> columnDefs =splitString(columnsPart,',');
            for (auto& def : columnDefs) {
                std::string colDef = trim(def);
                size_t spacePos = colDef.find(' ');

                if (spacePos != std::string::npos){
                    std::string colName = trim(colDef.substr(0,spacePos));
                    std::string colType = trim(colDef.substr(spacePos+1));
                    columns.emplace_back(colName,colType);
                }
            }
            db.createTable(tableName,columns);
        }else{
            std::cout<<"Error: unrecognized command aftre CREATE: "<<tablekeyword<<"\n";
        }
    }


void CommandParser::parseInsertInto(Database& db,std::istringstream& iss){
    std::string intoKeyword;
    iss>>intoKeyword;
    std::transform(intoKeyword.begin(),intoKeyword.end(),intoKeyword.begin(),::tolower);

    if (intoKeyword == "into"){
        std::string tableName;
        iss>>tableName;

        //跳过VALUES关键词
        std::string valuesKeyword;
        iss>>valuesKeyword;

        //读取值
        std::string valuesStr;
        std::getline(iss,valuesStr);

        //读取括号内的值
        size_t start = valuesStr.find('(');
        size_t end = valuesStr.find(')');

        if (start == std::string::npos || end == std::string::npos){
            std::cout << "Error: Invalid INSERT syntax. Use: INSERT INTO table VALUES (value1, value2, ...)\n";
            return;
        }

        std::string valuesPart =valuesStr.substr(start+1,end-start-1);
        std::vector<std::string> values = splitString(valuesPart,',');

        for (auto& value : values){
            value = trim(value);
            if ((value.front() == '\'' && value.back() == '\'')||
            (value.front() == '"'&&value.back()=='"')){
                value =value.substr(1,value.size() - 2);
            }
        }

        //获取表并插入行
        Table* table = db.getTable(tableName);
        if (!table) return;

        Row newRow;
        for (const auto& val : values){
            newRow.addValue(val);
        }

        table->insertRow(newRow);
        std::cout<<"Row inserted successfully.\n";
    }else{
         std::cout << "Error: Unrecognized command after INSERT: " << intoKeyword << "\n";
    }
}

void CommandParser::parseSelect(Database&db ,std::istringstream& iss){
    //简单处理：只支持 SELECT * FROM tableName
    std::string asterisk;
    iss>>asterisk;

    if (asterisk != "*"){
        std::cout<<"Error: Only SELECT * is currently supported.\n";
        return;
    }
    
    std::string fromKeyword;
    iss>>fromKeyword;
    std::transform(fromKeyword.begin(),fromKeyword.end(),fromKeyword.begin(),::tolower);

    if (fromKeyword != "from"){
        std::cout<<"Error: Expected FROM keyword after SELECT *.\n";
        return;
    }

    std::string tableName;
    iss>>tableName;

    Table* table = db.getTable(tableName);
    if (!table) return;

    table->display();
}

std::vector<std::string> CommandParser::splitString(const std::string& s,char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream,token,delimiter)) {
        if (!token.empty()){
            tokens.push_back(trim(token));
        }
    }
    return tokens;
}

std::string CommandParser::trim(const std::string& s){
    auto start = s.begin();
    while (start != s.end()&&std::isspace(*start)){
        start++;
    }

    auto end = s.end();
    do{
        end--;
    }while (std::distance(start,end) > 0 && std::isspace(*end));

    return std::string(start,end+1);
}