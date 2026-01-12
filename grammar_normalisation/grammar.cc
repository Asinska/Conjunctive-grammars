#include "grammar.h"

#include <algorithm>
#include <iostream>  // for debug
#include <queue>
#include <vector>

#include "grammar_io.h"
#include "grammar_types.h"

ConjunctiveGrammar::ConjunctiveGrammar() {
  grammar_io_ = GrammarIO();
  nonterminals_cnt_ = 0;
  terminals_cnt_ = 0;
}

void ConjunctiveGrammar::Read() {
  grammar_io_.Read(nonterminals_cnt_, terminals_cnt_, start_symbol_,
                   productions_, symbol_table_);
  Init();
}

void ConjunctiveGrammar::Normalise() {
  EliminateMixedProductions();
  FindNullableSet();
  SubstituteStartSymbol();
  EliminateEpsilonConjuncts();
  EliminateLongConjuncts();
  EliminateUnitConjuncts();
}

// bool ConjunctiveGrammar::IsNormal() {
//   return is_normalised_;
// }

void ConjunctiveGrammar::Print() {
  grammar_io_.Print(nonterminals_cnt_, terminals_cnt_, start_symbol_,
                    productions_, symbol_table_);
}

void ConjunctiveGrammar::Init() {
  terminal_to_producer_.resize(symbol_table_.GetTerminalCount(), -1);
}

void ConjunctiveGrammar::EliminateMixedProductions() {
  for (int i = 0; i < (int)productions_.size(); i++) {
    Production &production = productions_[i];
    if (production.conjunction.size() == 1 &&
        production.conjunction[0].size() == 1) {
      if (production.conjunction[0][0].type == SymbolType::kTerminal) {
        production.type = ProductionType::kTerminal;
        continue;
      }
      if (production.conjunction[0][0].type == SymbolType::kEpsilon) {
        production.type = ProductionType::kEpsilon;
        continue;
      }
    }
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      for (Symbol &symbol : conjunct) {
        if (symbol.type == SymbolType::kTerminal) {
          if (terminal_to_producer_[symbol.value] == -1) {
            terminal_to_producer_[symbol.value] =
                symbol_table_.AddNonterminal();
            productions_.push_back(
                Production(ProductionType::kTerminal,
                           terminal_to_producer_[symbol.value], {{symbol}}));
          }
          Symbol new_symbol = Symbol(SymbolType::kNonterminal,
                                     terminal_to_producer_[symbol.value]);
          symbol = new_symbol;
        }
      }
    }
    production.type = ProductionType::kNonterminal;
  }
}

void ConjunctiveGrammar::FindNullableSet() {
  is_nullable_.resize(symbol_table_.GetNonterminalCount());
  std::vector<std::vector<int>> appears_in_production(
      symbol_table_.GetNonterminalCount());
  std::vector<int> degree(productions_.size());
  for (int i = 0; i < (int)productions_.size(); i++) {
    if (productions_[i].type == ProductionType::kEpsilon) {
      is_nullable_[productions_[i].producer] = true;
    } else if (productions_[i].type == ProductionType::kNonterminal) {
      for (std::vector<Symbol> &conjunct : productions_[i].conjunction) {
        for (Symbol &symbol : conjunct) {
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
    for (int i : appears_in_production[nonterminal]) {
      degree[i]--;
      if (degree[i] == 0) {
        if (is_nullable_[productions_[i].producer]) continue;
        is_nullable_[productions_[i].producer] = true;
        nullable.push(productions_[i].producer);
      }
    }
  }
  std::vector<Production> new_productions;
  for (Production &production : productions_) {
    if (production.type == ProductionType::kTerminal) {
      new_productions.push_back(production);
    } else if (production.type == ProductionType::kEpsilon) {
      continue;
    } else {
      bool contains_epsilon_conjunct = false;
      for (std::vector<Symbol> &conjunct : production.conjunction) {
        for (Symbol &symbol : conjunct) {
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
  // for (int i = 0; i < symbol_table_.GetNonterminalCount(); i++) {
  //   if (is_nullable_[i]) std::cout << i << ' ';
  // }
}

void ConjunctiveGrammar::SubstituteStartSymbol() {
  if (is_nullable_[start_symbol_] == false) return;
  bool appears_on_right_side = false;
  for (Production &production : productions_) {
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      for (Symbol &symbol : conjunct) {
        if (symbol.type == SymbolType::kNonterminal &&
            symbol.value == start_symbol_) {
          appears_on_right_side = true;
          break;
        }
      }
    }
  }
  if (appears_on_right_side == false) return;
  int new_start_symbol = symbol_table_.AddNonterminal();
  productions_.push_back(
      Production(ProductionType::kNonterminal, new_start_symbol,
                 {{Symbol(SymbolType::kNonterminal, start_symbol_)}}));
  is_nullable_[new_start_symbol] = true;
  start_symbol_ = new_start_symbol;
}

void ConjunctiveGrammar::EliminateEpsilonConjuncts() {
  std::vector<Production> new_productions;
  for (Production &production : productions_) {
    if (production.type != ProductionType::kNonterminal) {
      new_productions.push_back(production);
      continue;
    }
    int nullable_cnt = 0;
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      for (Symbol &symbol : conjunct) {
        if (is_nullable_[symbol.value]) nullable_cnt++;
      }
    }
    for (int mask = 0; mask < (1 << nullable_cnt); mask++) {
      int it = 0;
      std::vector<std::vector<Symbol>> new_conjunction;
      bool discard = false;
      for (std::vector<Symbol> &conjunct : production.conjunction) {
        std::vector<Symbol> new_conjunct;
        for (Symbol &symbol : conjunct) {
          if (!is_nullable_[symbol.value] || (mask & (1 << (it++))))
            new_conjunct.push_back(symbol);
        }
        if (new_conjunct.empty()) {
          discard = true;
          break;
        } else
          new_conjunction.push_back(new_conjunct);
      }
      if (!discard)
        new_productions.push_back(Production(ProductionType::kNonterminal,
                                             production.producer,
                                             new_conjunction));
    }
  }
  if (is_nullable_[start_symbol_])
    new_productions.push_back(Production(ProductionType::kEpsilon,
                                         start_symbol_,
                                         {{Symbol(SymbolType::kEpsilon)}}));
  productions_ = new_productions;
}

void ConjunctiveGrammar::EliminateLongConjuncts() {
  for (int i = 0; i < (int)productions_.size(); i++) {
    Production &production = productions_[i];
    if (production.type != ProductionType::kNonterminal) continue;
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      while ((int)conjunct.size() > 2) {
        Symbol y = conjunct.back();
        conjunct.pop_back();
        Symbol x = conjunct.back();
        conjunct.pop_back();
        if (nonterminal_pair_to_producer.find({x.value, y.value}) ==
            nonterminal_pair_to_producer.end()) {
          nonterminal_pair_to_producer[{x.value, y.value}] =
              symbol_table_.AddNonterminal();
          productions_.push_back(Production(
              ProductionType::kNonterminal,
              nonterminal_pair_to_producer[{x.value, y.value}], {{x, y}}));
        }
        conjunct.push_back(
            Symbol(SymbolType::kNonterminal,
                   nonterminal_pair_to_producer[{x.value, y.value}]));
      }
    }
  }
}

Production ConjunctiveGrammar::MergeSortedConjuncts(Production &a,
                                                    Production &b) {
  std::vector<std::vector<Symbol>> new_conjunction;
  int it_a = 0, it_b = 0;
  while (it_a < (int)a.conjunction.size() || it_b < (int)b.conjunction.size()) {
    if (it_a == (int)a.conjunction.size() ||
        (it_b < (int)b.conjunction.size() &&
         b.conjunction[it_b] <= a.conjunction[it_a])) {
      if (b.conjunction[it_b] == a.conjunction[it_a]) it_a++;
      new_conjunction.push_back(b.conjunction[it_b++]);
    } else {
      if (a.conjunction[it_a] == std::vector<Symbol>{Symbol(SymbolType::kNonterminal, b.producer)})
        it_a++;
      else
        new_conjunction.push_back(a.conjunction[it_a++]);
    }
  }
  return Production(ProductionType::kNone, a.producer, new_conjunction);
}

void ConjunctiveGrammar::GenerateProductionsBySubstitution(
    Production production, std::vector<Production> &new_productions,
    std::vector<int> &nonterminals_productions_positions, int mode) {
  for (std::vector<Symbol> conjunct : production.conjunction) {
    if ((conjunct.size() == 1) &&
        (conjunct[0].type == SymbolType::kNonterminal) &&
        (conjunct[0].value < production.producer || mode)) {
      for (int i = nonterminals_productions_positions[conjunct[0].value];
           i < (int)new_productions.size() &&
           new_productions[i].producer == conjunct[0].value;
           i++) {
        GenerateProductionsBySubstitution(
            MergeSortedConjuncts(production, new_productions[i]),
            new_productions, nonterminals_productions_positions, mode);
      }
      return;
    }
  }
  if (((int)production.conjunction.size() == 1) &&
      ((int)production.conjunction[0].size() == 1) &&
      (production.conjunction[0][0].type == SymbolType::kTerminal)) {
    production.type = ProductionType::kTerminal;
    new_productions.push_back(production);
    return;
  }
  if (mode) {
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      if (conjunct.size() == 1 && conjunct[0].type == SymbolType::kTerminal)
        return;
    }
  }
  new_productions.push_back(production);
}

void ConjunctiveGrammar::SortProductions() {
  std::vector<std::vector<int>> nonterminal_productions_idxs(
      symbol_table_.GetNonterminalCount());
  for (int i = 0; i < (int)productions_.size(); i++) {
    nonterminal_productions_idxs[productions_[i].producer].push_back(i);
  }
  std::vector<Production> new_productions;
  for (int i = 0; i < symbol_table_.GetNonterminalCount(); i++) {
    for (int idx : nonterminal_productions_idxs[i]) {
      new_productions.push_back(productions_[idx]);
    }
  }
  productions_ = new_productions;

  std::vector<std::pair<int, std::vector<Symbol>>> all_nonterminal_conjuncts;
  for (int i = 0; i < (int)productions_.size(); i++) {
    Production &production = productions_[i];
    if (production.type == ProductionType::kNonterminal) {
      for (std::vector<Symbol> &conjunct : production.conjunction) {
        all_nonterminal_conjuncts.push_back({i, conjunct});
      }
      production.conjunction.clear();
    }
  }


  std::vector<std::vector<std::pair<int, std::vector<Symbol>>>> sorting_table(
      symbol_table_.GetNonterminalCount());
  std::vector<std::pair<int, std::vector<Symbol>>> unit_conjuncts;
  for (auto conjunct : all_nonterminal_conjuncts) {
    if ((int)conjunct.second.size() == 1) unit_conjuncts.push_back(conjunct);
    else sorting_table[conjunct.second.back().value].push_back(conjunct);
  }
  all_nonterminal_conjuncts.clear();
  for (auto conjunct: unit_conjuncts) {
        all_nonterminal_conjuncts.push_back(conjunct);
  }
  for (int i = 0; i < symbol_table_.GetNonterminalCount(); i++) {
    for (auto conjunct : sorting_table[i]) {
       all_nonterminal_conjuncts.push_back(conjunct);

    }
    sorting_table[i].clear();
  }
  sorting_table.shrink_to_fit();
  for (auto conjunct : all_nonterminal_conjuncts) {
    sorting_table[conjunct.second[0].value].push_back(conjunct);
  }
  for (int i = 0; i < symbol_table_.GetNonterminalCount(); i++) {
    for (auto conjunct : sorting_table[i]) {
      productions_[conjunct.first].conjunction.push_back(conjunct.second);
    }
    sorting_table[i].clear();
  }
  for (Production &production : productions_) {
    production.conjunction.erase(std::unique(production.conjunction.begin(),
                                             production.conjunction.end()),
                                 production.conjunction.end());
  }
}

void ConjunctiveGrammar::EliminateUnitConjuncts() {
  SortProductions();
  std::vector<Production> new_productions;
  std::vector<int> nonterminals_productions_positions(symbol_table_.GetNonterminalCount());
  for (int i = 0; i < (int)productions_.size(); i++) {
    Production &production = productions_[i];
    if (i == 0 || production.producer != productions_[i - 1].producer) {
      nonterminals_productions_positions[production.producer] =
          new_productions.size();
    }
    if (production.type != ProductionType::kNonterminal) {
      new_productions.push_back(production);
      continue;
    }
    bool discard = false;
    for (std::vector<Symbol> conjunct : production.conjunction) {
      if ((int)conjunct.size() == 1 &&
          conjunct[0].value == production.producer) {
        discard = true;
        break;
      }
    }
    if (discard) continue;
    GenerateProductionsBySubstitution(production, new_productions,
                                      nonterminals_productions_positions, 0);
  }
  productions_ = new_productions;
  new_productions.clear();
  for (int i = (int)productions_.size() - 1; i >= 0; i--) {
    Production &production = productions_[i];
    if (i == (int)productions_.size() - 1 ||
        production.producer != productions_[i + 1].producer)
      nonterminals_productions_positions[production.producer] =
          new_productions.size();
    if (production.type != ProductionType::kNonterminal) {
      new_productions.push_back(production);
      continue;
    }
    bool discard = false;
    for (std::vector<Symbol> conjunct : production.conjunction) {
      if ((int)conjunct.size() == 1 &&
          conjunct[0].value == production.producer) {
        discard = true;
        break;
      }
    }
    if (discard) continue;
    GenerateProductionsBySubstitution(production, new_productions,
                                      nonterminals_productions_positions, 1);
  }
  productions_ = new_productions;
}
