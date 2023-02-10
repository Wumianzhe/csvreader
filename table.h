#ifndef TABLE_H_
#define TABLE_H_

#include <string>
#include <variant>
#include <vector>

using cell_t = std::variant<int, std::string>;

struct Table {
    std::vector<std::string> header;
    std::vector<int> rows;
    std::vector<cell_t> values;
    int width = 0; // for convenience and to avoid calling size every time
    int height = 0;
    // in place modification for now
    void eval();
    inline cell_t& el(int i, int j) { return values[i * width + j]; }
    // prefer to have these mentioned explicitly
    Table() = default;
    Table(Table&) = default;
    Table(Table&&) = default;
    Table& operator=(const Table&) = default;
    Table& operator=(Table&&) = default;
};

Table read(std::string filename);
std::ostream& operator<<(std::ostream& stream, const Table& table);

#endif // TABLE_H_
