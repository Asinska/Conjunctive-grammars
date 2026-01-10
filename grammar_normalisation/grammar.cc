#include "grammar.h"

#include <queue>
#include <vector>
#include <iostream> // for debug

#include "grammar_io.h"
#include "grammar_types.h"


ConjunctiveGrammar::ConjunctiveGrammar() {
  grammar_io_ = GrammarIO();
  nonterminals_cnt_ = 0;
  terminals_cnt_ = 0;
}

void ConjunctiveGrammar::Read() {
  grammar_io_.Read(nonterminals_cnt_, terminals_cnt_, start_symbol_, productions_, symbol_table_);
  Init();
}

// void ConjunctiveGrammar::Parse() {}

void ConjunctiveGrammar::Normalise() {
  EliminateMixedProductions();
  FindNullableSet();
  // EliminateEpsilonConjuncts();
  // EliminateLongConjuncts();
  // EliminateUnitConjuncts();
}

// bool ConjunctiveGrammar::IsNormal() {
//   return is_normalised_;
// }

void ConjunctiveGrammar::Print() {
  grammar_io_.Print(nonterminals_cnt_, terminals_cnt_, start_symbol_, productions_, symbol_table_);
}

// void ConjunctiveGrammar::ReadProduction() {}
// 
// void ConjunctiveGrammar::ReadNonterminals() {}

// void ConjunctiveGrammar::ReadTerminals() {}

// std::vector<std::string> ConjunctiveGrammar::GetTokenizedLine() {
  // return std::vector<std::string>();
// }

// void ConjunctiveGrammar::Error(std::string error_message) {}

void ConjunctiveGrammar::Init() {
  terminal_to_producer_.resize(symbol_table_.GetTerminalCount(), -1);
}

void ConjunctiveGrammar::EliminateMixedProductions() {
  for (Production &production: productions_) {
    if (production.conjunction.size() == 1 && production.conjunction[0].size() == 1) {
      if (production.conjunction[0][0].type == SymbolType::kTerminal) {
        production.type = ProductionType::kTerminal;
        continue;
      }
      if (production.conjunction[0][0].type == SymbolType::kEpsilon) {
        production.type = ProductionType::kEpsilon;
        continue;
      }
    }
    for (std::vector<Symbol> &conjunct: production.conjunction) {
      for (Symbol &symbol: conjunct) {
        if (symbol.type == SymbolType::kTerminal) {
          if (terminal_to_producer_[symbol.value] == -1) {
            terminal_to_producer_[symbol.value] = symbol_table_.AddNonterminal();
          }
          Symbol new_symbol = Symbol(SymbolType::kNonterminal, terminal_to_producer_[symbol.value]);
          symbol = new_symbol;
        }
      }
    }
    production.type = ProductionType::kNonterminal;
  }
}

void ConjunctiveGrammar::FindNullableSet() {
  is_nullable_.resize(symbol_table_.GetNonterminalCount());
  std::vector<std::vector<int>> appears_in_production(symbol_table_.GetNonterminalCount());
    std::vector<int>degree(productions_.size());
    for (int i = 0; i < (int)productions_.size(); i++) {
      if (productions_[i].type == ProductionType::kEpsilon) {
        is_nullable_[productions_[i].producer] = true;
      }
      else if (productions_[i].type == ProductionType::kNonterminal) {
        for (std::vector<Symbol> &conjunct: productions_[i].conjunction) {
            for (Symbol &symbol: conjunct) {
              if (symbol.type == SymbolType::kEpsilon) break;
                appears_in_production[symbol.value].push_back(i);
                degree[i]++;
            }
        }
      }
    }
    std::queue<int> nullable;
    for (int i = 0; i < symbol_table_.GetNonterminalCount(); i++) {
        if (is_nullable_[i]) {
            nullable.push(i);
        }
    }
    while (!nullable.empty()) {
        int nonterminal = nullable.front();
        nullable.pop();
        for (int i: appears_in_production[nonterminal]) {
            degree[i]--;
            if (degree[i] == 0) {
                if(is_nullable_[productions_[i].producer]) continue;
                is_nullable_[productions_[i].producer] = true;
                nullable.push(productions_[i].producer);
            }
        }
    }
  std::vector<Production> new_productions;
  for (Production &production: productions_) {
      if (production.type == ProductionType::kTerminal) {
        new_productions.push_back(production);
      }
      else if (production.type == ProductionType::kEpsilon) {
        continue;
      }
      else {
        bool contains_epsilon_conjunct = false;
        for (std::vector<Symbol> &conjunct: production.conjunction) {
            for (Symbol &symbol: conjunct) {
              if (symbol.type == SymbolType::kEpsilon) {
                contains_epsilon_conjunct = true;
                break;
              }
            }
        }
        if (!contains_epsilon_conjunct) {
          new_productions.push_back(production);
        }
      }
    }
    productions_ = new_productions;
    for (int i = 0; i < symbol_table_.GetNonterminalCount(); i++) {
      if (is_nullable_[i]) std::cout << i << ' ';
    }
}

void ConjunctiveGrammar::EliminateEpsilonConjuncts() {}

void ConjunctiveGrammar::EliminateLongConjuncts() {}

// Production ConjunctiveGrammar::MergeSortedConjuncts(Production a,
//                                                     Production b) {
//   return Production();
// }

void ConjunctiveGrammar::GenerateProductionsBySubstitution(
    Production p, std::vector<Production>& new_productions, std::vector<int>& l,
    int mode) {}

void ConjunctiveGrammar::EliminateUnitConjuncts() {}

// void ConjunctiveGrammar::PrintProductions() {}

// void ConjunctiveGrammar::PrintTerminals() {}

// void ConjunctiveGrammar::PrintNonterminals() {}

// void ConjunctiveGrammar::IntroduceNewNonterminal(std::string name_hint) {}
