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
  bool Read(std::istream &input, int &start_symbol, std::vector<Production> &productions,
            SymbolTable &symbol_table);
  void Print(std::ostream &output, int &start_symbol, std::vector<Production> &productions,
             SymbolTable &symbol_table);

 private:
  std::vector<std::string> GetTokenizedLine(std::istream &input);
  int Error(std::string error_message);
  int ReadProduction(std::istream &input, std::vector<Production> &productions,
                     SymbolTable &symbol_table);
  int ReadNonterminals(std::istream &input, int &start_symbol, SymbolTable &symbol_table);
  int ReadTerminals(std::istream &input, SymbolTable &symbol_table);
  void PrintProductions(std::ostream &output, std::vector<Production> &productions,
                        SymbolTable &symbol_table);
  void PrintTerminals(std::ostream &output, SymbolTable &symbol_table);
  void PrintNonterminals(std::ostream &output, int &start_symbol, SymbolTable &symbol_table);
};
}  // namespace conjunctive_grammar

#endif  // CONJUNCTIVE_GRAMMMAR_GRAMMAR_IO_H_