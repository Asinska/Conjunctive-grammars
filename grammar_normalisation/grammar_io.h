#ifndef GRAMMAR_NORMALISATION_GRAMMAR_IO_H_
#define GRAMMAR_NORMALISATION_GRAMMAR_IO_H_

#include <map>
#include <string>
#include <vector>

#include "grammar_types.h"
#include "symbol_names_generator.h"
#include "symbol_table.h"

class GrammarIO {
 public:
  GrammarIO();
  void Read(int &start_symbol,
            std::vector<Production> &productions, SymbolTable &symbol_table);
  void Print(int &start_symbol,
             std::vector<Production> &productions, SymbolTable &symbol_table);

 private:
  std::vector<std::string> GetTokenizedLine();
  void Error(std::string error_message);
  bool ReadProduction(std::vector<Production> &productions,
                      SymbolTable &symbol_table);
  void ReadNonterminals(int &start_symbol,
                        SymbolTable &symbol_table);
  void ReadTerminals(SymbolTable &symbol_table);
  void PrintProductions(std::vector<Production> &productions,
                        SymbolTable &symbol_table);
  void PrintTerminals(SymbolTable &symbol_table);
  void PrintNonterminals(int &start_symbol,
                         SymbolTable &symbol_table);
};

#endif  // GRAMMAR_NORMALISATION_GRAMMAR_IO_H_