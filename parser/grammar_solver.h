#ifndef PARSER_GRAMMAR_SOLVER_H_
#define PARSER_GRAMMAR_SOLVER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "grammar_types.h"

namespace conjunctive_grammar {

class GrammarSolver {
 public:
  GrammarSolver();

  void ParseInput();
  void Solve();
  void PrintResult();

 private:
  int n_;
  Nonterminal* start_symbol_;
  std::string terminal_name_;

  std::vector<std::unique_ptr<Nonterminal>> nonterminals_;
  std::vector<std::unique_ptr<NonterminalPair>> nonterminal_pairs_;

  std::vector<Production> productions_;

  std::map<std::string, Nonterminal*> name_to_nonterminal_;
  std::map<std::pair<Nonterminal*, Nonterminal*>, NonterminalPair*>
      nonterminal_pair_to_pointer_;

  bool IsNonterminal(std::string s);
  void ReadN();
  void ReadNonterminals();
  void ReadTerminal();
  bool ReadProduction();
  std::vector<std::string> GetTokenizedLine();
  void Error(std::string error_message);
  void ReadGrammar();
  void EvaluatePair(NonterminalPair* nonterminal_pair, int i);
};

}  // namespace conjunctive_grammar

#endif  // PARSER_GRAMMAR_SOLVER_H_