typedef void (*t_dydt)(int *neq, double t, double *A, double *DADT);
typedef void (*t_calc_jac)(int *neq, double t, double *A, double *JAC, unsigned int __NROWPD__);
typedef void (*t_calc_lhs)(int cSub, double t, double *A, double *lhs);
typedef void (*t_update_inis)(int cSub, double *);
typedef void (*t_dydt_lsoda_dum)(int *neq, double *t, double *A, double *DADT);
typedef void (*t_jdum_lsoda)(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd);


typedef struct {
  // These options should not change based on an individual solve
  int badSolve;
  double ATOL;          //absolute error
  double RTOL;          //relative error
  double H0;
  double HMIN;
  int global_jt;
  int global_mf;
  int global_debug;
  int mxstep;
  int MXORDN;
  int MXORDS;
  //
  int do_transit_abs;
  int nlhs;
  int neq;
  int stiff;
  int ncov;
  SEXP stateNames;
  SEXP lhsNames;
  SEXP paramNames;
  int *par_cov;
  double *inits;
  double *scale;
  int do_par_cov;
  t_dydt dydt;
  t_calc_jac calc_jac;
  t_calc_lhs calc_lhs;
  t_update_inis update_inis;
  t_dydt_lsoda_dum dydt_lsoda_dum;
  t_jdum_lsoda jdum_lsoda;
  void *set_solve;
  // approx fun options
  double f1;
  double f2;
  int kind;
  int is_locf;
  int cores;
  int extraCmt;
} rx_solving_options;


typedef struct {
  long slvr_counter;
  long dadt_counter;
  long jac_counter;
  double *InfusionRate;
  int *BadDose;
  int nBadDose;
  double HMAX; // Determined by diff
  double tlast;
  double podo;
  double *par_ptr;
  double *dose;
  double *solve;
  double *lhs;
  int  *evid;
  int *rc;
  double *cov_ptr;
  int n_all_times;
  int ixds;
  int ndoses;
  double *all_times;
  int *idose;
  int idosen;
  int id;
  int sim;
  double ylow;
  double yhigh;
} rx_solving_options_ind;

typedef struct {
  rx_solving_options_ind *subjects;
  int nsub;
  int nsim;
  int nobs;
  int add_cov;
  int matrix;
  int *stateIgnore;
  SEXP op;
} rx_solve;

typedef void (*t_set_solve)(rx_solve *);
typedef rx_solve *(*t_get_solve)();


rx_solve *getRxSolve_(SEXP ptr);
void rxSolveDataFree(SEXP ptr);
int rxUpdateResiduals_(SEXP md);
rx_solve *getRxSolve(SEXP ptr);
void freeRxSolve(SEXP ptr);

SEXP getSolvingOptionsPtr(double ATOL,          //absolute error
                          double RTOL,          //relative error
                          double H0,
                          double HMIN,
                          int global_jt,
                          int global_mf,
                          int global_debug,
                          int mxstep,
                          int MXORDN,
                          int MXORDS,
                          // Approx options
                          int do_transit_abs,
                          int nlhs,
                          int neq,
                          int stiff,
                          double f1,
                          double f2,
                          int kind,
                          int is_locf,
                          int cores,
                          int ncov,
                          int *par_cov,
                          int do_par_cov,
                          double *inits,
			  double *scale,
                          SEXP stateNames,
                          SEXP lhsNames,
                          SEXP paramNames,
                          SEXP dydt,
                          SEXP calc_jac,
                          SEXP calc_lhs,
                          SEXP update_inis,
                          SEXP dydt_lsoda_dum,
                          SEXP jdum_lsoda,
                          SEXP set_solve,
                          SEXP get_solve);
void getSolvingOptionsIndPtr(double *InfusionRate,
                             int *BadDose,
                             double HMAX, // Determined by diff
                             double *par_ptr,
                             double *dose,
                             int *idose,
                             double *solve,
                             double *lhs,
                             int *evid,
                             int *rc,
                             double *cov_ptr,
                             int n_all_times,
                             double *all_times,
                             int id,
                             int sim,
                             rx_solving_options_ind *o);
SEXP rxSolveData(rx_solving_options_ind *subjects,
                 int nsub,
                 int nsim,
                 int *stateIgnore,
                 int nobs,
                 int add_cov,
                 int matrix,
                 SEXP op);
void par_solve(rx_solve *rx, SEXP sd, int ini_updateR);

rx_solving_options *getRxOp(rx_solve *rx);
SEXP RxODE_df(SEXP sd, int doDose);
SEXP RxODE_par_df(SEXP sd);
