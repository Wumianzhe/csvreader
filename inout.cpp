#include "inout.h"
#include <algorithm>
#include <charconv>
#include <fstream>
#include <sstream>

cell_t parseCell(std::string word);
std::vector<std::string> split(std::string line, char sep);

using namespace std;

Table read(std::string filename) {
    ifstream in(filename);

    std::string line;
    if (!std::getline(in, line)) {
        throw std::invalid_argument("input file empty");
    }
    auto header = split(line, ',');
    if (header[0] != "") {
        throw std::invalid_argument("malformed table: first cell not empty");
    }
    Table ret(header);

    // parse table body
    while (std::getline(in, line)) {
        auto row = split(line, ',');
        if (row.size() != ret.width + 1) {
            throw std::invalid_argument("malformed table: width mismatch");
        }

        cell_t rowNum = parseCell(row[0]);
        if (rowNum.index() != 0) {
            throw std::invalid_argument("malformed table: row number is not a number");
        }
        ret.rows.insert({get<int>(rowNum), ret.height});
        ret.height += 1;

        // append to value vector
        transform(next(row.begin()), row.end(), back_inserter(ret.values), parseCell);
    }
    return ret;
}

std::ostream& operator<<(std::ostream& stream, const Table& table) {
    // flatten header and row numbers
    vector<string> flatHeader;
    transform(table.cols.begin(), table.cols.end(), back_inserter(flatHeader), [](auto elem) { return elem.first; });
    vector<int> flatNums;
    transform(table.rows.begin(), table.rows.end(), back_inserter(flatNums), [](auto elem) { return elem.first; });
    // sorting is required afterwards (maps make find during evaluation easier, but require sorting later)
    sort(flatHeader.begin(), flatHeader.end(), [&](string l, string r) { return table.cols.at(l) < table.cols.at(r); });
    sort(flatNums.begin(), flatNums.end(), [&](int l, int r) { return table.rows.at(l) < table.rows.at(r); });

    // print header
    for (const string& col : flatHeader) {
        stream << "," << col;
    }
    stream << "\n";
    // print body
    for (int i = 0; i < table.height; i++) {
        for (int j = -1; j < table.width; j++) {
            if (j < 0) {
                stream << flatNums[i];
                continue;
            }
            cell_t val = table.values[i * table.width + j];
            if (val.index() == 0) {
                stream << "," << get<0>(val);
            } else {
                stream << "," << get<1>(val);
            }
        }
        stream << "\n";
    }

    return stream;
}

// split line into words by separator
vector<string> split(string line, char sep) {
    stringstream iss(line);
    vector<string> ret;
    for (string word; getline(iss, word, sep);) {
        ret.push_back(word);
    }
    return ret;
}

// put int into variant if cell contains value and string otherwise
cell_t parseCell(string word) {
    int val;
    if (from_chars(word.data(), word.data() + word.size(), val).ec == std::errc())
        return val;
    else
        return word;
}
