#ifndef PARSER_CONVOLUTIONS_H_
#define PARSER_CONVOLUTIONS_H_
#include <vector>

std::vector<char> StandardConvolution(std::vector<char>& x, int lx, int rx,
                                      std::vector<char>& y, int ly, int ry);
std::vector<char> MultiplicationConvolution(std::vector<char>& x, int lx,
                                            int rx, std::vector<char>& y,
                                            int ly, int ry);

#endif  // PARSER_CONVOLUTIONS_H_