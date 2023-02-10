#include "table.h"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <deque>
#include <iostream>
#include <map>

using namespace std;

using addr_t = pair<string, int>;
// using expr_t = tuple<addr_t, char, addr_t>;
struct expr_t {
    addr_t lAddr;
    char op;
    addr_t rAddr;
};

expr_t parseExpression(string str);
void evaluateStack(deque<pair<pair<int, int>, expr_t>>& evalStack, map<string, int>& header, map<int, int>& rows,
                   Table& values);

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

    // stores parsed expression and coordinates it came from
    deque<pair<pair<int, int>, expr_t>> evalStack;
    // i need coordinates (or so I thought when I wrote this, it might work with range for)
    // TODO think about rewriting with references to avoid coordinates
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            auto elem = at(i, j);
            // it's an integer, no evaluation needed
            if (elem.index() == 0) {
                continue;
            }
            // it's an expression (whitespace is not expected and probably breaks everything)
            expr_t expr = parseExpression(get<1>(elem));
            evalStack.push_front({{i, j}, expr});
            // right shift is becoming excessive
            evaluateStack(evalStack, mHeader, mRows, *this);
        }
    }
}

expr_t parseExpression(string str) {
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

void evaluateStack(deque<pair<pair<int, int>, expr_t>>& evalStack, map<string, int>& header, map<int, int>& rows,
                   Table& values) {
    auto addrToCoords = [&](addr_t addr) {
        int col;
        int row;
        if (header.find(addr.first) == header.end()) {
            return pair{-1, -1};
        } else {
            col = header.at(addr.first);
        }
        if (rows.find(addr.second) == rows.end()) {
            return pair{-1, -1};
        } else {
            row = rows.at(addr.second);
        }
        return pair{row, col};
    };
    auto coordsToAddrString = [&](pair<int, int> coords) {
        auto it_Col = find_if(header.begin(), header.end(), [&](auto kv) { return kv.second == coords.first; });
        auto it_Row = find_if(rows.begin(), rows.end(), [&](auto kv) { return kv.second == coords.second; });
        return it_Col->first + to_string(it_Row->first);
    };
    // depth first evaluation
    while (!evalStack.empty()) {
        auto evalCell = evalStack.front();
        auto coords = evalCell.first;
        // looking for loops
        // probably should be ran only after stack length exceeds some value
        if (find_if(next(evalStack.begin()), evalStack.end(),
                    [&](auto cell) { return cell.first == evalCell.first; }) != evalStack.end()) {
            // loop was found, throw an exception with address of repeated cell
            throw runtime_error("Loop during evaluation, contains cell " + coordsToAddrString(coords));
        }
        // whether operands are numeric
        auto lCoords = addrToCoords(evalCell.second.lAddr);
        if (lCoords.first == -1) {
            throw invalid_argument("Invalid adress in cell " + coordsToAddrString(coords));
        }
        cell_t left = values.at(lCoords.first, lCoords.second);
        if (left.index() == 1) {
            expr_t expr = parseExpression(get<1>(left));
            evalStack.push_front({lCoords, expr});
            continue;
        }
        auto rCoords = addrToCoords(evalCell.second.rAddr);
        cell_t right = values.at(rCoords.first, rCoords.second);
        if (rCoords.first == -1) {
            throw invalid_argument("Invalid adress in cell " + coordsToAddrString(coords));
        }
        if (right.index() == 1) {
            expr_t expr = parseExpression(get<1>(right));
            evalStack.push_front({rCoords, expr});
            continue;
        }
        // finally, operation
        cell_t& cell = values.at(coords.first, coords.second);
        switch (evalCell.second.op) {
        case '+': {
            cell = get<0>(left) + get<0>(right);
            break;
        }
        case '-': {
            cell = get<0>(left) - get<0>(right);
            break;
        }
        case '*': {
            cell = get<0>(left) * get<0>(right);
            break;
        }
        case '/': {
            if (get<0>(right) == 0) {
                throw invalid_argument("division by zero in cell " + coordsToAddrString(coords));
            }
            cell = get<0>(left) / get<0>(right);
            break;
        }
        default: {
            throw invalid_argument("invalid operation in cell " + coordsToAddrString(coords));
        }
        }
        evalStack.pop_front();
    }
}
