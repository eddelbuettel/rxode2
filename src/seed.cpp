#define STRICT_R_HEADER
#define min2( a , b )  ( (a) < (b) ? (a) : (b) )
#include <RcppArmadillo.h>
#include "../inst/include/RxODE.h"
#include "rxomp.h"
#include <R.h>
#ifdef ENABLE_NLS
#include <libintl.h>
#define _(String) dgettext ("RxODE", String)
/* replace pkg as appropriate */
#else
#define _(String) (String)
#endif
using namespace Rcpp;
using namespace arma;

bool useRxSeed = false;

uint32_t rxSeed = 0;

extern "C" SEXP _rxSetSeed(SEXP intIn) {
  int type = TYPEOF(intIn);
  if (Rf_length(intIn) != 1) {
    Rf_errorcall(R_NilValue, _("'seed' must be an integer of length 1."));
  }
  if (type == REALSXP) {
    double in = REAL(intIn)[0];
    if (in < 0) {
      rxSeed = 0;
      useRxSeed = false;
    } else {
      rxSeed = (uint32_t)(in);
      useRxSeed = true;
    }
  } else if (type == INTSXP) {
    int in = REAL(intIn)[0];
    if (in < 0) {
      rxSeed = 0;
      useRxSeed = false;
    } else {
      rxSeed = (uint32_t)(in);
      useRxSeed = true;
    }
  } else {
    Rf_errorcall(R_NilValue, _("'seed' must be an integer of length 1."));
  }
  return R_NilValue;
}

uint32_t getRxSeed1(int ncores) {
  uint32_t seed;
  if (useRxSeed) {
    seed = rxSeed;
    rxSeed += ncores;
  } else {
    double seedD = runif(1, 1.0, std::numeric_limits<uint32_t>::max())[0];
    seed = static_cast<uint32_t>(seedD);
    seed = min2(seed, std::numeric_limits<uint32_t>::max() - ncores - 1);
  }
  return seed;
}
