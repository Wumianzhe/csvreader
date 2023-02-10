#include "table.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <iostream>
#include <map>
#include <stack>

using namespace std;

using addr_t = pair<string, int>;
using expr_t = tuple<addr_t, char, addr_t>;

expr_t parseExpr(string str);
void Table::eval() {
    // using maps here for faster search
    map<string, int> mHeader;
    int index = 0;
    transform(header.begin(), header.end(), inserter(mHeader, mHeader.end()), [&](string key) {
        return pair{key, index++};
    });
    index = 0;
    map<int, int> mRows;
    transform(rows.begin(), rows.end(), inserter(mRows, mRows.end()), [&](int key) { return pair{key, index++}; });

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            auto elem = el(i, j);
            // it's an integer, no evaluation needed
            if (elem.index() == 0) {
                continue;
            }
            // it's an expression
            expr_t expr = parseExpr(get<1>(elem));
        }
    }
}

expr_t parseExpr(string str) {
    addr_t addrL, addrR;
    // find fist digit and copy all that comes before (other than =)
    auto it_FNum = find_if(next(str.begin()), str.end(), [](char c) { return isdigit(c); });
    copy(next(str.begin()), it_FNum, back_inserter(addrL.first));
    // parse out integer
    auto [p_Op, ec] = from_chars(str.data() + (it_FNum - str.begin()), str.data() + str.size(), addrL.second);
    char op = *p_Op;
    // it's a pointer, so need to find where to start first, otherwise same as before
    int dist = p_Op - str.data() + 1;
    auto it_SNum = find_if(str.begin() + dist, str.end(), [](char c) { return isdigit(c); });
    copy(str.begin() + dist, it_SNum, back_inserter(addrR.first));
    // parse out integer
    auto [p_end, ec2] = from_chars(str.data() + (it_SNum - str.begin()), str.data() + str.size(), addrR.second);
    return {addrL, op, addrR};
}
