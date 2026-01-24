#include <iostream>
#include <fstream>

#include "conjunctive_grammar/grammar.h"

int main(int argc, char const **argv) {
  std::ifstream instream;
  std::ofstream outstream; 
  std::istream* input_ptr = &std::cin;
  std::ostream* output_ptr = &std::cout;

  if (argc >= 2) {
    instream.open(argv[1]);
    if (instream.is_open()) {
      input_ptr = &instream;
    } else {
      std::cerr << "Failed to open " << argv[1] << '\n';
      return 1;
    }
  }

  if (argc >= 3) {
    outstream.open(argv[2]);
    if (outstream.is_open()) {
      output_ptr = &outstream;
    } else {
      std::cerr << "Failed to open " << argv[2] << '\n';
      return 1;
    }
  }
  conjunctive_grammar::ConjunctiveGrammar grammar =
      conjunctive_grammar::ConjunctiveGrammar();
  
  if (!grammar.Read(*input_ptr)) {
    return 1;
  }
  if (!grammar.IsNormal()) {
    grammar.Normalise();
  } else {
    std::cout << "Grammar is already in normal form.\n";
  }
  grammar.Print(*output_ptr);
}