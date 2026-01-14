#ifndef CONJUNCTIVE_GRAMMAR_SYMBOL_NAMES_GENERATOR_H_
#define CONJUNCTIVE_GRAMMAR_SYMBOL_NAMES_GENERATOR_H_

#include <string>

namespace conjunctive_grammar {

class SymbolNamesGenerator {
 public:
  SymbolNamesGenerator();
  std::string Generate();

 private:
  const int kAlphabetSize = 26;
  int idx_;
  int current_length_;
  int current_symbols_cnt;
};

}  // namespace conjunctive_grammar

#endif  // CONJUNCTIVE_GRAMMAR_SYMBOL_NAMES_GENERATOR_H_