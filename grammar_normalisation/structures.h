#ifndef STRUCTURES
#define STRUCTURES
#define EMPTY_STRING_SYM "eps"
#define EMPTY_STRING_NONTERMINAL "_eps"
#include <vector>
#include <map>
#include <string>
using namespace std;

struct Production {
    int N;
    vector<vector<int>> conjunction;
    Production(int nt, vector<vector<int>> v) : N(nt), conjunction(v) {}
};


extern vector<string> nonterminals, terminals;
// int start_symbol_number = 0;
extern map<string, int> nt_symbols_to_numbers, t_symbols_to_numbers;
// int nonterminals_cnt, terminals_cnt;
extern vector<Production> nonterminal_productions, terminal_productions;
extern vector<bool> nullable;
extern vector<int> nt_generator;
extern map<pair<int, int>, int> nt_pair_generator;

#endif