#ifndef CONJUNCTIVE_GRAMMARS_PARSER_GRAMMAR_TYPES_
#define CONJUNCTIVE_GRAMMARS_PARSER_GRAMMAR_TYPES_

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

#endif  // CONJUNCTIVE_GRAMMARS_PARSER_GRAMMAR_TYPES_