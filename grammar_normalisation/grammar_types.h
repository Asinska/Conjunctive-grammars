#ifndef GRAMMAR_NORMALISATION_GRAMMAR_TYPES_H_
#define GRAMMAR_NORMALISATION_GRAMMAR_TYPES_H_

#include <vector>

enum class SymbolType {
  kTerminal,
  kNonterminal,
  kEpsilon
};

struct Symbol {
  SymbolType type;
  int value;
  Symbol(SymbolType type) : type(type), value(-1) {}
  Symbol(SymbolType type, int value) : type(type), value(value) {}
};

enum class ProductionType {
  kTerminal,
  kNonterminal,
  kEpsilon,
  kNone
};

struct Production {
  ProductionType type;
  int producer;
  std::vector<std::vector<Symbol>> conjunction;
  Production(int nonterminal_number, std::vector<std::vector<Symbol>> v) : producer(nonterminal_number), conjunction(v) {}
};

#endif  // GRAMMAR_NORMALISATION_GRAMMAR_TYPES_H_