#include "conjunctive_grammar/symbol_names_generator.h"

#include <algorithm>
#include <string>

SymbolNamesGenerator::SymbolNamesGenerator()
    : idx_(0), current_length_(1), current_symbols_cnt(kAlphabetSize) {}

std::string SymbolNamesGenerator::Generate() {
  std::string symbol_name = "";
  int idx_copy = idx_;
  for (int i = 0; i < current_length_; i++) {
    symbol_name.push_back('A' + idx_copy % kAlphabetSize);
    idx_copy /= kAlphabetSize;
  }
  symbol_name.push_back('_');
  reverse(symbol_name.begin(), symbol_name.end());

  idx_++;
  if (idx_ >= current_symbols_cnt) {
    current_symbols_cnt *= kAlphabetSize;
    current_length_++;
    idx_ = 0;
  }
  return symbol_name;
}