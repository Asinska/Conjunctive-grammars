#include "solver/grammar_solver.h"

int main() {
  conjunctive_grammar::GrammarSolver grammar_solver =
      conjunctive_grammar::GrammarSolver();
  grammar_solver.ParseInput();
  grammar_solver.Solve();
  grammar_solver.PrintResult();
  return 0;
}