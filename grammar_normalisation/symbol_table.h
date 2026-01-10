#ifndef GRAMMAR_NORMALISATION_SYMBOL_TABLE_H_
#define GRAMMAR_NORMALISATION_SYMBOL_TABLE_H_

#include <vector>
#include <string>
#include <map>

#include "symbol_names_generator.h"

class SymbolTable {
 public:
  bool IsNonterminal(const std::string &symbol_name);
  bool IsTerminal(const std::string &symbol_name);
  void AddNonterminal(const std::string &symbol_name);
  int AddNonterminal();
  void AddTerminal(const std::string &symbol_name);
  int GetNonterminalId(const std::string &symbol_name);
  int GetTerminalId(const std::string &symbol_name);
  std::string GetNonterminalName(int symbol_id);
  std::string GetTerminalName(int symbol_id);

 private:
  std::vector<std::string> nonterminals_, terminals_;
  std::map<std::string, int> nonterminal_to_id_, terminal_to_id_;
  SymbolNamesGenerator names_generator_;
};



#endif  // GRAMMAR_NORMALISATION_SYMBOL_TABLE_H_ 