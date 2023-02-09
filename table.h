#ifndef TABLE_H_
#define TABLE_H_

#include <map>
#include <string>
#include <variant>
#include <vector>

typedef std::variant<int, std::string> cell_t;

struct Table {
    std::map<std::string, int> cols;
    std::map<int, int> rows;
    std::vector<cell_t> values;
    int width = 0; // for convenience and to avoid calling size on map every time
    int height = 0;
    Table(std::vector<std::string> colNames) {
        width = colNames.size() - 1;
        for (int i = 1; i < colNames.size(); i++) {
            // there probably is another way to do this, but I don't know it
            cols.insert({colNames[i], i});
        }
    }
};

#endif // TABLE_H_
