#include "solver/grammar_solver.h"

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "grammar_solver.h"
#include "solver/convolution.h"
#include "solver/solver_types.h"

namespace {
constexpr std::string_view kNegSym = "~";
constexpr std::string_view kEmptyStringSym = "eps";
}  // namespace

namespace grammar_solver {

GrammarSolver::GrammarSolver(conjunctive_grammar::ConjunctiveGrammar& grammar,
                             int n)
    : n_(n), start_symbol_(nullptr) {
  for (int i = 0; i < grammar.GetNonterminalsCnt(); i++) {
    nonterminals_.push_back(std::make_unique<Nonterminal>(n));
  }
  start_symbol_ = nonterminals_[grammar.GetStartSymbol()].get();

  std::vector<conjunctive_grammar::Production> grammar_productions =
      grammar.GetProductions();
  for (conjunctive_grammar::Production production : grammar_productions) {
    if (production.type == conjunctive_grammar::ProductionType::kEpsilon) {
      nonterminals_[production.producer]->v[0] = 1;
      continue;
    } else if (production.type ==
               conjunctive_grammar::ProductionType::kTerminal) {
      nonterminals_[production.producer]->produces_terminal = true;
      continue;
    } else {
      std::vector<std::pair<bool, NonterminalPair*>> list_of_pairs;
      for (std::vector<conjunctive_grammar::Symbol> conjunct :
           production.conjunction) {
        std::pair<Nonterminal*, Nonterminal*> pair_of_nonterminals = {
            nonterminals_[conjunct[0].value].get(),
            nonterminals_[conjunct[1].value].get()};
        if (nonterminal_pair_to_pointer_.find(pair_of_nonterminals) ==
            nonterminal_pair_to_pointer_.end()) {
          auto newpair = std::make_unique<NonterminalPair>(
              pair_of_nonterminals.first, pair_of_nonterminals.second);
          nonterminal_pair_to_pointer_[pair_of_nonterminals] = newpair.get();
          nonterminal_pairs_.push_back(move(newpair));
        }
        list_of_pairs.push_back(
            {true, nonterminal_pair_to_pointer_[pair_of_nonterminals]});
      }
      productions_.push_back(SolverProduction(
          nonterminals_[production.producer].get(), list_of_pairs));
    }
  }
}

std::unique_ptr<GrammarSolver> GrammarSolver::Create(
    conjunctive_grammar::ConjunctiveGrammar& grammar, int n) {
  if (grammar.GetTerminalsCnt() != 1) {
    std::cerr << "Cannot create solver for non-unary grammar.\n";
    return nullptr;
  }
  if (!grammar.IsNormal()) {
    std::cerr << "Cannot create solver for unnormalized grammar.\n";
    return nullptr;
  }
  return std::unique_ptr<GrammarSolver>(new GrammarSolver(grammar, n));
}

void GrammarSolver::Solve() {
  for (const auto& nt : nonterminals_) {
    if (nt->produces_terminal) {
      nt->v[1] = true;
    }
  }

  for (int i = 2; i <= n_; i++) {
    for (const auto& pair : nonterminal_pairs_) {
      EvaluatePair(pair.get(), i);
    }

    for (const auto& production : productions_) {
      bool conjunction_satisfied = true;
      for (const auto& conjunct : production.conjunction) {
        conjunction_satisfied =
            conjunction_satisfied &&
            (conjunct.first == conjunct.second->convolution[i]);
      }

      Nonterminal* producer = production.producer;
      if (static_cast<int>(producer->v.size()) <= i) {
        producer->v.push_back(conjunction_satisfied);
      } else {
        producer->v[i] = producer->v[i] || conjunction_satisfied;
      }
    }
  }
}

std::vector<char> GrammarSolver::GetResult() { return start_symbol_->v; }

void GrammarSolver::EvaluatePair(NonterminalPair* nonterminal_pair, int i) {
  for (int k = 1; (k < i) && ((i % k) == 0); k <<= 1) {
    std::vector<char> u1 =
        MultiplicationConvolution(nonterminal_pair->first->v, k, 2 * k,
                                  nonterminal_pair->second->v, i - k, i);
    std::vector<char> u2 =
        MultiplicationConvolution(nonterminal_pair->second->v, k, 2 * k,
                                  nonterminal_pair->first->v, i - k, i);
    for (int j = i; j <= i + 2 * k - 2; j++) {
      if ((int)nonterminal_pair->convolution.size() <= j)
        nonterminal_pair->convolution.push_back(u1[j - i + 2] || u2[j - i + 2]);
      else
        nonterminal_pair->convolution[j] = (nonterminal_pair->convolution[j] ||
                                            u1[j - i + 2] || u2[j - i + 2]);
    }
  }
}

}  // namespace grammar_solver