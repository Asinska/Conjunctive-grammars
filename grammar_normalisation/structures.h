#ifndef STRUCTURES
#define STRUCTURES

#define NEG_SYM "~"
#define EMPTY_STRING_SYM "eps"
#define EMPTY_STRING_NONTERMINAL "_eps"
#define NULLABLE_DIRECT_PRODUCTION 2
#define NULLABLE_INDIRECT_PRODUCTION 1
#define NOT_NULLABLE 0
#include <vector>
#include <map>
#include <string>
using namespace std;

struct Production {
    int N;
    vector<pair<bool, vector<int>>> conjunction;
    Production(int nt, vector<pair<bool, vector<int>>> v) : N(nt), conjunction(v) {}
};


extern vector<string> nonterminals, terminals;
// int start_symbol_number = 0;
extern map<string, int> nt_symbols_to_numbers, t_symbols_to_numbers;
// int nonterminals_cnt, terminals_cnt;
extern vector<Production> nonterminal_productions, terminal_productions;
extern vector<int> nullable;
extern vector<int> nt_generator;

#endif