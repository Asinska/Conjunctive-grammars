// grammar_solver.h
#ifndef CONJUNCTIVE_GRAMMAR_GRAMMAR_SOLVER_H_
#define CONJUNCTIVE_GRAMMAR_GRAMMAR_SOLVER_H_

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <string>
#include "grammar_types.h"

// Namespace to avoid collision
namespace conjunctive_grammar {

// Forward declarations
// struct Nonterminal;
// struct NonterminalPair;

class GrammarSolver {
 public:
  GrammarSolver();
  // ~GrammarSolver() = default;

  // Use PascalCase for methods
  void ParseInput(); 
  // void Solve(int n);
  void Solve();
  void PrintResult();// const;

 private:
  // Data members end with a trailing underscore
  int n_; 
  Nonterminal* start_symbol_;
  string terminal_name_;
  
  // Use smart pointers (std::unique_ptr) to own objects. No "naked" new.
  std::vector<std::unique_ptr<Nonterminal>> nonterminals_;
  std::vector<std::unique_ptr<NonterminalPair>> nonterminal_pairs_;
  
  // Productions can be stored directly if they are copyable/movable
  std::vector<Production> productions_;
  
  // Helper to lookup names
  std::map<std::string, Nonterminal*> name_to_nonterminal_;
  std::map<pair<Nonterminal*, Nonterminal*>, NonterminalPair*> nonterminal_pair_to_pointer_;

  bool IsNonterminal(string s);
  void ReadN();
  void ReadNonterminals();
  void ReadTerminal();
  bool ReadProduction();
  std::vector<std::string> GetTokenizedLine();
  void Error(string error_message);
  void ReadGrammar();
  void EvaluatePair(NonterminalPair* nonterminal_pair, int i);

};

} // namespace conjunctive_grammar

#endif // CONJUNCTIVE_GRAMMAR_GRAMMAR_SOLVER_H_