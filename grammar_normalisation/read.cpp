#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include "structures.h"
#include "helpers.h"
using namespace std;


void ERROR(string message) {
    cerr << "Incorrect input error: " << message << '\n';
    exit(EXIT_FAILURE);
}

vector<string> getTokenizedLine() {
    string line;
    getline(cin, line);
    stringstream ss(line);
    vector<string> tokens;
    string tmp;
    while (ss >> tmp) {
        tokens.push_back(tmp);
    }
    return tokens;
}

inline bool is_nonterminal(string s) {
    return nt_symbols_to_numbers.find(s) != nt_symbols_to_numbers.end();
}
inline bool is_terminal(string s) {
    return t_symbols_to_numbers.find(s) != t_symbols_to_numbers.end();

}

void read_nonterminals() {
    vector<string> line = getTokenizedLine();
    if (line.size() == 0) {
        ERROR("Expected at least one nonterminal symbol.");
    }
    for (string nt: line) {
        if (is_nonterminal(nt)) {
            ERROR("Nonterminal symbols should be pairwise distinct.");
        }
        nt_symbols_to_numbers[nt] = nonterminals.size();
        nonterminals.push_back(nt);
        // nonterminals_cnt++;
    }
    nullable.resize(nonterminals.size(), false);
}
void read_terminals() {
    vector<string> line = getTokenizedLine();
    if (line.size() == 0) {
        ERROR("Expected at least one terminal symbol.");
    }
    for (string t: line) {
        if (is_nonterminal(t)) 
            ERROR("Terminal symbol appears in nonterminal symbols list.");
        if (is_terminal(t)) 
            ERROR("Terminal symbols should be pairwise distinct.");
        t_symbols_to_numbers[t] = terminals.size();
        terminals.push_back(t);
        // terminals_cnt++;
    }
    nt_generator.resize(terminals.size(), -1);
}


bool read_production() {
    // cnt++;
    string production_error = "Invalid format of production: ";

    vector<string> line = getTokenizedLine();
    if (line.size() == 0) return false;
    if (line.size() < 3) {
        ERROR(production_error+"Production should consist of nonterminal symbol, arrow and positive numbber of conjuncts.");
    }
    if (!is_nonterminal(line[0]) || line[1] != "->") {
        ERROR(production_error);
    }
    if (line.size() == 3) {
        if (line[2] == EMPTY_STRING_SYM) { //A -> ε
            nullable[nt_symbols_to_numbers[line[0]]] = true;
            return true;
        }
        if (is_terminal(line[2])) { //A -> a
            terminal_productions.push_back(Production(nt_symbols_to_numbers[line[0]], {{t_symbols_to_numbers[line[2]]}}));
            return true;
        }
    }


    vector<vector<int>> conjuncts;
    int pos = 2;
    while (pos < (int)line.size()) {
        vector<int> conjunct;
        string sym;
        while (pos < (int)line.size() && ((sym = line[pos]) != "&")) {
            if (is_nonterminal(line[pos])) {
                conjunct.push_back(nt_symbols_to_numbers[line[pos]]);
            } 
            else if (is_terminal(line[pos])) {
                int t_number = t_symbols_to_numbers[line[pos]];
                if (nt_generator[t_number] == -1) {
                    nt_generator[t_number] = introduce_new_nonterminal(line[pos]);
                    terminal_productions.push_back(Production(nt_generator[t_number], {{t_number}}));
                }
                conjunct.push_back(nt_generator[t_number]);
            }
            else if (line[pos] == EMPTY_STRING_SYM) {
                conjunct.push_back(nt_symbols_to_numbers[EMPTY_STRING_NONTERMINAL]);
            }
            else {
                ERROR(production_error);
            }
            pos++;
        }
        conjuncts.push_back(conjunct);
        pos++;
    }
    nonterminal_productions.push_back(Production(nt_symbols_to_numbers[line[0]], conjuncts));
    return true;
}

void read_all_productions() {
    while (read_production()) ;
}