#ifndef CONJUNCTIVE_GRAMMARS_PARSER_CONVOLUTIONS_
#define CONJUNCTIVE_GRAMMARS_PARSER_CONVOLUTIONS_
#include <vector>
using namespace std;

vector<char> StandardConvolution(vector<char>& x, int lx, int rx, vector<char>& y, int ly, int ry);
vector<char> MultiplicationConvolution(vector<char>& x, int lx, int rx, vector<char>& y, int ly, int ry);



#endif  // CONJUNCTIVE_GRAMMARS_PARSER_CONVOLUTIONS_