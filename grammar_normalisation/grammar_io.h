#ifndef PARSER_GRAMMAR_IO_H_
#define PARSER_GRAMMAR_IO_H_

#include <vector>
#include <string>
#include <map>
#include "symbol_names_generator.h"
#include "grammar_types.h"
#include "symbol_table.h"



class GrammarIO {
 public:
  GrammarIO();
  void Read(int &nonterminals_cnt, int &terminals_cnt, int &start_symbol, std::vector<Production> &productions, SymbolTable &symbol_table);
  void Print(int &nonterminals_cnt, int &terminals_cnt, int &start_symbol, std::vector<Production> &productions, SymbolTable &symbol_table);
 private:
  // int start_symbol_;
  // std::vector<std::string> nonterminals_, terminals_;
  // std::map<std::string, int> nonterminal_to_id_, terminal_to_id_;
  // // std::vector<Production> nonterminal_productions_, terminal_productions_;
  // // std::vector<bool> is_nullable_;
  // // std::vector<int> terminal_to_producer_;
  // // std::map<std::pair<int, int>, int> nonterminal_pair_to_producer;
  // SymbolNamesGenerator names_generator_;

  std::vector<std::string> GetTokenizedLine();
  void Error(std::string error_message);
  bool ReadProduction(std::vector<Production> &productions, SymbolTable &symbol_table);
  void ReadNonterminals(int &nonterminals_cnt, int &start_symbol, SymbolTable &symbol_table);
  void ReadTerminals(int &terminals_cnt, SymbolTable &symbol_table);
  void PrintProductions(std::vector<Production> &productions, SymbolTable &symbol_table);
  void PrintTerminals(int &terminals_cnt, SymbolTable &symbol_table);
  void PrintNonterminals(int &nonterminals_cnt, int &start_symbol, SymbolTable &symbol_table);
  // void IntroduceNewNonterminal();




};   


#endif  // PARSER_GRAMMAR_IO_H_