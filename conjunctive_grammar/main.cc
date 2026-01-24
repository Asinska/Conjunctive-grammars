#include <iostream>

#include "conjunctive_grammar/grammar.h"

int main() {
  conjunctive_grammar::ConjunctiveGrammar grammar =
      conjunctive_grammar::ConjunctiveGrammar();
  if (!grammar.Read()) {
    return 0;
  }
  if (!grammar.IsNormal()) {
    grammar.Normalise();
  } else {
    std::cout << "Grammar is already in normal form.\n";
  }
  grammar.Print();
}