#ifndef SOLVER_GRAMMAR_SOLVER_H_
#define SOLVER_GRAMMAR_SOLVER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "conjunctive_grammar/grammar.h"
#include "solver/solver_types.h"

namespace grammar_solver {

class GrammarSolver {
 public:
  static std::unique_ptr<GrammarSolver> Create(
      conjunctive_grammar::ConjunctiveGrammar& grammar, int n);
  void Solve();
  std::vector<char> GetResult();

 private:
  int n_;
  Nonterminal* start_symbol_;

  GrammarSolver(conjunctive_grammar::ConjunctiveGrammar& grammar, int n);
  std::vector<std::unique_ptr<Nonterminal>> nonterminals_;
  std::vector<std::unique_ptr<NonterminalPair>> nonterminal_pairs_;
  std::vector<SolverProduction> productions_;

  std::map<std::pair<Nonterminal*, Nonterminal*>, NonterminalPair*>
      nonterminal_pair_to_pointer_;

  void EvaluatePair(NonterminalPair* nonterminal_pair, int i);
};

}  // namespace grammar_solver

#endif  // SOLVER_GRAMMAR_SOLVER_H_