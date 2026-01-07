#ifndef CONJUNCTIVE_GRAMMARS_PARSER_GRAMMAR_TYPES_
#define CONJUNCTIVE_GRAMMARS_PARSER_GRAMMAR_TYPES_

#include <vector>
#include "convolutions.h"

struct Nonterminal {
    bool produces_terminal = false;
    std::vector<char> v;
    Nonterminal() {
        v.resize(2);
    }
};


struct NonterminalPair {
    Nonterminal *first, *second;
    std::vector<char> convolution;

    NonterminalPair(Nonterminal* a, Nonterminal* b) : first(a), second(b) {
        convolution.resize(2);
    }

    // void eval(int i) {
    //     for (int k = 1; (k < i) && ((i%k) == 0); k <<= 1) {
    //         std::vector<char> u1 = multconv(A->V, k, 2*k, B->V, i-k, i);
    //         std::vector<char> u2 = multconv(B->V, k, 2*k, A->V, i-k, i);
    //         for (int j = i; j <= i+2*k-2; j++)  {
    //             if ((int)convolution.size() <= j ) convolution.push_back(u1[j-i+2] || u2[j-i+2]);
    //             else convolution[j] = (convolution[j] || u1[j-i+2] || u2[j-i+2]); 
    //         }
    //     }
    // }
};

struct Production {
    Nonterminal* producer;
    std::vector<pair<bool, NonterminalPair*>> conjunction;
    Production(Nonterminal *nonterminal, std::vector<pair<bool, NonterminalPair*>> conjunction) : producer(nonterminal), conjunction(conjunction) {}
};


#endif  // CONJUNCTIVE_GRAMMARS_PARSER_GRAMMAR_TYPES_