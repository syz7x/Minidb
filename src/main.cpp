#include <iostream>
#include <string>

int main(){
    std::string input;
    
    while (true){
    std::getline(std::cin , input);
    if (input == ".exit"){
        std::cout<<"Goodbye!\n";
        break;
    }else if (input == "hello"){
        std::cout<<"Hello from the Minidb! This is your REPL working\n";
    }else{
        std::cout << "Unrecognized command: '" << input << "'\n";
    }
    }
    return 0;
}