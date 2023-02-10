#include "table.h"
#include <iostream>

using namespace std;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "missing argument" << endl;
        return 1;
    }
    string filename = argv[1];
    try {
        Table t = read(filename);
        cout << t;
    } catch (const std::exception& ex) {
        cout << ex.what() << endl;
    }

    return 0;
}
