#include <vector>
#include <iostream>
#include <algorithm>
#include "structures.h"
using namespace std;

int it = 0, len = 1, power = 26;
string get_new_symbol() {
    string s;
    while (1) {
        s = "";
        int copy = it;
        for (int i = 0; i < len; i++) {
            s.push_back('A' + copy%26);
            copy/=26;
        }
        s.push_back('_');
        reverse(s.begin(), s.end());
        
        it++;
        if (it >= power) {
            power *= 26;
            len++;
            it = 0;
        }
        if (nt_symbols_to_numbers.find(s) == nt_symbols_to_numbers.end()) return s;
    }
}


int introduce_new_nonterminal(string s = "") {
    if (s.empty()) {
        s = get_new_symbol();
    }
    else s = "_" + s;
    nt_symbols_to_numbers[s] = nonterminals.size();
    nonterminals.push_back(s);
    nullable.push_back(false);
    return nt_symbols_to_numbers[s];
}


// pair<int, int> sortin_table();

// void bucket_sort_second(vector<vector<int>> &v) {
//     if (sorting)
//     for ()
// }