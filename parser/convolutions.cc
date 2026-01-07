#include <gmpxx.h>

#include <vector>
using namespace std;

vector<char> splice(vector<char>& x, int l, int r) {
  vector<char> result(r - l + 1);
  for (int i = l; i < min(r, (int)x.size()); i++) result[i - l + 1] = x[i];
  return result;
}

vector<char> StandardConvolution(vector<char>& x, int lx, int rx,
                                 vector<char>& y, int ly, int ry) {
  vector<char> xx = splice(x, lx, rx);
  vector<char> yy = splice(y, ly, ry);
  int n = xx.size() - 1;
  vector<char> z(2 * n + 1);
  for (int i = 2; i <= 2 * n; i++) {
    for (int j = 0; j <= i; j++) {
      if (j < (int)xx.size() && (i - j) < (int)yy.size()) {
        z[i] = z[i] || (xx[j] && yy[i - j]);
      }
    }
  }
  return z;
}

vector<char> MultiplicationConvolution(vector<char>& x, int lx, int rx,
                                       vector<char>& y, int ly, int ry) {
  int len = rx - lx;
  int log = 0, power = 1;
  while (power <= len + 1) {
    power *= 2;
    log++;
  }
  mpz_t X, Y, Z;
  mpz_init(X);
  mpz_init(Y);
  mpz_init(Z);

  for (int i = lx; i < min(rx, (int)x.size()); i++) {
    if (x[i]) {
      mpz_setbit(X, (i - lx + 1) * log);
    }
  }
  for (int i = ly; i < min(ry, (int)y.size()); i++) {
    if (y[i]) {
      mpz_setbit(Y, (i - ly + 1) * log);
    }
  }
  mpz_mul(Z, X, Y);
  vector<char> z(2 * len + 1);
  for (int i = 0; i <= 2 * len; i++) {
    for (int j = i * log; j < (i + 1) * log; j++) {
      if (mpz_tstbit(Z, j)) z[i] = true;
    }
  }
  return z;
}