#include <vector>
#include <iostream>
#include "structures.h"
using namespace std;


int introduce_new_nonterminal(string s = "") {
    s = "_" + s;
    nt_symbols_to_numbers[s] = nonterminals.size();
    nonterminals.push_back(s);
    nullable.push_back(0);
    return nt_symbols_to_numbers[s];
}