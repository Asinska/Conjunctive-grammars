#ifndef STRUCTURES
#define STRUCTURES

#include <vector>
#include "helpers.h"
using namespace std;

struct Nonterminal {
    bool ProducesTerminal = false;
    vector<char> V;
    Nonterminal() {
        V.resize(2);
    }
};


struct NonterminalPair {
    Nonterminal *A, *B;
    vector<char> convolution;

    NonterminalPair(Nonterminal* A, Nonterminal* B) : A(A), B(B) {
        convolution.resize(2);
    }

    void eval(int i) {
        for (int k = 1; (k < i) && ((i%k) == 0); k <<= 1) {
            vector<char> u1 = multconv(A->V, k, 2*k, B->V, i-k, i);
            vector<char> u2 = multconv(B->V, k, 2*k, A->V, i-k, i);
            for (int j = i; j <= i+2*k-2; j++)  {
                if ((int)convolution.size() <= j ) convolution.push_back(u1[j-i+2] || u2[j-i+2]);
                else convolution[j] = (convolution[j] || u1[j-i+2] || u2[j-i+2]); 
            }
        }
    }
};

struct Production {
    Nonterminal* N;
    vector<pair<bool, NonterminalPair*>> conjunction;
    Production(Nonterminal *nt, vector<pair<bool, NonterminalPair*>> v) : N(nt), conjunction(v) {}
};

extern vector<Nonterminal*> nonterminals;
extern vector<NonterminalPair*> allpairs;
extern vector<Production> productions;

extern int n;


#endif