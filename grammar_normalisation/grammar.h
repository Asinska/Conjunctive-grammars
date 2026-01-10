#ifndef GRAMMAR_NORMALISATION_GRAMMAR_H_
#define GRAMMAR_NORMALISATION_GRAMMAR_H_

#include <vector>
#include <string>
#include <map>

#include "symbol_names_generator.h"
#include "grammar_io.h"
#include "grammar_types.h"
#include "symbol_table.h"



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
  int nonterminals_cnt_, terminals_cnt_;
  GrammarIO grammar_io_;
  SymbolTable symbol_table_;
  // std::vector<std::string> nonterminals_, terminals_;
  // std::map<std::string, int> nonterminal_to_id_, terminal__to_id_;
  // std::vector<Production> nonterminal_productions_, terminal_productions_;
  std::vector<Production> productions_;
  std::vector<bool> is_nullable_;
  std::vector<int> terminal_to_producer_;
  std::map<std::pair<int, int>, int> nonterminal_pair_to_producer;
  // SymbolNamesGenerator names_generator_;

  // void ReadProduction();
  // void ReadNonterminals();
  // void ReadTerminals();
  // std::vector<std::string> GetTokenizedLine();
  // void Error(std::string error_message);
  void Init();
  void EliminateMixedProductions();
  void FindNullableSet();
  void EliminateEpsilonConjuncts();
  void EliminateLongConjuncts();
  Production MergeSortedConjuncts(Production a, Production b);
  void GenerateProductionsBySubstitution(Production p, std::vector<Production> &new_productions, std::vector<int> &l, int mode);
  void EliminateUnitConjuncts();
  // void PrintProductions();
  // void PrintTerminals();
  // void PrintNonterminals();
  // void IntroduceNewNonterminal(std::string name_hint);



};   

#endif  // GRAMMAR_NORMALISATION_GRAMMAR_H_