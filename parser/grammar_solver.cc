#include "grammar_solver.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

// Include your other headers (renamed per previous advice)
#include "convolutions.h"    // Formerly helpers.h
#include "grammar_types.h" // Formerly structures.h (if you kept types separate)

namespace conjunctive_grammar {

// Constants can be defined here if they are private implementation details
// const char* kNegationSym = "~";
// const char* kEmptyStringSym = "eps";
#define NEG_SYM "~"
#define EMPTY_STRING_SYM "eps"

// --- Constructor ---
GrammarSolver::GrammarSolver() : n_(0), start_symbol_(NULL) {}

// --- Public Method: ParseInput ---
void GrammarSolver::ParseInput() {
  ReadN();
//   cout << n_ << '\n';
  ReadGrammar();
}

// --- Public Method: Solve ---
void GrammarSolver::Solve() {
  // If n was not set by ParseInput, use the parameter
  // if (n_ == 0) n_ = n;

  // Initialize base cases for terminals
  for (const auto& nt : nonterminals_) {
    if (nt->produces_terminal) {
      nt->v[1] = true;
    }
  }

  // Dynamic programming / Parsing logic
  for (int i = 2; i <= n_; i++) {
    // 1. Evaluate all convolution pairs
    for (const auto& pair : nonterminal_pairs_) {
      // Assuming you moved eval() logic into this class or kept it in the struct.
      // If kept in struct, call: pair->Eval(i);
      // If moved here, you would implement the logic inline or as a private helper.
      EvaluatePair(pair.get(), i); 
    }

    // 2. Evaluate productions
    for (const auto& production : productions_) {
      bool conjunction_satisfied = true;
      for (const auto& conjunct : production.conjunction) {
        // bool expected = term.first; // The boolean flag (negation)
        // NonterminalPair* pair_ptr = term.second;
        
        // // Check if the pair produced the string of length i
        // // Note: You need bounds checking in real code
        // if (i < pair_ptr->convolution.size()) {
        //      conjunction_satisfied &= (expected == pair_ptr->convolution[i]);
        // } else {
        //      conjunction_satisfied = false;
        // }
        conjunction_satisfied = conjunction_satisfied && (conjunct.first == conjunct.second->convolution[i]);
      }


      // Update the nonterminal's vector
      Nonterminal* producer = production.producer;
      if (static_cast<int>(producer->v.size()) <= i) {
        producer->v.push_back(conjunction_satisfied);
      } else {
        producer->v[i] = producer->v[i] || conjunction_satisfied;
      }
    }
  }
}

// --- Public Method: PrintResult ---
void GrammarSolver::PrintResult() {
  // if (nonterminals_.empty()) return;

  // Assuming the first nonterminal is the start symbol
  // const Nonterminal* start_symbol = nonterminals_[0].get();
  
  for (int i = 0; i <= n_; i++) {
    if (start_symbol_->v[i]) {
      std::cout << i << " ";
    }
  }
  std::cout << "\n";
}

bool GrammarSolver::IsNonterminal(string s)
{
  return name_to_nonterminal_.find(s) != name_to_nonterminal_.end();
}

// --- Private Helper: ReadN ---
void GrammarSolver::ReadN() {
  std::string line;
  std::getline(std::cin, line);
  std::stringstream ss(line);
  if (!(ss >> n_)) {
    Error("Word's length must be a number.");
  }
}

void GrammarSolver::ReadNonterminals()
{
  vector<string> line = GetTokenizedLine();
  if (line.size() == 0) {
      Error("Expected at least one nonterminal symbol.");
  }
//   int cnt = 0;
  for (string s: line) {
      auto nonterminal_pointer = make_unique<Nonterminal>();
      Nonterminal* raw_ptr = nonterminal_pointer.get();
      if (start_symbol_ == nullptr) start_symbol_ = raw_ptr;
      nonterminals_.push_back(move(nonterminal_pointer));
      
      if (IsNonterminal(s)) {
          Error("Nonterminal symbols should be pairwise distinct.");
      }
      name_to_nonterminal_[s] = raw_ptr;
  }
}

void GrammarSolver::ReadTerminal()
{
  vector<string> line = GetTokenizedLine();
  if (line.size() != 1) {
        Error("Exactly one terminal symbol expected.");
    }
    terminal_name_ = line[0];
    if (IsNonterminal(terminal_name_)) {
        Error("Terminal symbol appears in nonterminal symbols list.");
    }
}

bool GrammarSolver::ReadProduction()
{
  vector<string> line = GetTokenizedLine();
  if (line.size() == 0) return false;
  if (line.size() < 3 || !IsNonterminal(line[0]) || line[1] != "->") {
      Error("Production error: Production should consist of one nonterminal symbol, an arrow and at least one nonempty conjunct.");
  }
  if (line.size() == 3) {
      if (line[2] == EMPTY_STRING_SYM) {
        // cout << (int) start_symbol_ << '\n';
          if (name_to_nonterminal_[line[0]] != start_symbol_) {
              Error("Production error: Only start symbol should be able to produce empty word.");
          }
          name_to_nonterminal_[line[0]]->v[0] = true;
      }
      else if (line[2] == terminal_name_)
          name_to_nonterminal_[line[0]]->produces_terminal = true;
      else 
          Error("Production error: rules with one conjunct consisting of one symbol must be terminal productions or empty word productions.");
  }   
  else {
      vector<pair<bool, NonterminalPair*>> v;
      for (int i = 2; i < (int)line.size(); i+=3) {
          bool positive_conjunct = true;
          if (line[i] == NEG_SYM) {
              positive_conjunct = false;
              i++;
          }
          if (i+1 >= (int)line.size() || (i+2 != (int)line.size() && line[i+2] != "&")) {
              Error("Production error.");
          }
          if (!IsNonterminal(line[i]) || !IsNonterminal(line[i])) {
              Error("Production error: unknown nonterminal symbols.");
          }
          pair<Nonterminal*, Nonterminal*> pair_of_nonterminals = {name_to_nonterminal_[line[i]], name_to_nonterminal_[line[i+1]]};
          if (nonterminal_pair_to_pointer_.find(pair_of_nonterminals) == nonterminal_pair_to_pointer_.end()) {
              auto newpair = make_unique<NonterminalPair>(pair_of_nonterminals.first, pair_of_nonterminals.second);
              nonterminal_pair_to_pointer_[pair_of_nonterminals] = newpair.get();
              nonterminal_pairs_.push_back(move(newpair));
          }
          v.push_back({positive_conjunct, nonterminal_pair_to_pointer_[pair_of_nonterminals]});
      }
      productions_.push_back(Production(name_to_nonterminal_[line[0]], v));
  }
  return true;
}

// --- Private Helper: ReadGrammar ---
// This contains the heavy lifting from your old parse_input.cpp
void GrammarSolver::ReadGrammar() {
  ReadNonterminals();
  ReadTerminal();
//   cout << nonterminals_.size() << ' ' << terminal_name_ << '\n';
  while(ReadProduction());
}

// --- Private Helper: EvaluatePair ---
// Logic moved from NonterminalPair::eval to keep structs simple
void GrammarSolver::EvaluatePair(NonterminalPair* nonterminal_pair, int i) {
      for (int k = 1; (k < i) && ((i%k) == 0); k <<= 1) {
        std::vector<char> u1 = MultiplicationConvolution(nonterminal_pair->first->v, k, 2*k, nonterminal_pair->second->v, i-k, i);
        std::vector<char> u2 = MultiplicationConvolution(nonterminal_pair->second->v, k, 2*k, nonterminal_pair->first->v, i-k, i);
        for (int j = i; j <= i+2*k-2; j++)  {
            if ((int)nonterminal_pair->convolution.size() <= j ) nonterminal_pair->convolution.push_back(u1[j-i+2] || u2[j-i+2]);
            else nonterminal_pair->convolution[j] = (nonterminal_pair->convolution[j] || u1[j-i+2] || u2[j-i+2]); 
        }
    }
}



// --- Private Helper: GetTokenizedLine ---
std::vector<std::string> GrammarSolver::GetTokenizedLine() {
  string line;
  getline(cin, line);
  stringstream ss(line);
  vector<string> tokens;
  string tmp;
  while (ss >> tmp) {
      tokens.push_back(tmp);
  }
  return tokens;
}

void GrammarSolver::Error(string error_message)
{
  cerr << "Incorrect input error: " << error_message << '\n';
  exit(EXIT_FAILURE);
}

} // namespace conjunctive_grammar