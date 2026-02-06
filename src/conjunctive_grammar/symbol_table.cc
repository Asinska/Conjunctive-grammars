#include "src/conjunctive_grammar/symbol_table.h"

namespace conjunctive_grammar {

bool SymbolTable::IsNonterminal(const std::string &symbol) {
  return nonterminal_to_id_.find(symbol) != nonterminal_to_id_.end();
}

bool SymbolTable::IsTerminal(const std::string &symbol) {
  return terminal_to_id_.find(symbol) != terminal_to_id_.end();
}

void SymbolTable::AddNonterminal(const std::string &symbol_name) {
  nonterminal_to_id_[symbol_name] = nonterminals_.size();
  nonterminals_.push_back(symbol_name);
}

int SymbolTable::AddNonterminal() {
  std::string new_name;
  while (IsNonterminal(new_name = names_generator_.Generate()) ||
         IsTerminal(new_name))
    ;
  AddNonterminal(new_name);
  return nonterminal_to_id_[new_name];
}

void SymbolTable::AddTerminal(const std::string &symbol_name) {
  terminal_to_id_[symbol_name] = terminals_.size();
  terminals_.push_back(symbol_name);
}

int SymbolTable::GetNonterminalId(const std::string &symbol_name) {
  return nonterminal_to_id_[symbol_name];
}

int SymbolTable::GetTerminalId(const std::string &symbol_name) {
  return terminal_to_id_[symbol_name];
}
std::string SymbolTable::GetNonterminalName(int symbol_id) {
  return nonterminals_[symbol_id];
}
std::string SymbolTable::GetTerminalName(int symbol_id) {
  return terminals_[symbol_id];
}

int SymbolTable::GetNonterminalCount() { return nonterminals_.size(); }

int SymbolTable::GetTerminalCount() { return terminals_.size(); }

}  // namespace conjunctive_grammar