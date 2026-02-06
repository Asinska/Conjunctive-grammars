#ifndef SOLVER_CONVOLUTION_H_
#define SOLVER_CONVOLUTION_H_
#include <vector>

std::vector<char> StandardConvolution(const std::vector<char>& x, int lx,
                                      int rx, const std::vector<char>& y,
                                      int ly, int ry);
std::vector<char> MultiplicationConvolution(const std::vector<char>& x, int lx,
                                            int rx, const std::vector<char>& y,
                                            int ly, int ry);

#endif  // SOLVER_CONVOLUTION_H_