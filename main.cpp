#include <iostream>
#include <string>
#include "include\minidb\Database.h"
#include "include\minidb\CommandParser.h"

int main(){
    Database db;

    std::cout << "Loading existing tables..." << std::endl;
    db.loadAllTables();
    

    std::string input;
    
    std::cout << "MiniDB - A simple in-memory database\n";
    std::cout << "Enter SQL commands or .exit to quit:\n";

    while (true){
        std::cout<<"minidb>";
        std::getline(std::cin,input);

        if (input.empty()){
            continue;
        }
        CommandParser::execute(db,input);
}
    return 0;
}