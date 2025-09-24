#ifndef ROW_H
#define ROW_H

#include <string>
#include <vector>
class Row{
public:
    std::vector<std::string> values;

    void addValue(const std::string& value);
};
#endif