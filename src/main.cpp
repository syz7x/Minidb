#include <iostream>
#include <string>
#include "Database.h"
#include "CommandParser.h"

int main(){
    Database db;
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