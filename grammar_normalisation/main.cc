#include "grammar.h"

int main() {
  ConjunctiveGrammar grammar = ConjunctiveGrammar();
  grammar.Read();
  grammar.Normalise();
  grammar.Print();
}