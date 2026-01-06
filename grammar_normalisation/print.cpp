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
        for (vector<int> conjunct: p.conjunction) {
            if (first) first = false;
            else cout << "& ";
            for (int x: conjunct) {
                cout << nonterminals[x] << " ";
            }
        }
        cout << "\n";
    }
    for (Production p: terminal_productions) {
        cout << nonterminals[p.N] << " -> " << terminals[p.conjunction[0][0]]<< "\n";
    }
    // // // cout << "_" << EMPTY_STRING_SYM << " -> " << EMPTY_STRING_SYM << "\n";
    if (nullable[0]) cout << nonterminals[0] << " -> " << EMPTY_STRING_SYM << "\n";
}