#include <vector>
#include <algorithm>
#include <iostream> // for debug
#include "structures.h"
#include "read.h"
#include "helpers.h"
#include "print.h"
// #define NEG_SYM "~"
// #define EMPTY_STRING_SYM "eps"
// #define EMPTY_STRING_NONTERMINAL "_eps"
// #define NULLABLE_DIRECT_PRODUCTION 2
// #define NULLABLE_INDIRECT_PRODUCTION 1
// #define NOT_NULLABLE 0


using namespace std;

// vector<string> nonterminals, terminals;
// int start_symbol_number = 0;
// map<string, int> nt_symbols_to_numbers, t_symbols_to_numbers;
// int nonterminals_cnt, terminals_cnt;
// vector<Production> nonterminal_productions, terminal_productions;
// vector<int> nullable;
// vector<int> nt_generator;
// int cnt = 0;





void read_grammar() {
    read_nonterminals();
    read_terminals();
    int empty_string_nonterminal = introduce_new_nonterminal(EMPTY_STRING_SYM);
    nullable[empty_string_nonterminal] = 1;
    read_all_productions();
}

void find_nullable_set() {
    sort(nonterminal_productions.begin(), nonterminal_productions.end(), [](Production x, Production y) {
        if (x.N != y.N) return x.N < y.N;
        return x.conjunction < y.conjunction;
    });
    for (int i = 0; i < (1<<((int)nonterminals.size())); i++) {
        bool change = false;
        for (int j = 0; j < (int)nonterminal_productions.size(); j++) {
            int NT = nonterminal_productions[j].N;
            if (nullable[NT] == NULLABLE_DIRECT_PRODUCTION) continue;
            bool nullable_nonterminal = false;
            while ((j < (int)nonterminal_productions.size()) && (nonterminal_productions[j].N == NT)) {
                bool nullable_conjunction = true;
                for (auto conjunct: nonterminal_productions[j].conjunction) {
                    bool nullable_conjunct = true;
                    for (int nonterminal: conjunct.second) {
                        if (nullable[nonterminal] != NOT_NULLABLE) {
                            nullable_conjunct = false;
                            break;
                        }
                    }
                    if (conjunct.first == false) {
                        nullable_conjunct ^= true;
                    }
                    if (nullable_conjunct == false){
                        nullable_conjunct = false;
                        break;
                    }
                }    
                nullable_nonterminal |= nullable_conjunction;
                j++;
            }
            if (nullable_nonterminal != nullable[NT]) {
                nullable[NT] = nullable_nonterminal;
                change = true;
            }
        }
        if (!change) break;
    }
    for (int i = 0; i < (int)nonterminals.size(); i++) {
        if (nullable[i]) cout << nonterminals[i] << ' ';
    }
    cout << '\n';
}

void print_grammar() {
    print_nonterminals();
    print_terminals();
    print_productions();    
}

int main() {
    read_grammar();
    find_nullable_set();
    print_grammar();
}