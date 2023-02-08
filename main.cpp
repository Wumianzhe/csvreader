#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <variant>
#include <vector>

typedef std::variant<int, std::string> cell_t;

struct Table {
  std::map<std::string, int> cols;
  std::map<int, int> rows;
  std::vector<cell_t> values;
  Table(std::vector<std::string> colNames) {
    for (int i = 1; i < colNames.size(); i++) {
      // there is probably is a fancier and "purer" way to do this, but I don't
      // know it
      cols.insert({colNames[i], i});
    }
  }
};

Table read(std::string filename);
void print(Table table);
std::vector<std::string> split(std::string line, char sep);

using namespace std;
int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "missing argument" << endl;
    return 1;
  }
  string filename = argv[1];
  try {
    Table t = read(filename);
  } catch (const std::exception &ex) {
    cout << ex.what() << endl;
  }

  return 0;
}

Table read(std::string filename) {
  ifstream in(filename);

  std::string line;
  if (!std::getline(in, line)) {
    throw std::invalid_argument("input file empty");
  }
  auto words = split(line, ',');
  if (words[0] != "") {
    throw std::invalid_argument("malformed csv");
  }
  Table ret(words);

  while (std::getline(in, line)) {
  }
  return ret;
}

// split line into words by separator
vector<string> split(string line, char sep) {
  std::stringstream iss(line);
  vector<string> ret;
  for (string word; getline(iss, word, sep);) {
    ret.push_back(word);
  }
  return ret;
}
