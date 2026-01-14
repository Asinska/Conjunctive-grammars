#ifndef CONJUNCTIVE_GRAMMAR_GRAMMAR_H_
#define CONJUNCTIVE_GRAMMAR_GRAMMAR_H_

#include <map>
#include <string>
#include <vector>

#include "conjunctive_grammar/grammar_io.h"
#include "conjunctive_grammar/grammar_types.h"
#include "conjunctive_grammar/symbol_names_generator.h"
#include "conjunctive_grammar/symbol_table.h"

class ConjunctiveGrammar {
 public:
  ConjunctiveGrammar();
  void Read();
  void Normalise();
  // bool IsNormal();
  void Print();

 private:
  int start_symbol_;
  bool is_normalised_;
  GrammarIO grammar_io_;
  SymbolTable symbol_table_;

  std::vector<Production> productions_;
  std::vector<bool> is_nullable_;
  std::vector<int> terminal_to_producer_;
  std::map<std::pair<int, int>, int> nonterminal_pair_to_producer_;

  int NonterminalsCnt();
  void EliminateMixedProductions();
  void FindNullableSet();
  void SubstituteStartSymbol();
  void EliminateEpsilonConjuncts();
  void EliminateLongConjuncts();
  void SortProductions();
  Production MergeSortedConjuncts(Production &a, Production &b);
  void GenerateProductionsBySubstitution(
      Production p, std::vector<Production> &new_productions,
      std::vector<int> &l, int mode);
  void EliminateUnitConjuncts();
};

#endif  // CONJUNCTIVE_GRAMMAR_GRAMMAR_H_