#include <iostream>
#include <vector>
#include <string>

#include "solver/grammar_solver.h"
#include "conjunctive_grammar/grammar.h"



int main() {
  int n; std::cin >> n;
  std::string s;
  getline(std::cin, s);
  conjunctive_grammar::ConjunctiveGrammar G = conjunctive_grammar::ConjunctiveGrammar();
  G.Read();
  G.Normalise();
  G.Print();
  auto solver = grammar_solver::GrammarSolver::Create(G, n);
  if (solver == nullptr) {
    std::cout << "Failed to solve.\n";
    return 1;
  }
  solver->Solve();
  std::vector<char> result = solver->GetResult();
  for (int i = 0; i <= n; i++) {
    if (result[i]) std::cout << i << ' ';
  }
  return 0;
}