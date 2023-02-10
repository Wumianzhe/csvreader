#include "table.h"
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
    // if file is not empty, there's at least one string in header
    if (header[0] != "") {
        throw std::invalid_argument("malformed table: first cell not empty");
    }
    Table ret;
    copy(next(header.begin()), header.end(), back_inserter(ret.header));
    ret.width = header.size() - 1;

    // parse table body
    while (std::getline(in, line)) {
        auto row = split(line, ',');
        if (row.size() != ret.width + 1) {
            throw std::invalid_argument("malformed table: width mismatch");
        }
        auto cellConv = [](string word) {
            // put int into variant if cell contains value and string otherwise
            int val;
            if (from_chars(word.data(), word.data() + word.size(), val).ec == std::errc())
                return cell_t{val};
            else
                return cell_t{word};
        };

        cell_t rowNum = cellConv(row[0]);
        if (rowNum.index() != 0) {
            throw std::invalid_argument("malformed table: row number is not a number");
        }
        ret.rows.push_back(get<int>(rowNum));
        ret.height += 1;

        // append to value vector
        transform(next(row.begin()), row.end(), back_inserter(ret.values), cellConv);
    }
    return ret;
}

std::ostream& operator<<(std::ostream& stream, const Table& table) {
    // print header
    for (const string& col : table.header) {
        stream << "," << col;
    }
    stream << "\n";
    // print body
    for (int i = 0; i < table.height; i++) {
        for (int j = -1; j < table.width; j++) {
            if (j < 0) {
                stream << table.rows[i];
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
