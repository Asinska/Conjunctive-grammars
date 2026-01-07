// #include <iostream>
// #include <sstream>
// #include <vector>
// #include <unordered_map>

#include "grammar_solver.h"
// #include "parse_input.h"
// #include ".h"

// using namespace std;

// void calculate(int n) {
//     for (Nonterminal* nt: nonterminals) {
//         if (nt->ProducesTerminal) nt->V[1] = true;
//     }
//     for (int i = 2; i <= n; i++) {
//         for (NonterminalPair *P: allpairs) {
//             P->eval(i);
//         }
//         for (Production P: productions) {
//             bool con = true;
//             for (auto a: P.conjunction) {
//                 con = con && (a.first == a.second->convolution[i]);
//             }
//             if ((int)(P.N->V).size() <= i) (P.N->V).push_back(con);
//             else P.N->V[i] = P.N->V[i] || con;
//         }
//     }
// }
// namespace conjunctive_grammar{

    
    int main() {
    // ReadN();
    // readGrammar();
    // calculate(n);
    // for (int i = 0; i <= n; i++) {
        //     if (nonterminals[0]->V[i]) cout << i << ' ';
        // }
        // cout << '\n';
        conjunctive_grammar::GrammarSolver grammar_solver = conjunctive_grammar::GrammarSolver();
        grammar_solver.ParseInput();
        grammar_solver.Solve();
        grammar_solver.PrintResult();
    }
// }   // namespace conjunctive