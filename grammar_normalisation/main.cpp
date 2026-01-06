#include <vector>
#include <algorithm>
#include <iostream> // for debug
#include <queue>
#include "structures.h"
#include "read.h"
#include "helpers.h"
#include "print.h"
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
    nullable[empty_string_nonterminal] = true;
    read_all_productions();
}

void find_nullable_set() {
    vector<vector<int>> appears_in_production(nonterminals.size());
    vector<int>deg(nonterminal_productions.size());
    for (int i = 0; i < (int)nonterminal_productions.size(); i++) {
        for (vector<int> conjunct: nonterminal_productions[i].conjunction) {
            for (int nt: conjunct) {
                appears_in_production[nt].push_back(i);
                deg[i]++;
            }
        }
    }
    queue<int> nullable_q;
    for (int i = 0; i < (int)nonterminals.size(); i++) {
        if (nullable[i]) {
            nullable_q.push(i);
        }
    }
    while (!nullable_q.empty()) {
        int nt = nullable_q.front();
        nullable_q.pop();
        for (int i: appears_in_production[nt]) {
            deg[i]--;
            if (deg[i] == 0) {
                if(nullable[nonterminal_productions[i].N]) continue;
                nullable[nonterminal_productions[i].N] = true;
                nullable_q.push(nonterminal_productions[i].N);
            }
        }
    }
    // for (int i = 0; i < (int)nonterminals.size(); i++) {
    //     if (nullable[i]) cout << "Nullable: " << nonterminals[i] << '\n';
    // }
}

void eliminate_epsilon_conjuncts() {
    vector<Production> new_productions;
    for (Production p: nonterminal_productions) {
        int nullable_cnt = 0;
        for (vector<int> conjunct: p.conjunction) {
            for (int nt: conjunct) {
                if (nullable[nt]) nullable_cnt++;
            }
        }
        for (int mask = 0; mask < (1<<nullable_cnt); mask++) {
            int it = 0;
            vector<vector<int>> new_conjunction;
            bool discard = false;
            for (vector<int> conjunct: p.conjunction) {
                vector<int> new_conjunct;
                for (int nt: conjunct) {
                    if (!nullable[nt] || (nullable[nt] && (mask&(1<<(it++))))) new_conjunct.push_back(nt);
                }
                if (new_conjunct.empty()) {
                    discard = true;
                    break;
                }
                else new_conjunction.push_back(new_conjunct);
            }
            if (!discard) new_productions.push_back(Production(p.N, new_conjunction));
        }
    }
    nonterminal_productions = new_productions; 
}

void eliminate_long_conjuncts() {
    for (int i = 0; i < (int)nonterminal_productions.size(); i++) {
        Production &p = nonterminal_productions[i];
        for (vector<int> &conjunct: p.conjunction) {
            while ((int)conjunct.size() > 2) {
                int y = conjunct.back();
                conjunct.pop_back();
                int x = conjunct.back();
                conjunct.pop_back();
                if (nt_pair_generator.find({x, y}) == nt_pair_generator.end()) {
                    nt_pair_generator[{x, y}] = introduce_new_nonterminal();
                    nonterminal_productions.push_back(Production(nt_pair_generator[{x, y}], {{x, y}}));
                }
                conjunct.push_back(nt_pair_generator[{x, y}]);
            }
        }
    }
}


Production merge(Production a, Production b) {
    vector<vector<int>> new_conjunction;
    int it_a = 0, it_b = 0;
    while (it_a < (int)a.conjunction.size() || it_b < (int)b.conjunction.size()) {
        if (it_a == (int)a.conjunction.size() || (it_b < (int)b.conjunction.size() && b.conjunction[it_b] <= a.conjunction[it_a])) {
            if (b.conjunction[it_b] == a.conjunction[it_a]) it_a++;
            new_conjunction.push_back(b.conjunction[it_b++]);
        }
        else {
            if (a.conjunction[it_a] == vector<int>{b.N}) it_a++;
            else new_conjunction.push_back(a.conjunction[it_a++]);
        }
    }
    return Production(a.N, new_conjunction);
}
void generate_new_productions(Production p, vector<Production> &new_productions, vector<int> &l, int mode) {
    for (vector<int> conjunct: p.conjunction) {
        if ((conjunct.size() == 1) && (conjunct[0] < (int)nonterminals.size()) && (conjunct[0] < p.N || mode)) {
            for (int i = l[conjunct[0]]; i < (int)new_productions.size() && new_productions[i].N == conjunct[0]; i++) {
                generate_new_productions(merge(p, new_productions[i]), new_productions, l, mode);
            }
            for (int i = 0; i < (int)terminal_productions.size(); i++) {
                if (terminal_productions[i].N == conjunct[0]) {
                    // cout << "!! " << terminal_productions[i].N << ' ' << terminal_productions[i].conjunction[0][0] << ' ' << terminal_productions[i].conjunction[0][0]+(int)nonterminals.size() << '\n';
                    generate_new_productions(merge(p, Production(conjunct[0], {{terminal_productions[i].conjunction[0][0]+(int)nonterminals.size()}})), new_productions, l, mode);

                }
            }
            return;
        }
    }
    if (((int)p.conjunction.size() == 1) && ((int)p.conjunction[0].size() == 1) && (p.conjunction[0][0] >= (int)nonterminals.size())) {
        p.conjunction[0][0] -= (int)nonterminals.size();
        terminal_productions.push_back(p);
        // cout << "! " << p.N <<' ' <<  p.conjunction[0][0] << '\n';
        return;
    }
    if (mode) {
        for (vector<int> conjunct: p.conjunction) {
            if (conjunct.size() == 1 && conjunct[0] >= (int)nonterminals.size()) return;
        }
    }
    new_productions.push_back(p);
}


void eliminate_unit_conjuncts() {
    vector<Production> new_productions;
    // int it = 0;
    vector<int> l(nonterminals.size());
    for (int i = 0; i < (int)nonterminal_productions.size(); i++) {
        Production p = nonterminal_productions[i];
        if (i == 0 || p.N != nonterminal_productions[i-1].N) l[p.N] = new_productions.size();
        sort(p.conjunction.begin(), p.conjunction.end());
        p.conjunction.erase(unique(p.conjunction.begin(), p.conjunction.end()), p.conjunction.end());
        bool discard = false;
        for (vector<int> conjunct: p.conjunction) {
            if ((int)conjunct.size() == 1 && conjunct[0] == p.N) {
                discard = true;
                break;
            }
        }
        if (discard) continue;
        generate_new_productions(p, new_productions, l, 0);
    }
    nonterminal_productions = new_productions;
    new_productions.clear();
    for (int i = (int)nonterminal_productions.size()-1; i>= 0; i--) {
        Production p = nonterminal_productions[i];
        if (i == (int)nonterminal_productions.size() -1 || p.N != nonterminal_productions[i+1].N) l[p.N] = new_productions.size();
        // sort(p.conjunction.begin(), p.conjunction.end());
        // p.conjunction.erase(unique(p.conjunction.begin(), p.conjunction.end()), p.conjunction.end());
        bool discard = false;
        for (vector<int> conjunct: p.conjunction) {
            if ((int)conjunct.size() == 1 && conjunct[0] == p.N) {
                discard = true;
                break;
            }
        }
        if (discard) continue;
        generate_new_productions(p, new_productions, l, 1);
    }
    nonterminal_productions = new_productions;
}

void print_grammar() {
    print_nonterminals();
    print_terminals();
    print_productions();    
}

int main() {
    read_grammar();
    find_nullable_set();
    eliminate_epsilon_conjuncts();
    eliminate_long_conjuncts();
    eliminate_unit_conjuncts();
    print_grammar();
}