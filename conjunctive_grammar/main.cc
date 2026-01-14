#include "conjunctive_grammar/grammar.h"

int main() {
  conjunctive_grammar::ConjunctiveGrammar grammar = conjunctive_grammar::ConjunctiveGrammar();
  grammar.Read();
  grammar.Normalise();
  grammar.Print();
}