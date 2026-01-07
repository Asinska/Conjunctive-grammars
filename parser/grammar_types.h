#ifndef PARSER_GRAMMAR_TYPES_H_
#define PARSER_GRAMMAR_TYPES_H_

#include <vector>

#include "convolutions.h"

struct Nonterminal {
  bool produces_terminal = false;
  std::vector<char> v;
  Nonterminal() { v.resize(2); }
};

struct NonterminalPair {
  Nonterminal *first, *second;
  std::vector<char> convolution;

  NonterminalPair(Nonterminal* a, Nonterminal* b) : first(a), second(b) {
    convolution.resize(2);
  }
};

struct Production {
  Nonterminal* producer;
  std::vector<std::pair<bool, NonterminalPair*>> conjunction;
  Production(Nonterminal* nonterminal,
             std::vector<std::pair<bool, NonterminalPair*>> conjunction)
      : producer(nonterminal), conjunction(conjunction) {}
};

#endif  // PARSER_GRAMMAR_TYPES_H_