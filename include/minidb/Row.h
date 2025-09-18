#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class Row{
public:
    std::vector<std::string> values;

    void addValue(const std::string& value){
        values.push_back(value);
    }
};