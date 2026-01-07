#ifndef CONJUNCTIVE_GRAMMARS_PARSER_CONVOLUTIONS_
#define CONJUNCTIVE_GRAMMARS_PARSER_CONVOLUTIONS_
#include <vector>

std::vector<char> StandardConvolution(std::vector<char>& x, int lx, int rx,
                                      std::vector<char>& y, int ly, int ry);
std::vector<char> MultiplicationConvolution(std::vector<char>& x, int lx,
                                            int rx, std::vector<char>& y,
                                            int ly, int ry);

#endif  // CONJUNCTIVE_GRAMMARS_PARSER_CONVOLUTIONS_