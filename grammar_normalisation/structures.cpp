#include <vector>
#include <string>
#include <map>
#include "structures.h"
using namespace std;

vector<string> nonterminals, terminals;
// start_symbol_number = 0;
map<string, int> nt_symbols_to_numbers, t_symbols_to_numbers;
// nonterminals_cnt, terminals_cnt;
vector<Production> nonterminal_productions, terminal_productions;
vector<bool> nullable;
vector<int> nt_generator;
map<pair<int, int>, int> nt_pair_generator;
