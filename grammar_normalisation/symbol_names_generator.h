#ifndef GRAMMAR_NORMALISATION_SYMBOL_NAMES_GENERATOR_H_
#define GRAMMAR_NORMALISATION_SYMBOL_NAMES_GENERATOR_H_

#include <string>

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

#endif	// GRAMMAR_NORMALISATION_SYMBOL_NAMES_GENERATOR_H_