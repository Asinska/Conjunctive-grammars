#ifndef GRAMMAR_NORMALISATION_SYMBOL_TABLE_H_
#define GRAMMAR_NORMALISATION_SYMBOL_TABLE_H_

#include <vector>
#include <string>
#include <map>

#include "symbol_names_generator.h"

class SymbolTable {
 public:
  bool IsNonterminal(std::string symbol_name);
  bool IsTerminal(std::string symbol_name);
  void AddNonterminal(std::string symbol_name);
  int AddNonterminal();
  void AddTerminal(std::string symbol_name);
  int GetNonterminalId(std::string symbol_name);
  int GetTerminalId(std::string symbol_name);
  std::string GetNonterminalName(int symbol_id);
  std::string GetTerminalName(int symbol_id);

 private:
  std::vector<std::string> nonterminals_, terminals_;
  std::map<std::string, int> nonterminal_to_id_, terminal_to_id_;
  SymbolNamesGenerator names_generator_;
};



#endif  // GRAMMAR_NORMALISATION_SYMBOL_TABLE_H_ 