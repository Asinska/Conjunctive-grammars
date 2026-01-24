#include "conjunctive_grammar/grammar_io.h"

#include <iostream>
#include <sstream>

#include "conjunctive_grammar/grammar_types.h"
#include "conjunctive_grammar/symbol_table.h"

namespace {
constexpr std::string_view kEmptyStringSym = "eps";
}  // namespace

namespace conjunctive_grammar {

GrammarIO::GrammarIO() {}

bool GrammarIO::Read(int &start_symbol, std::vector<Production> &productions,
                     SymbolTable &symbol_table) {
  if (ReadNonterminals(start_symbol, symbol_table) == -1) return false;
  if (ReadTerminals(symbol_table) == -1) return false;
  int status;
  while ((status = ReadProduction(productions, symbol_table)) == 1)
    ;
  if (status == -1) return false;
  return true;
}

void GrammarIO::Print(int &start_symbol, std::vector<Production> &productions,
                      SymbolTable &symbol_table) {
  PrintNonterminals(start_symbol, symbol_table);
  PrintTerminals(symbol_table);
  PrintProductions(productions, symbol_table);
}

std::vector<std::string> GrammarIO::GetTokenizedLine() {
  std::string line;
  getline(std::cin, line);
  std::stringstream ss(line);
  std::vector<std::string> tokens;
  std::string tmp;
  while (ss >> tmp) {
    tokens.push_back(tmp);
  }
  return tokens;
}

int GrammarIO::Error(std::string error_message) {
  std::cerr << "Incorrect input error: " << error_message << '\n';
  return -1;
}

int GrammarIO::ReadProduction(std::vector<Production> &productions,
                               SymbolTable &symbol_table) {
  std::string production_error = "Invalid format of production: ";

  std::vector<std::string> line = GetTokenizedLine();
  if (line.size() == 0) return 0;
  if (line.size() < 3 || !symbol_table.IsNonterminal(line[0]) ||
      line[1] != "->") {
    return Error(production_error +
          "Production should consist of nonterminal symbol, arrow and positive "
          "number of conjuncts.");
  }

  int producer = symbol_table.GetNonterminalId(line[0]);
  int it = 2;
  while (it < (int)line.size()) {
    std::vector<std::vector<Symbol>> conjuncts;
    while (it < (int)line.size() && line[it] != "|") {
      std::vector<Symbol> conjunct;
      std::string symbol;
      while (it < (int)line.size() && ((symbol = line[it]) != "&") &&
             (symbol != "|")) {
        if (symbol_table.IsNonterminal(symbol)) {
          conjunct.push_back(Symbol(SymbolType::kNonterminal,
                                    symbol_table.GetNonterminalId(symbol)));
        } else if (symbol_table.IsTerminal(symbol)) {
          conjunct.push_back(Symbol(SymbolType::kTerminal,
                                    symbol_table.GetTerminalId(symbol)));
        } else if (symbol == kEmptyStringSym) {
          conjunct.push_back(Symbol(SymbolType::kEpsilon));
        } else {
          return Error(production_error + "Unknown symbol");
        }
        it++;
      }
      if (conjunct.size() > 1) {
        for (Symbol symbol : conjunct) {
          if (symbol.type == SymbolType::kEpsilon) {
            return Error(
                "Conjunct must be either epsilon or (nonterminals + "
                "terminals)+");
          }
        }
      }
      if (conjunct.empty()) {
        return Error("Conjunct must be nonempty");
      }
      conjuncts.push_back(conjunct);
      if (it == (int)line.size() || line[it] == "|") {
        break;
      }
      it++;
    }
    if (conjuncts.empty()) {
      return Error("Conjunction must be nonempty.");
    }
    productions.push_back(Production(producer, conjuncts));
    it++;
  }
  return 1;
}

int GrammarIO::ReadNonterminals(int &start_symbol, SymbolTable &symbol_table) {
  std::vector<std::string> line = GetTokenizedLine();
  if (line.size() == 0) {
    return Error("Expected at least one nonterminal symbol.");
  }
  start_symbol = -1;
  for (std::string symbol : line) {
    if (symbol_table.IsNonterminal(symbol)) {
      return Error("Nonterminal symbols should be pairwise distinct.");
    }
    symbol_table.AddNonterminal(symbol);
    if (start_symbol == -1) {
      start_symbol = symbol_table.GetNonterminalId(symbol);
    }
  }
  return 1;
}

int GrammarIO::ReadTerminals(SymbolTable &symbol_table) {
  std::vector<std::string> line = GetTokenizedLine();
  if (line.size() == 0) {
    return Error("Expected at least one terminal symbol.");
  }
  for (std::string symbol : line) {
    if (symbol_table.IsNonterminal(symbol))
      return Error("Terminal symbol appears in nonterminal symbols list.");
    if (symbol_table.IsTerminal(symbol))
      return Error("Terminal symbols should be pairwise distinct.");
    symbol_table.AddTerminal(symbol);
  }
  return 1;
}

void GrammarIO::PrintProductions(std::vector<Production> &productions,
                                 SymbolTable &symbol_table) {
  for (Production &production : productions) {
    std::cout << symbol_table.GetNonterminalName(production.producer) << " -> ";
    bool first_conjunct = true;
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      if (!first_conjunct) {
        std::cout << "& ";
      }
      first_conjunct = false;
      for (Symbol &symbol : conjunct) {
        if (symbol.type == SymbolType::kNonterminal) {
          std::cout << symbol_table.GetNonterminalName(symbol.value) << ' ';
        } else if (symbol.type == SymbolType::kTerminal) {
          std::cout << symbol_table.GetTerminalName(symbol.value) << ' ';
        } else {
          std::cout << kEmptyStringSym << ' ';
        }
      }
    }
    std::cout << '\n';
  }
}

void GrammarIO::PrintTerminals(SymbolTable &symbol_table) {
  for (int i = 0; i < symbol_table.GetTerminalCount(); i++) {
    std::cout << symbol_table.GetTerminalName(i) << ' ';
  }
  std::cout << '\n';
}

void GrammarIO::PrintNonterminals(int &start_symbol,
                                  SymbolTable &symbol_table) {
  std::cout << symbol_table.GetNonterminalName(start_symbol) << ' ';
  for (int i = 0; i < symbol_table.GetNonterminalCount(); i++) {
    if (i == start_symbol) continue;
    std::cout << symbol_table.GetNonterminalName(i) << ' ';
  }
  std::cout << '\n';
}

}  // namespace conjunctive_grammar