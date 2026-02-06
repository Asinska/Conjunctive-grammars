#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "src/conjunctive_grammar/grammar.h"
#include "src/solver/grammar_solver.h"

int main(int argc, char const** argv) {
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
  if (argc >= 3) {
    n = std::atoi(argv[2]);
  } else {
    std::cin >> n;
    std::string s;
    getline(std::cin, s);  // reading trailing whitespace
  }

  conjunctive_grammar::ConjunctiveGrammar G =
      conjunctive_grammar::ConjunctiveGrammar();
  if (!G.Read(*input_ptr)) {
    return 1;
  }
  G.Normalise();
  // G.Print(std::cout);
  auto solver = grammar_solver::GrammarSolver::Create(G, n);
  if (solver == nullptr) {
    std::cerr << "Failed to create the solver.\n";
    return 1;
  }
  solver->Solve();
  std::vector<char> result = solver->GetResult();
  // for (int i = 0; i <= n; i++) {
  //   if (result[i]) std::cout << i << ' ';
  // }
  if (result[n] == 1)
    std::cout << "String of length " << n << " is in the language.\n";
  else
    std::cout << "String of length " << n << " IS NOT in the language.\n";
  return 0;
}