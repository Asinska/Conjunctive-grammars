#ifndef SOLVER_SOLVER_TYPES_H_
#define SOLVER_SOLVER_TYPES_H_

#include <vector>
namespace grammar_solver {

struct Nonterminal {
  bool produces_terminal = false;
  std::vector<char> v;
  Nonterminal() { v.resize(2); }
  Nonterminal(int n) { v.resize(n + 1); }
};

struct NonterminalPair {
  Nonterminal *first, *second;
  std::vector<char> convolution;

  NonterminalPair(Nonterminal* a, Nonterminal* b) : first(a), second(b) {
    convolution.resize(2);
  }
};

struct SolverProduction {
  Nonterminal* producer;
  std::vector<std::pair<bool, NonterminalPair*>> conjunction;
  SolverProduction(Nonterminal* nonterminal,
                   std::vector<std::pair<bool, NonterminalPair*>> conjunction)
      : producer(nonterminal), conjunction(conjunction) {}
};

}  // namespace grammar_solver

#endif  // SOLVER_SOLVER_TYPES_H_