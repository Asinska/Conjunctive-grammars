#include <gmpxx.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "structures.h"
#include "parse_input.h"
#include "helpers.h"
using namespace std;

void calculate(int n) {
    for (Nonterminal* nt: nonterminals) {
        if (nt->ProducesTerminal) nt->V[1] = true;
    }
    for (int i = 2; i <= n; i++) {
        for (NonterminalPair *P: allpairs) {
            P->eval(i);
        }
        for (Production P: productions) {
            bool con = true;
            for (auto a: P.conjunction) {
                con = con && (a.first == a.second->convolution[i]);
            }
            if ((int)(P.N->V).size() <= i) (P.N->V).push_back(con);
            else P.N->V[i] = P.N->V[i] || con;
        }
    }
}

int main() {
    string line;
    getline(cin, line);
    stringstream ss(line);
    if (!(ss >> n)) {
        cout << "Incorrect input\n";
        return 0;
    } 
    readGrammar();
    calculate(n);
    for (int i = 0; i <= n; i++) {
        if (nonterminals[0]->V[i]) cout << i << ' ';
    }
    cout << '\n';
}