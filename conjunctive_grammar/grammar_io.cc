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

bool GrammarIO::Read(std::istream &input, int &start_symbol, std::vector<Production> &productions,
                     SymbolTable &symbol_table) {
  if (ReadNonterminals(input, start_symbol, symbol_table) == -1) return false;
  if (ReadTerminals(input, symbol_table) == -1) return false;
  int status;
  while ((status = ReadProduction(input, productions, symbol_table)) == 1)
    ;
  if (status == -1) return false;
  return true;
}

void GrammarIO::Print(std::ostream &output, int &start_symbol, std::vector<Production> &productions,
                      SymbolTable &symbol_table) {
  PrintNonterminals(output, start_symbol, symbol_table);
  PrintTerminals(output, symbol_table);
  PrintProductions(output, productions, symbol_table);
}

std::vector<std::string> GrammarIO::GetTokenizedLine(std::istream &input) {
  std::string line;
  getline(input, line);
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

int GrammarIO::ReadProduction(std::istream &input, std::vector<Production> &productions,
                              SymbolTable &symbol_table) {
  std::string production_error = "Invalid format of production: ";

  std::vector<std::string> line = GetTokenizedLine(input);
  if (line.size() == 0) return 0;
  if (line.size() < 3 || !symbol_table.IsNonterminal(line[0]) ||
      line[1] != "->") {
    return Error(
        production_error +
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

int GrammarIO::ReadNonterminals(std::istream &input, int &start_symbol, SymbolTable &symbol_table) {
  std::vector<std::string> line = GetTokenizedLine(input);
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

int GrammarIO::ReadTerminals(std::istream &input, SymbolTable &symbol_table) {
  std::vector<std::string> line = GetTokenizedLine(input);
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

void GrammarIO::PrintProductions(std::ostream &output, std::vector<Production> &productions,
                                 SymbolTable &symbol_table) {
  for (Production &production : productions) {
    output << symbol_table.GetNonterminalName(production.producer) << " -> ";
    bool first_conjunct = true;
    for (std::vector<Symbol> &conjunct : production.conjunction) {
      if (!first_conjunct) {
        output << "& ";
      }
      first_conjunct = false;
      for (Symbol &symbol : conjunct) {
        if (symbol.type == SymbolType::kNonterminal) {
          output << symbol_table.GetNonterminalName(symbol.value) << ' ';
        } else if (symbol.type == SymbolType::kTerminal) {
          output << symbol_table.GetTerminalName(symbol.value) << ' ';
        } else {
          output << kEmptyStringSym << ' ';
        }
      }
    }
    output << '\n';
  }
}

void GrammarIO::PrintTerminals(std::ostream &output, SymbolTable &symbol_table) {
  for (int i = 0; i < symbol_table.GetTerminalCount(); i++) {
    output << symbol_table.GetTerminalName(i) << ' ';
  }
  output << '\n';
}

void GrammarIO::PrintNonterminals(std::ostream &output, int &start_symbol,
                                  SymbolTable &symbol_table) {
  output << symbol_table.GetNonterminalName(start_symbol) << ' ';
  for (int i = 0; i < symbol_table.GetNonterminalCount(); i++) {
    if (i == start_symbol) continue;
    output << symbol_table.GetNonterminalName(i) << ' ';
  }
  output << '\n';
}

}  // namespace conjunctive_grammar