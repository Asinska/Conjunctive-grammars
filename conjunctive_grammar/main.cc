#include "conjunctive_grammar/grammar.h"

int main() {
  conjunctive_grammar::ConjunctiveGrammar grammar =
      conjunctive_grammar::ConjunctiveGrammar();
  if (!grammar.Read()) {
    return 0;
  }
  grammar.Normalise();
  grammar.Print();
}