#ifndef TABLE_H_
#define TABLE_H_

#include <map>
#include <string>
#include <variant>
#include <vector>

typedef std::variant<int, std::string> cell_t;

struct Table {
    std::vector<std::string> header;
    std::vector<int> rows;
    std::vector<cell_t> values;
    int width = 0; // for convenience and to avoid calling size every time
    int height = 0;
};

Table read(std::string filename);
std::ostream& operator<<(std::ostream& stream, const Table& table);

#endif // TABLE_H_
