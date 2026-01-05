#include <vector>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <map>
#include "structures.h"
#define NEG_SYM "~"
#define EMPTY_STRING_SYM "eps"

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


void readGrammar() {
    unordered_map<string, pair<int, Nonterminal*>> NT_sym;
    Nonterminal* start = NULL;

    vector<string> line = getTokenizedLine();
    if (line.size() == 0) {
        ERROR("Expected at least one nonterminal symbol.");
    }
    int cnt = 0;
    for (string s: line) {
        Nonterminal* NT = new Nonterminal();
        if (start == NULL) start = NT;
        nonterminals.push_back(NT);
        
        if (NT_sym.find(s) != NT_sym.end()) {
            ERROR("Nonterminal symbols should be pairwise distinct.");
        }
        NT_sym[s] = {cnt++, NT};
    }
    line = getTokenizedLine();
    if (line.size() != 1) {
        ERROR("Exactly one terminal symbol expected.");
    }
    string T_sym = line[0];
    if (NT_sym.find(T_sym) != NT_sym.end()) {
        ERROR("Terminal symbol appears in nonterminal symbols list.");
    }

    string production_error = "Expected production format:\nA -> B1 C1 & B2 C2 & ... & Bk Ck\nor\nA -> a";
    map<pair<int, int>, NonterminalPair*> NT_pairs;
    while ((line = getTokenizedLine()).size() > 0) {
        if (line.size() < 3) {
            ERROR(production_error);
        }
        if (line.size() == 3) {
            if (NT_sym.find(line[0]) == NT_sym.end() || line[1] != "->") {
                ERROR(production_error);
            }
            if (line[2] == EMPTY_STRING_SYM) {
                if (NT_sym[line[0]].first != 0) {
                    ERROR("Only start symbol should be able to produce empty word.");
                }
                NT_sym[line[0]].second->V[0] = true;
            }
            else if (line[2] == T_sym)
                (*NT_sym[line[0]].second).ProducesTerminal = true;
            else 
                ERROR(production_error);
        }   
        else {
            if (NT_sym.find(line[0]) == NT_sym.end() || line[1] != "->") {
                ERROR(production_error);
            }
            vector<pair<bool, NonterminalPair*>> v;
            for (int i = 2; i < (int)line.size(); i+=3) {
                bool b = true;
                if (line[i] == NEG_SYM) {
                    b = false;
                    i++;
                }
                if (i+1 >= (int)line.size() || (i+2 != (int)line.size() && line[i+2] != "&")) {
                    ERROR(production_error);
                }
                if (NT_sym.find(line[i]) == NT_sym.end() || NT_sym.find(line[i+1]) == NT_sym.end()) {
                    ERROR(production_error);
                }
                pair<int, int> p = {NT_sym[line[i]].first, NT_sym[line[i+1]].first};
                if (NT_pairs.find(p) == NT_pairs.end()) {
                    NonterminalPair* newpair = new NonterminalPair(NT_sym[line[i]].second,NT_sym[line[i+1]].second);
                    NT_pairs[p] = newpair;
                    allpairs.push_back(newpair);
                }
                v.push_back({b, NT_pairs[p]});
            }
            productions.push_back(Production(NT_sym[line[0]].second, v));
        }
    }
}