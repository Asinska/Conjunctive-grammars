#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "conjunctive_grammar/grammar.h"
#include "solver/grammar_solver.h"

int main(int argc, char const **argv) {
  std::ifstream filestream; 
  std::istream* input_ptr = &std::cin;

  if (argc >= 2) {
    filestream.open(argv[1]);
    if (filestream.is_open()) {
      input_ptr = &filestream;
    } else {
      std::cerr << "Failed to open " << argv[1] << '\n';
      return 1;
    }
  }
  int n;
  std::cin >> n;
  std::string s;
  getline(std::cin, s);
  conjunctive_grammar::ConjunctiveGrammar G =
      conjunctive_grammar::ConjunctiveGrammar();
  if (!G.Read(*input_ptr)) {
    return 0;
  }
  G.Normalise();
  G.Print(std::cout);
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