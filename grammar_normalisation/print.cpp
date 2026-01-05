// #include <vector>
// #include <string>
#include <iostream>

#include "structures.h"


void print_nonterminals() {
    for (string nt: nonterminals) {
        cout << nt << ' ';
    }
    cout << "\n";
}

void print_terminals() {
    for (string t: terminals) {
        cout << t << ' ';
    }
    cout << "\n";
}


void print_productions() {
    for (Production p: nonterminal_productions) {
        cout << nonterminals[p.N] << " -> ";
        bool first = true;
        for (pair<bool, vector<int>> conjunct: p.conjunction) {
            if (first) first = false;
            else cout << "& ";
            if (conjunct.first == false) {
                cout << NEG_SYM << " ";
            }
            for (int x: conjunct.second) {
                cout << nonterminals[x] << " ";
            }
        }
        cout << "\n";
    }
    for (Production p: terminal_productions) {
        cout << nonterminals[p.N] << " -> " << terminals[p.conjunction[0].second[0]] << "\n";
    }
    // cout << "_" << EMPTY_STRING_SYM << " -> " << EMPTY_STRING_SYM << "\n";
    if (nullable[0]) cout << nonterminals[0] << " -> " << EMPTY_STRING_SYM << "\n";
}