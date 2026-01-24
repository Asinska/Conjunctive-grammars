#ifndef CONJUNCTIVE_GRAMMMAR_GRAMMAR_IO_H_
#define CONJUNCTIVE_GRAMMMAR_GRAMMAR_IO_H_

#include <map>
#include <string>
#include <vector>

#include "conjunctive_grammar/grammar_types.h"
#include "conjunctive_grammar/symbol_names_generator.h"
#include "conjunctive_grammar/symbol_table.h"

namespace conjunctive_grammar {

class GrammarIO {
 public:
  GrammarIO();
  bool Read(int &start_symbol, std::vector<Production> &productions,
            SymbolTable &symbol_table);
  void Print(int &start_symbol, std::vector<Production> &productions,
             SymbolTable &symbol_table);

 private:
  std::vector<std::string> GetTokenizedLine();
  int Error(std::string error_message);
  int ReadProduction(std::vector<Production> &productions,
                     SymbolTable &symbol_table);
  int ReadNonterminals(int &start_symbol, SymbolTable &symbol_table);
  int ReadTerminals(SymbolTable &symbol_table);
  void PrintProductions(std::vector<Production> &productions,
                        SymbolTable &symbol_table);
  void PrintTerminals(SymbolTable &symbol_table);
  void PrintNonterminals(int &start_symbol, SymbolTable &symbol_table);
};
}  // namespace conjunctive_grammar

#endif  // CONJUNCTIVE_GRAMMMAR_GRAMMAR_IO_H_